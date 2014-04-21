#pragma once

#include <exception>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <new>
#include <utility>
#include <type_traits>
#include <queue>

// Forward declaration (this comes after the pool itself)
template <typename T>
class PoolAllocator;

// Forward declaration (this comes after the pool itself)
template <typename T>
class PoolIterator;

/**
 * \brief Provides a pool of memory from which a given type can be allocated
 * \tparam The type of the contents of the pool
 *
 * In situations where objects are frequently allocated and deallocated, using the default
 * "new" and "delete", which are thinly wrapped _malloc_ and _free_ calls, has two main problems:
 *
 * 1. The allocations can be placed anywhere on the heap,
 *    giving very poor spatial locality,
 *    possibly causing a large amount of cache misses, and in turn,
 *    huge performance losses.
 * 2. Memory fragmentation can occur if these allocations are done
 *    in conjunction with allocation of other, differently-sized
 *    objects.
 *
 * A memory pool is a pattern that can be used to avoid this.
 * A memory pool is given a fixed maximum size on initialization, at which point
 * it makes a single, up-fornt allocation for the space for that many objects.
 * From then on, memory for objects can be divvied out as-needed without making
 * any actual allocations.
 * These objects will also have good spatial locality,
 * given that they are in the same pool.
 *
 * In order to track which slots in the pool are in use and which aren't,
 * this class stores a singly-linked list of free slots _inside_ the free slots
 * (see the Slot union).
 *
 * At this point, you may be wondering: Isn't there
 * [boost::pool](http://www.boost.org/doc/libs/1_55_0/libs/pool/doc/html/index.html)?
 * Yes. But,
 *
 * 1. This allows for iteration over the pool, which `boost::pool` does not.
 * 2. The author has a bit of an aversion to boost, as it often increases compile times
 *    and is another dependency to worry about. Using boost often feels to the author like
 *    calling in a carrier battle group when a fishing boat is needed.
 *
 * \warning For release builds or profiling, be sure to define NDEBUG in compilation units
 *          using a Pool. Several functions perform O(n) verification steps in functions
 *          that are otherwise O(1).
 */
template <typename T>
class Pool {

public:

	typedef T value_type;

	/// Provides a quick typedef for the pool's allocator (see PoolAllocator)
	typedef PoolAllocator<T> allocator;

	/// Provides a typedef for the unique_ptr returned from Pool functions
	typedef std::unique_ptr<T, std::function<void(T*)>> unique_ptr;

	typedef PoolIterator<T> iterator;

	typedef PoolIterator<const T> const_iterator;

	friend class PoolIterator<T>;
	friend class PoolIterator<const T>;

	/**
	 * \brief Constructs a pool of a given size
	 * \param poolSize The maximum number of elements this pool will be able to store
	 * \throws std::bad_alloc if enough memory for the pool cannot be allocated with malloc
	 */
	Pool(size_t poolSize) :
		buff(nullptr),
		firstFree(nullptr),
		numSlots(poolSize),
		numAllocated(0)
	{
		buff = static_cast<Slot*>(malloc(poolSize * sizeof(Slot)));
		if (buff == nullptr)
			throw std::bad_alloc();

		// Initialize all our slots. Since they all start free,
		// they will point to the next slot.
		for (size_t i = 0; i < poolSize; ++i)
			buff[i].next = &buff[i + 1];

		// The last slot's next pointer should point to null
		buff[poolSize - 1].next = nullptr;
		// The first free slot is our first slot
		firstFree = &buff[0];
	}

	/**
	 * \brief Destructor
	 * \pre All slots in the pool have been freed
	 * \warning If all slots have not been freed by the time this is called,
	 *          std::terminate is called.
	 *          In the author's opinion, this is much better than soldiering on
	 *          in some undefined state with dangling pointers.
	 */
	~Pool()
	{
		// If everything isn't free, we're in undefined territory.
		// Give up!
		if (firstFree != buff || size() != 0) {
			fprintf(stderr, "A pool was destroyed before its elements were freed.\n");
			std::terminate();
		}

		free(buff);
	}

	/// A convenience function to get an allocator for this pool
	PoolAllocator<T> getAllocator() { return PoolAllocator<T>(*this); }

	/**
	 * \brief Gets the number of free slots
	 *
	 * Complexity is O(1) with NDEBUG defined
	 * and O(n) otherwise.
	 */
	size_t remaining() const
	{
#ifndef NDEBUG
		// Just walk the free list until we hit null
		size_t check = 0;
		for (Slot* curr = firstFree; curr != nullptr; curr = curr->next)
			++check;

		assert(check == numSlots - numAllocated);
#endif
		return numSlots - numAllocated;
	}

