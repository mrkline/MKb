#pragma once

#include <memory>

#include "Socket.hpp"
#include "IP.hpp"
#include "Exceptions.hpp"

#ifdef _WIN32
#  include "WinsockRequirement.hpp"
#endif

/**
\brief A User Datagram Protocol client for
		sending and receiving data over a network
\todo Async?
\todo Multicast?
\todo DNS?

In contrast to TCP, UDP is a connectionless protocol which provides no
guarantees to reliability, ordering, or data integrity. Data is sent in packets
known as datagrams.
*/
class UDPClient : public Socket
{
public:
	class InsufficientBufferException : public Exceptions::NetworkException
	{
	/// \todo Move me into Exceptions.h
	public:
		InsufficientBufferException(const char* exceptionMessage, const char* callingFunctionName = nullptr)
				: NetworkException(exceptionMessage, callingFunctionName)
		{ }
	};

	/// Initializes the UDP client
	UDPClient();

	/**
	\brief Initializes the UDP client and binds it to a given port
	\param port The port to bind the UDP socket to
	*/
	explicit UDPClient(int port);

	~UDPClient();

	/**
	\brief Binds the UDP socket to a given port so it can receive data
	\param port The port to bind to

	This must be done before Receive is called
	*/
	void bind(int port);

	/**
	\brief Sets a default destination for the Send() overload with
	       no specified destination
	\param destination The destination IP and port to send to
	*/
	void setDefaultDestination(const IPEndPoint& destination);

	/**
	\brief Sends data to the default destination
	\see SetDefaultDestination
	\param data The buffer containing data to send
	\param dataLen The amount (in bytes) of data to send
	\returns The amount (in bytes) of data sent

	This operation is blocking.
	*/
	size_t send(const char* data, size_t dataLen);

	/**
	\brief Sends data to a specified end point
	\param data The buffer containing data to send
	\param dataLen The amount (in bytes) of data to send
	\param destination The address and port to send the data to
	\returns The amount (in bytes) of data sent

	This operation is blocking.
	*/
	size_t send(const char* data, size_t dataLen, const IPEndPoint& destination);

	/**
	\brief Receives data, optionally providing the sender's IP and port
	\param recvBuff The buffer to copy the incoming datagram into
	\param recvBuffLen The amount (in bytes) of recvBuff that Receive
	       is allowed to copy into
	\param from Optional. Upon completion, this will contain the end point
	       the datagram originated from
	\returns The number of bytes received into recvBuff

	This operation is blocking.
	*/
	size_t receive(char* recvBuff, size_t recvBuffLen, IP* from = nullptr);

private:
#ifdef _WIN32
	WinsockRequirement ws;
#endif
	std::unique_ptr<sockaddr_in> defaultDest;
	bool bound;

	// Does common work shared by both constructors
	void init();
};
