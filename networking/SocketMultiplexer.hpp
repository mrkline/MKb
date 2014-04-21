#pragma once

#include <list>

#include "Socket.hpp"

/// This class is essentially a C++ wrapper of select, and is used to mux
/// multiple networking sockets
class SocketMultiplexer
{
public:
	// It's a linked list of socket pointers
	typedef std::list<Socket*> SocketList;

	/// Empties all sets
	SocketMultiplexer();
	
	/// Sets the set of sockets which will be queried for receive operations
	void setReadSet(const SocketList& sockets);

	/// Sets a single socket to be queried for receive operations
	void setReadSet(Socket* socket);

	/// Sets the set of sockets which will be queried for send operations
	void setWriteSet(const SocketList& sockets);

	/// Sets a single socket to be queried for send operations
	void setWriteSet(Socket* socket);

	/**
	 * \brief Gets the set of sockets for receive operations
	 *
	 * Before select is called, this function just returns the same list set by
	 * setReadSet. After select is called, this contains the sockets out of the
	 * original set that can receive data without blocking.
	 */
	const SocketList& getReadSet() const { return readList; }

	/**
	 * \brief Gets the set of sockets for send operations
	 *
	 * Before select is called, this function just returns the same list set by
	 * setWriteSet. After select is called, this function contains the sockets
	 * out of the original set that can send data without blocking.
	 */
	const SocketList& getWriteSet() const { return writeList; }

	/// After select is called, this function returns a list of any bad sockets
	/// from the read and write sets.
	const SocketList& getBadSet() const { return exceptList; }

	/// Empties the set of sockets for receive operations
	void clearReadSet();

	/// Empties the set of sockets for send operations
	void clearWriteSet();

	/**
	 * \brief Accepts a sets of sockets intended for sending and receiving and
	 *        returns the subset of those sockets which can do so without
	 *        blocking
	 *
	 * \param timeout a timeval struct that indicates the amount of time this
	 *        function should block before it times out
	 *
	 * \returns the number of sockets contained in the sets. This may be zero if
	 *          the function timed out before sockets were available for
	 *          non-blocking I/O
	 *
	 * Before select is called, setReadSet and setWriteSet should be called with
	 * lists of sockets on which the user intends to receive and send data,
	 * respectively. This function should then be called. After the successful
	 * return of this function, the read and write sets are stripped of sockets
	 * that cannot complete the desired operation without blocking.
	 */
	int select(struct timeval* timeout);

private:
	fd_set read;
	fd_set write;
	fd_set except;

	SocketList readList;
	SocketList writeList;
	SocketList exceptList;
};
