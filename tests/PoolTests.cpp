#include "PoolTests.hpp"

#include <vector>

#include "Test.hpp"
#include "Pool.hpp"

using namespace std;
using namespace Exceptions;
using namespace Testing;

namespace {

/// A dumb payload with which to test our Pool
class Payload {
public:

	Payload() : a(0), b(0) { }

	Payload(const Payload&) = default;

	Payload(int a, int b) : a(a), b(b) { }

	int a, b;
};

/// Test construction and destruction of the Pool
void instantiation()
{
	Pool<Payload> aPool(100);
}

/// Test construction and destruction of objects from the Pool
void construction()
{
	Pool<Payload> aPool(5);
	vector<Payload*> pointers;

	assert(aPool.size() == 0);

	// Allocate a bunch of objects
	for (size_t i = 0; i < aPool.max_size(); ++i) {
		pointers.emplace_back(aPool.construct(i, 42 + i));
		assert(aPool.size() == i + 1);
	}

	// Check that they were constructed as we expect
	for (size_t i = 0; i < pointers.size(); ++i) {
		Payload& p = *pointers[i];
		assert(p.a == (int)i);
		assert(p.b == 42 + (int)i);
	}

	// Check that we get nullptr back when we're out of space
	assertThrown<std::bad_alloc>([&] { aPool.construct(); });
	assert(aPool.tryConstruct() == nullptr);

	for (size_t i = 0; i < pointers.size(); ++i)
		aPool.destroy(pointers[i]);
}

/// Test the out-of-order destruction of objects from the Pool,
/// which should give us better covereage of Pool::destroy
/// than the construction test.
void destroy()
{
	Pool<Payload> aPool(5);
	vector<Payload*> pointers;

	// Allocate a bunch of objects
	for (size_t i = 0; i < aPool.max_size(); ++i) {
		pointers.emplace_back(aPool.construct(i, 42 + i));
	}

	// Check that they were constructed as we expect
	for (size_t i = 0; i < pointers.size(); ++i) {
		Payload& p = *pointers[i];
		assert(p.a == (int)i);
		assert(p.b == 42 + (int)i);
	}

	// Release them
	assert(aPool.size() == 5);
	aPool.destroy(pointers[0]);
	assert(aPool.size() == 4);
	aPool.destroy(pointers[4]);
	assert(aPool.size() == 3);
	aPool.destroy(pointers[1]);
	assert(aPool.size() == 2);
	aPool.destroy(pointers[3]);
	assert(aPool.size() == 1);
	aPool.destroy(pointers[2]);
	assert(aPool.size() == 0);
}

/// Test out Pool::allocate and Pool::deallocate
void allocate()
{
	Pool<Payload> aPool(10);

	// We have a lot of assertions here,
	// but hell, these are tests after all.

	Payload* first = aPool.allocate(3);
	assert(aPool.size() == 3);
	Payload* second = aPool.allocate(5);
	assert(aPool.size() == 8);
	Payload* third = aPool.allocate(2);
	assert(aPool.full());

	aPool.deallocate(first, 3);
	assert(aPool.size() == 7);
	aPool.deallocate(third, 2);
	assert(aPool.size() == 5);

	// Test that best-fit is working
	Payload* another = aPool.allocate(2);
	assert(aPool.size() == 7);
	// It should be put in the slot after second
	assert(another > second);
	assert(another == third);

	// Fit two allocations where we had our first allocation
	first = aPool.allocate(1);
	assert(aPool.size() == 8);
	Payload* secondFirst = aPool.allocate(2); // I suck at names
	assert(aPool.full());

	// We should be out
	assertThrown<std::bad_alloc>([&] { aPool.allocate(1); });

	// Deallocate two of the same size and allocate again.
	// Our algorithm should choose the first one
	aPool.deallocate(another, 2);
	assert(aPool.size() == 8);
	aPool.deallocate(secondFirst, 2);
	assert(aPool.size() == 6);

	another = aPool.allocate(2);
	assert(aPool.size() == 8);

	assert(another == secondFirst);

	// We should have two slots left
	assert(aPool.remaining() == 2);

	aPool.deallocate(first, 1);
	aPool.deallocate(second, 5);
	aPool.deallocate(another, 2);
}

/// Test using a pool and its allocator with a standard library container
void forSTL()
{
	Pool<Payload> aPool(20);
	vector<Payload, PoolAllocator<Payload>> vec1(10, aPool.getAllocator());
	vector<Payload, PoolAllocator<Payload>> vec2(10, aPool.getAllocator());
	// We should be out of memory in the pool now
	assertThrown<std::bad_alloc>([&] { vector<Payload, PoolAllocator<Payload>> vec3(1, aPool.getAllocator()); });
}

void iteration()
{
	Pool<Payload> aPool(5);
	vector<Payload*> pointers;

	// Allocate a bunch of objects
	for (size_t i = 0; i < aPool.max_size(); ++i) {
		pointers.emplace_back(aPool.construct(i, 42 + i));
	}

	// Check that they were constructed as we expect
	for (size_t i = 0; i < pointers.size(); ++i) {
		Payload& p = *pointers[i];
		assert(p.a == (int)i);
		assert(p.b == 42 + (int)i);
	}

	auto testIterator = [&](const std::initializer_list<int>& l) {
		Pool<Payload>::const_iterator it = aPool.cbegin();
		for (int n : l) {
			assert(it < aPool.cend());
			// Test both operators
			assert(n == it->a);
			assert(n == (*it).a);
			++it;
		}
		// Make sure our iterators reach the end properly
		assert(it == aPool.cend());
		// Test that these have been built correctly;
		// their mere invocation will bomb if there are template issues.
		assert(aPool.begin() + l.size() ==aPool.end());
	};

	// Release them
	assert(aPool.size() == 5);
	aPool.destroy(pointers[0]);
	assert(aPool.size() == 4);
	testIterator({1, 2, 3, 4});
	aPool.destroy(pointers[4]);
	assert(aPool.size() == 3);
	testIterator({1, 2, 3});
	aPool.destroy(pointers[1]);
	assert(aPool.size() == 2);
	testIterator({2, 3});
	aPool.destroy(pointers[3]);
	assert(aPool.size() == 1);
	testIterator({2});
	aPool.destroy(pointers[2]);
	assert(aPool.size() == 0);
}

} // end namespace anonymous

void Testing::runPoolTests()
{
	beginUnit("Pool");
	test("Instantiation", &instantiation);
	test("Construction", &construction);
	test("Destruction", &destroy);
	test("Allocate", &allocate);
	test("As allocator for STL", &forSTL);
	test("Iteration", &iteration);
}
