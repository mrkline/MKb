#include "TCPConnection.hpp"

#include <cstring>
#include <sstream>

#include "Exceptions.hpp"

using namespace std;
using namespace Exceptions;

TCPConnection::TCPConnection() :
	Socket()
{
}

TCPConnection::TCPConnection(SockDesc connSock) :
	Socket(connSock),
	canSend(true),
	canReceive(true),
	closedByOtherParty(false)
{
	if (!socketIsValid(connSock))
		throw ArgumentException("The provided socket is invalid.", __FUNCTION__);
}

TCPConnection::~TCPConnection()
{
	if (socketIsValid(sock)) {
		close(sock);
	}
}

void TCPConnection::connect(const IPEndPoint& server)
{
	if (socketIsValid(sock))
		throw InvalidOperationException("A connection has already been made.", __FUNCTION__);


	addrinfo *result = nullptr;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	stringstream ps;
	ps << server.Port;

	if (getaddrinfo(server.Address.getAsString().c_str(), ps.str().c_str(), &hints, &result) != 0)
		throw NetworkException( "getaddrinfo failed while trying to set up a connection.", __FUNCTION__);

	// Iterate through the given addresses, trying to connect
	for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (!socketIsValid(sock)) {
			/// \todo free result?
			throw NetworkException("Socket creation failed.", __FUNCTION__);
		}

		if (0 > ::connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen)) {
			close(sock);
			sock = invalidSocket;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (!socketIsValid(sock))
		throw NetworkException( "A connection to the server could not be established.", __FUNCTION__);

	canSend = true;
	canReceive = true;
	closedByOtherParty = false;
}

void TCPConnection::disconnect()
{
	if (!socketIsValid(sock)) {
		throw InvalidOperationException(
			"The connection cannot be disconnected since it is not connected in the first place.",
			__FUNCTION__);
	}

	if (close(sock) != 0)
		throw NetworkException("Socket closure failed during disconnection.", __FUNCTION__);

	sock = invalidSocket;
	canSend = false;
	canReceive = false;
}

size_t TCPConnection::send(const char* data, size_t dataLen )
{
	if (!socketIsValid(sock)) {
		throw InvalidOperationException("The connection cannot send data since" " no connection is established",
		                                __FUNCTION__);
	}

	if (!canSend)
		throw InvalidOperationException( "Sending has been shut down on this connection.", __FUNCTION__);

	if (closedByOtherParty)
		throw InvalidOperationException( "The other party has closed the connection", __FUNCTION__);

	int ret = ::send(sock, data, dataLen, 0);

	if (0 > ret)
		throw NetworkException("Sending over the connection failed", __FUNCTION__);

	return ret;
}

size_t TCPConnection::receive( char* recvBuff, size_t recvBuffLen )
{
	if (!socketIsValid(sock)) {
		throw InvalidOperationException("The connection cannot receive data since no connection is established",
		                                __FUNCTION__);
	}

	if (!canReceive)
		throw InvalidOperationException( "Receiving has been shut down on this connection.", __FUNCTION__);

	if (closedByOtherParty)
		throw InvalidOperationException( "The other party has closed the connection", __FUNCTION__);

	int ret = recv(sock, recvBuff, recvBuffLen, 0);

	if (0 > ret)
		throw NetworkException("Receiving over the connection failed.", __FUNCTION__);

	if (ret == 0)
		closedByOtherParty = true;

	return ret;
}

void TCPConnection::shutDownSending()
{
	if (!canSend)
		throw InvalidOperationException( "Sending has already been shut down on this connection.", __FUNCTION__);

	if (shutdown(sock, SHUT_WR) != 0)
		throw NetworkException("Sending shutdown failed.", __FUNCTION__);

	canSend = false;
}

void TCPConnection::shutDownReceiving()
{
	if (!canReceive)
		throw InvalidOperationException( "Receiving has already been shut down on this connection.", __FUNCTION__);

	if (shutdown(sock, SHUT_RD) != 0)
		throw NetworkException("Receiving shutdown failed.", __FUNCTION__);

	canReceive = false;
}