	/**
	 * \brief Returns the number of currently allocated slots in the pool
	 *
	 * Complexity is O(1)
	 */
	size_t size() const { return numAllocated; }

	/**
	 * \brief  Returns the maximum number of allocations that can be made from the pool
	 *
	 * Complexity is O(1)
	 */
	size_t max_size() const { return numSlots; }

	/**
	 * \brief Returns true if no slots in the pool are allocated
	 *
	 * Complexity is O(1)
	 */
	bool empty() const { return numAllocated == 0; }

	/**
	 * \brief Returns true if no slots in the pool are free
	 *
	 * Complexity is O(1)
	 */
	bool full() const { return numAllocated == numSlots; }

	/**
	 * \brief Allocates (but does not construct)
	 *        _num_ contiguous objects and returns a pointer to the first one
	 * \param num The number of contiguous objects to allocate from the pool
	 * \throws std::bad_alloc if there is not enough space for _num_ congituous objects
	 *         anywhere in the pool
	 * \todo This could be optimized for the case where we are allocating
	 *       a single slot.
	 *
	 * Allocation is done by best-fit, and, in the case of a tie,
	 * by whatever block is first in the pool.
	 *
	 * Complexity is O(n), as we must search for free blocks via the
	 * free list.
	 *
	 * This function is mainly intneded for use with a PoolAllocator
	 * and probably shouldn't be used raw.
	 */
	T* allocate(size_t num)
	{
		// A potential block to allocate
		struct Block {
			// The start of the block
			Slot* start;
			// The number of slots in the block
			size_t size;
			// A pointer to the "next free" pointer pointing to this block.
			// Useful so that we can update it if this block is used.
			Slot** previous;

			Block(Slot* st, size_t sz, Slot** prev) : start(st), size(sz), previous(prev) { }
		};

		// Provides ordering for the priority queue of which block to pick.
		const auto calcFit = [num](const Block& b1, const Block& b2) {
			// We shouldn't have blocks that don't fit our needed amount in the queue
			assert(b1.size >= num);
			assert(b2.size >= num);
			const int n = (int)num;
			const int b1n = (int)b1.size;
			const int b2n = (int)b2.size;
			const int a1 = abs(b1n - n);
			const int a2 = abs(b2n - n);
			if (a1 == a2)
				return b1.start > b2.start; // If these are the same fit, prefer the one closer to the start
			else
				return  a1 > a2; // Sort by best fit
		};

		// By using a priority queue with calcFit, we get a best-fit algorithm.
		std::priority_queue<Block, std::vector<Block>, decltype(calcFit)> bestFitter(calcFit);

		// Iterate through our free list, finding contiguous chunks
		Slot** curr = &firstFree;
		std::pair<size_t, Slot**> countInfo = getContiguousCount(*curr);
		for (; *curr != nullptr; curr = countInfo.second, countInfo = getContiguousCount(*curr)) {
			// If a chunk can fit our needed size, throw it in the priority queue.
			if (countInfo.first >= num)
				bestFitter.emplace(*curr, countInfo.first, curr);
		}

		// We didn't find any block that could meet our request.
		if (bestFitter.empty())
			throw std::bad_alloc();

		numAllocated += num;

		// The top of the priority queue gives us our best fit.
		const auto best = bestFitter.top();

		// Set our previous "next free" pointer to
		// the next pointer at the end of this block,
		// effectively skipping this block
		*best.previous = best.start[num - 1].next;

		return reinterpret_cast<T*>(best.start); // Return our best fit block
	}

	/**
	 * \brief Deallocates _num_ contiguous objects staritng at the given address
	 * \param allocated A pointer to the block of objects to deallocate
	 * \param num The number of contiguous objects to deallocate from the pool
	 * \warning Some verification is done to make sure that _allocated_ is
	 *          a valid pointer to a slot in the pool, but none is made
	 *          to ensure that this block was actually allocated, as doing so
	 *          would be inconveniently expensive.
	 * \todo This could be optimized for the case where we are deallocating
	 *       a single slot.
	 *
	 * Complexity is O(n), as we must iterate through the free list
	 * and update it.
	 *
	 * This function is mainly intneded for use with a PoolAllocator
	 * and probably shouldn't be used raw.
	 */
	void deallocate(T* allocated, size_t num)
	{
		// We can do this since a Slot
		Slot* blockStart = reinterpret_cast<Slot*>(allocated);

		// Validation: Make sure this is a valid pointer
		if (!isValidPointer(blockStart))
			throw std::invalid_argument("The provided pointer is not valid");

		// If the original author sucks at indirection and you can eliminate
		// some of the corner cases below, go nuts.

		// This will be our first free node
		if (firstFree == nullptr) {
			blockStart[num - 1].next = nullptr;
			firstFree = blockStart;
		}
		// The slot comes before our previously first free slot
		else if (blockStart < firstFree) {
			blockStart[num - 1].next = firstFree;
			firstFree = blockStart;
		}
		// Walk the free list, inserting this block in the correct place
		else {
			Slot** curr = &firstFree;
			while ((*curr)->next != nullptr && (*curr)->next < blockStart) {
				curr = &(*curr)->next;
			}
			if ((*curr)->next == blockStart) {
				throw std::logic_error("Double deallocate detected");
			}

			blockStart[num - 1].next = (*curr)->next;
			(*curr)->next = blockStart;
		}

		// Set up the block's "next free" pointers
		for (size_t i = 0; i < num - 1; ++i)
			blockStart[i].next = &blockStart[i + 1];

		numAllocated -= num;
	}


