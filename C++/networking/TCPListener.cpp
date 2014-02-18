#include "TCPListener.hpp"

#include <cstring>
#include <sstream>

#include "Exceptions.hpp"
#include "TCPConnection.hpp"

using namespace Exceptions;

TCPListener::TCPListener(int port)
		: listenSock(invalidSocket)
{
	std::stringstream ps;
	ps << port;

	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	if (getaddrinfo(nullptr, ps.str().c_str(), &hints, &ai) != 0)
		throw NetworkException("getaddrinfo failed.", __FUNCTION__);
}

TCPListener::~TCPListener()
{
	freeaddrinfo(ai);

	if (socketIsValid(listenSock))
		close(listenSock);
}

void TCPListener::start( int maxRequests /*= SOMAXCONN*/ )
{
	if (socketIsValid(listenSock))
		throw InvalidOperationException( "The listener has already been started.", __FUNCTION__);

	listenSock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

	if (!socketIsValid(listenSock))
		throw NetworkException("Socket creation failed.", __FUNCTION__);

	if (0 > bind(listenSock, ai->ai_addr, (int)ai->ai_addrlen)) {
		close(listenSock);
		throw NetworkException("A socket could not be bound to its given port.", __FUNCTION__);
	}

	if (0 > listen(listenSock, maxRequests))
		throw NetworkException("A socket failed on listening for connection requests.", __FUNCTION__);
}

void TCPListener::stop()
{
	if (!socketIsValid(listenSock))
		throw InvalidOperationException("The listener has not been started, so it cannot be stopped.", __FUNCTION__);

	if (close(listenSock) != 0)
		throw NetworkException("The listener failed while stopping itself", __FUNCTION__);
}

std::unique_ptr<TCPConnection> TCPListener::accept()
{
	if (!socketIsValid(listenSock))
		throw InvalidOperationException("The listener must be started before it can accept connections.",
		                                __FUNCTION__);

	SockDesc connSock = ::accept(listenSock, NULL, NULL);

	if (!socketIsValid(connSock))
		throw NetworkException( "The listener failed while accepting a connection.", __FUNCTION__);

	return std::unique_ptr<TCPConnection>(new TCPConnection(connSock));
}
