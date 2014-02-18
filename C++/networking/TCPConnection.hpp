#pragma once

#include "Socket.hpp"
#include "IP.hpp"

#ifdef _WIN32
#  include "WinsockRequirement.hpp"
#endif

/**
\brief A Transmission Control Protocol connection for
		sending and receiving data over a network
\todo Async?
\todo DNS?

TCP is a reliable connection-based protocol which guarantees that data will be
received without transmission errors and in the order that it was sent.
Data is sent as a continuous stream.
*/

class TCPConnection : public Socket
{
public:

	/// Creates a disconnected TCP connection
	TCPConnection();

	~TCPConnection();

	/**
	\brief Creates a TCP connection from an existing connected socket
	\param connSock The connected socket to create the object from
	\throws ArgumentException if connSock is an invalid socket

	This should not be used by client code and is intended to be used
	by TCPListener to accept connections.
	*/
	TCPConnection(SockDesc connSock);

	/**
	\brief Establishes a TCP connection between the client and a server
	       at the given end point
	\param server The IP and port of the TCP server to connect to
	\throws InvalidOperationException if the connection is already established
	\throws NetworkException if getaddrinfo, socket, or connect fail
	*/
	void connect(const IPEndPoint& server);

	/**
	\brief Closes the TCP connection
	\throws InvalidOperationException if the connection isn't established
	*/
	void disconnect();

	/**
	\brief Sends data over the established connection
	\param data The buffer containing data to send
	\param dataLen The amount (in bytes) of data to send
	\returns The amount (in bytes) of data sent
	\throws InvalidOperationException if the connection is not established,
	        sending has been shut down, or the other party has closed the
	        connection
	\throws NetworkException if send fails

	This operation is blocking.
	*/
	size_t send(const char* data, size_t dataLen);

	/**
	\brief Receives data over the established connection
	\param recvBuff The buffer to copy the incoming datagram into
	\param recvBuffLen The amount (in bytes) of recvBuff that Receive
	       is allowed to copy into
	\returns The number of bytes received into recvBuff,
			or 0 if the connection was closed gracefully
	\throws InvalidOperationException if the connection is not established,
	        receiving has been shut down, or the other party has closed the
	        connection
	\throws NetworkException if recv fails

	This operation is blocking.
	*/
	size_t receive(char* recvBuff, size_t recvBuffLen);

	/**
	\brief Shuts down sending operations on the connection
	\throws InvalidOperationException if sending has already been shut down
	\throws NetworkException if shutdown fails
	*/
	void shutDownSending();

	/**
	\brief Shuts down receiving operations on the connection
	\throws InvalidOperationException if receiving has already been shut down
	\throws NetworkException if shutdown fails
	*/
	void shutDownReceiving();

	// Disallow copy and assign
	void operator=(const TCPConnection&) = delete;
	TCPConnection(const TCPConnection&) = delete;

private:

#ifdef _WIN32
	WinsockRequirement ws;
#endif
	bool canSend;
	bool canReceive;
	bool closedByOtherParty;
};
