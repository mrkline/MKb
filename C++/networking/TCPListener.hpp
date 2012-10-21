#pragma once

#include "Portability.hpp"
#include "IP.hpp"

#ifdef _WIN32
#  include "WinsockRequirement.hpp"
#endif


class TCPConnection;

/*!
\brief Serves as a TCP host that listens for and accepts incoming connections
*/
class TCPListener
{
public:
	/*!
	\brief Initializes the listener to a given port
	\param port The port to listen in on
	\throws NetworkException if Winsock's getaddrinfo fails
	*/
	TCPListener(int port);

	~TCPListener();

	/*!
	\brief Makes the listener begin listening for a maximum number of incoming
	       connection requests
	\param maxRequests The maximum number of connection requests this listener
	       will place on backlog to accept. The default is the maximum number
	       of connections.
	\throws InvalidOperationException if the listener has already been started
	\throws NetworkException if Winsock's socket, bind, or listen fail
	*/
	void start(int maxRequests = SOMAXCONN);

	/*!
	\brief Stops the listener so that it no longer listens for incoming
	       connection requests
	\throws InvalidOperationException if the listener has not been started
	\throws NetworkException if Winsock's shutdown fails
	*/
	void stop();

	//! Returns true if the listener has been successfully started
	bool isRunning() { return listenSock != INVALID_SOCKET; }

	/*!
	\brief Accepts an incoming connection request
	\warning The code calling this method is responsible for deleting
	         the returned connection
	\returns The accepted TCP connection
	\throws InvalidOperationException if the listener has not been started
	\throws NetworkException if Winsock's accept fails
	*/
	TCPConnection* accept();

private:
	// Disallow copy and assign
	void operator=(const TCPListener&) {}
	TCPListener(const TCPListener&) {}

#ifdef _WIN32
	WinsockRequirement ws;
#endif
	SockDesc listenSock;
	addrinfo* ai;
};