	/**
	 * \brief Allocates, constructs, and returns a single node from the pool
	 * \param args Variadic template magic that forwards whatever aruments that you would pass
	 *             to a constructor of T.
	 * \returns a pointer to a T, allocated from the pool then constructed.
	 * \throws std::bad_alloc if there is no room in the Pool
	 */
	template <typename... Args>
	T* construct(Args&&... args)
	{
		T* ret = allocate(1);
		::new (ret) T(std::forward<Args>(args)...);

		return ret;
	}

	/**
	 * \brief Allocates, constructs, and returns a single node from the pool
	 * \param args Variadic template magic that forwards whatever aruments that you would pass
	 *             to a constructor of T.
	 * \returns a pointer to a T, allocated from the pool then constructed,
	 *          or null if there is no room in the Pool
	 */
	template <typename... Args>
	T* tryConstruct(Args&&... args)
	{
		try {
			return construct(std::forward<Args>(args)...);
		}
		catch (const std::bad_alloc&) {
			return nullptr;
		}
	}

	/// Acts as the same manner as construct, but returns a std::unique_ptr
	/// that destroys the object automatically when the pointer falls out of scope
	template <typename... Args>
	unique_ptr constructUnique(Args&&... args)
	{
		return unique_ptr(construct(std::forward<Args>(args)...), [this](T* t) { destroy(t); });
	}

	/// Acts as the same manner as construct, but returns a std::shared_ptr
	/// that destroys the object automatically when all shared pointers to this object falls out of scope
	template <typename... Args>
	std::shared_ptr<T> constructShared(Args&&... args)
	{
		return std::shared_ptr<T>(construct(std::forward<Args>(args)...), [this](T* t) { destroy(t); });
	}


	/// Destroys then deallocates an object constructed from the pool using _construct_
	/// or _tryConstruct_
	void destroy(T* toRelease)
	{
		toRelease->~T(); // Call its destructor

		deallocate(toRelease, 1);
	}

	iterator begin() { return iterator(*this); }

	const_iterator begin() const { return const_iterator(*this); }

	const_iterator cbegin() const { return const_iterator(*this); }

	iterator end() { return iterator(buff + numSlots); }

	const_iterator end() const { return const_iterator(buff + numSlots); }

	const_iterator cend() const { return const_iterator(buff + numSlots); }

	// No copy or assign

	/// The copy constructor is deleted. A copy of a pool is useless
	/// since its already-allocated slots will have no users.
	Pool(const Pool&) = delete;

	/// The assignment operator is deleted. A copy of a pool is useless
	/// since its already-allocated slots will have no users.
	const Pool& operator=(const Pool&) = delete;

private:

	/// A slot in our pool.
	/// We use this union so that we can hold a pointer to the next free slot
	/// when the slot is not in use
	union Slot {
		T data; ///< The allocated data in the slot, or alternatively...
		Slot* next; ///< ...A pointer to the next free slot
	};

	/**
	 * \brief Gets the number of contiguous slots start
	 * \param s The first free slot in a possible group of contiguous slots
	 * \returns The number of contiguous slots, starting at the given slot,
	 *          and the next free pointer after this current slot,
	 *          or 0 and null if null is given as an input
	 * \warning This function assumes that the pointer it is passed is free.
	 */
	std::pair<size_t, Slot**> getContiguousCount(Slot* s) const
	{
		if (s == nullptr)
			return std::pair<size_t, Slot**>(0, nullptr);
		assert(isValidPointer(s));
		size_t contig = 1;
		while (s->next == s + 1) {
			++contig;
			++s;
		}

		return std::pair<size_t, Slot**>(contig, &s->next);
	}

	/// \brief Checks if a pointer is within the range of the buffer and is aligned.
	/// \warning This does not check if the pointer is free or used. That would take too much time.
	bool isValidPointer(Slot* s) const
	{
		if (s < buff || s >= buff + numSlots)
			return false; // The pointer is not inside our buffer

		const uintptr_t distance = (char*)s - (char*)buff;

		if (distance % sizeof(Slot) != 0)
			return false; // The pointer is not aligned

		return true;
	}

