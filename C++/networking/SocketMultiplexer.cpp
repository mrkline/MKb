#include "SocketMultiplexer.hpp"

#include "Exceptions.hpp"

SocketMultiplexer::SocketMultiplexer()
{
	FD_ZERO(&read);
	FD_ZERO(&write);
	FD_ZERO(&except);
}

void SocketMultiplexer::setReadSet(const std::list<Socket*>& sockets)
{
	FD_ZERO(&read);
	for (Socket* sock : sockets)
		FD_SET(sock->getSocket(), &read);

	readList = sockets;
}

void SocketMultiplexer::setReadSet(Socket* socket)
{
	FD_ZERO(&read);
	FD_SET(socket->getSocket(), &read);
	readList.clear();
	readList.push_back(socket);
}

void SocketMultiplexer::setWriteSet(const std::list<Socket*>& sockets)
{
	FD_ZERO(&write);
	for (Socket* sock : sockets)
		FD_SET(sock->getSocket(), &write);

	writeList = sockets;
}

void SocketMultiplexer::setWriteSet(Socket* socket)
{
	FD_ZERO(&write);
	FD_SET(socket->getSocket(), &write);
	writeList.clear();
	writeList.push_back(socket);
}

void SocketMultiplexer::clearReadSet()
{
	FD_ZERO(&read);
	readList.clear();
}

void SocketMultiplexer::clearWriteSet()
{
	FD_ZERO(&write);
	writeList.clear();
}

int SocketMultiplexer::select(struct timeval* timeout)
{
	// Clear the previous exception set
	FD_ZERO(&except);
	exceptList.clear();

	// According to documentation, the first argument to select is
	// "the highest-numbered file descriptor in any of hte three sets, plus 1"
	SockDesc highest = -1;
	
	// Fill the exception set with sockets from the read set
	for (Socket* sock : readList) {
		SockDesc curr = sock->getSocket();
		FD_SET(curr, &except);
		exceptList.push_back(sock);
		if (curr > highest)
			highest = curr;
	}

	// Fill the exception set with sockets from the write set
	for (Socket* sock : writeList) {
		SockDesc curr = sock->getSocket();
		// Only add the socket if it wasn't already in the read set
		if (!FD_ISSET(curr, &except)) {
			FD_SET(curr, &except);
			exceptList.push_back(sock);
			if (curr > highest)
				highest = curr;
		}
	}

	int ret = ::select(highest, &read, &write, &except, timeout);

	if (ret < 0)
		throw Exceptions::NetworkException("select failed", __FUNCTION__);

	// Filter the linked lists for items that should still be in them
	for (auto it = readList.begin(); it != readList.end(); ++it) {
		if (!FD_ISSET((*it)->getSocket(), &read))
			readList.erase(it--);
	}
	for (auto it = writeList.begin(); it != writeList.end(); ++it) {
		if (!FD_ISSET((*it)->getSocket(), &write))
			writeList.erase(it--);
	}
	for (auto it = exceptList.begin(); it != exceptList.end(); ++it) {
		if (!FD_ISSET((*it)->getSocket(), &except))
			exceptList.erase(it--);
	}

	return ret;
}