	Slot* buff; ///< The buffer for the entire pool
	Slot* firstFree; ///< A pointer to the first free slot in the pool
	size_t numSlots; ///< The total number of slots in the pool
	size_t numAllocated; ///< The number of allocated slots in the pool
};

/// An extremely simple allocator for a Pool of type T
/// which can be used by the standard library containers
template <typename T>
class PoolAllocator {

public:

	typedef T value_type;

	/// Constructor. Takes a reference to the pool from which to allocate
	PoolAllocator(Pool<T>& p) : pool(p) { }

	/// Calls _allocate_ on the allocator's pool.
	T* allocate(size_t num) { return pool.allocate(num); }

	/// Calls _deallocate_ on the allocator's pool.
	void deallocate(T* allocated, size_t n) { return pool.deallocate(allocated, n); }

	/// The pool to use.
	Pool<T>& pool;
};

/**
 * \brief A simple forward iterator that lets us iterate through a pool's used slots
 * \warning This iterator is invalidated if the pool is modified
 *
 * If you're perplexed by all the `std::remove_const<T>` nonsense,
 * it is so we can have const and non-const iterators.
 * In C++, the standard library containers offer both:
 *   - non-const iterators let you iterate through the collection
 *     and modify the items in the collection.
 *   - const iterators let you iterate through a collection,
 *     but do not let you modify its items.
 *
 * To get a const iterator, we `const T` from the pool as the template argument here.
 * For example, for a Pool of ints (`Pool<int>`), `cbegin` returns
 * a pool iterator of type `PoolIterator<const int>`.
 * However, you'll notice that we now need to strip the `const` off again when
 * referring to the pool's types. Hence `std::remove_const`.
 */

#pragma GCC diagnostic push
// Shut gcc up about std::iterator having a non-virtual destructor
#pragma GCC diagnostic ignored "-Weffc++"
template <typename T>
class PoolIterator : public std::iterator<std::forward_iterator_tag, T> {
#pragma GCC diagnostic pop

public:
	/// Iterators must be default-constructible
	PoolIterator() : current(nullptr), nextFree(nullptr) { }

	/// Default copy constructor - just copy the members
	PoolIterator(const PoolIterator&) = default;

	/// Creates an iterator that starts at the first used slot in a pool
	PoolIterator(const Pool<typename std::remove_const<T>::type>& pool) :
		current(pool.buff),
		nextFree(pool.firstFree)
	{
		// If our current slot is a free node, keep incrementing until it is not.
		while (current == nextFree) {
			++current;
			nextFree = nextFree->next;
		}
	}

	/// Creates an iterator for the Pool::end family of functions
	PoolIterator(typename Pool<typename std::remove_const<T>::type>::Slot* end) :
		current(end),
		nextFree(nullptr)
	{
	}

	// Common iterator operators.
	// Iterators act like pointers to their current item and can be dereferenced
	// as such.

	T& operator*() const { return *reinterpret_cast<T*>(current); }

	T* operator->() const { return reinterpret_cast<T*>(current); }

	/// Equality. Two iterators are true if they are pointing at the same item.
	bool operator==(const PoolIterator& o) const
	{
#ifndef NDEBUG
		// If the two are equal, nextFree should also be the same unless
		// someone modified the tree like they weren't supposed to.
		if (current == o.current)
			assert(nextFree == o.nextFree);
#endif
		return current == o.current;
	}

	/// Comparison operator, needed for all forward iterators
	bool operator<(const PoolIterator& o) const
	{
		assert(current != nullptr);
		assert(o.current != nullptr);
		return current < o.current;
	}

	/// Pre-increment
	PoolIterator& operator++()
	{
		// Increment the current pointer
		++current;

		// If our current slot is a free node, keep incrementing until it is not.
		while (current == nextFree) {
			++current;
			nextFree = nextFree->next;
		}

		return *this;
	}

	/// Post-increment
	PoolIterator operator++(int)
	{
		PoolIterator<T> ret(*this);
		operator++();
		return ret;
	}

	/// Increment by a given amount
	PoolIterator& operator+=(size_t by)
	{
		for (size_t i = 0; i < by; ++i)
			operator++();

		return *this;
	}

	/// Increment by a given amount
	PoolIterator operator+(size_t by)
	{
		PoolIterator<T> ret(*this);
		ret += by;
		return ret;
	}

private:
	// gcc tells me I need to use "typename". Huh. Okay.
	typename Pool<typename std::remove_const<T>::type>::Slot* current;
	typename Pool<typename std::remove_const<T>::type>::Slot* nextFree;
};
