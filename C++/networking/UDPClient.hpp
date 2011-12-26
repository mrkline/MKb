#pragma once

#include "IP.hpp"
#include "WinsockRequirement.hpp"
#include "Exceptions.hpp"

/*!
\brief A User Datagram Protocol client for
		sending and receiving data over a network
\todo Async?
\todo Multicast?
\todo DNS?

In contrast to TCP, UDP is a connectionless protocol which provides no guarantees
to reliability, ordering, or data integrity. Data is sent in packets known as datagrams.
*/
class UDPClient
{
public:
	class InsufficientBufferException : public NetworkException
	{
	public:
		InsufficientBufferException(const char* exceptionMessage,
		                            const char* callingFunctionName = nullptr)
				: NetworkException(exceptionMessage, callingFunctionName)
		{ }
	};

	//! Initializes the UDP client
	UDPClient();

	/*!
	\brief Initializes the UDP client and binds it to a given port
	\param port The port to bind the UDP socket to
	*/
	explicit UDPClient(int port);

	~UDPClient();

	/*!
	\brief Binds the UDP socket to a given port so it can receive data
	\param port The port to bind to

	This must be done before Receive is called
	*/
	void Bind(int port);

	/*!
	\brief Sets a default destination for the Send() overload with no specified destination
	\param destination The destination IP and port to send to
	*/
	void SetDefaultDestination(const IPEndPoint& destination);

	/*!
	\brief Sends data to the default destination
	\see SetDefaultDestination
	\param data The buffer containing data to send
	\param dataLen The amount (in bytes) of data to send
	\returns The amount (in bytes) of data sent

	This operation is blocking.
	*/
	size_t Send(const char* data, size_t dataLen);

	/*!
	\brief Sends data to a specified end point
	\param data The buffer containing data to send
	\param dataLen The amount (in bytes) of data to send
	\param destination The address and port to send the data to
	\returns The amount (in bytes) of data sent

	This operation is blocking.
	*/
	size_t Send(const char* data, size_t dataLen, const IPEndPoint& destination);

	/*!
	\brief Receives data, optionally providing the sender's IP and port
	\param recvBuff The buffer to copy the incoming datagram into
	\param recvBuffLen The amount (in bytes) of recvBuff that Receive is allowed to copy into
	\param from Optional. Upon completion, this will contain the end point the datagram originated from
	\returns The number of bytes received into recvBuff

	This operation is blocking.
	*/
	size_t Receive(char* recvBuff, size_t recvBuffLen, IP* from = nullptr);

	/*!
	\brief Shuts down sending operations on the connection
	\throws InvalidOperationException if sending has already been shut down
	\throws NetworkException if Winsock's shutdown fails
	*/
	void ShutDownSending();

	/*!
	\brief Shuts down receiving operations on the connection
	\throws InvalidOperationException if receiving has already been shut down
	\throws NetworkException if Winsock's shutdown fails
	*/
	void ShutDownReceiving();

private:
	WinsockRequirement ws;
	SOCKET sock;
	sockaddr_in* defaultDest;
	bool bound;
	bool canSend;
	bool canReceive;

	// Does common work shared by both constructors
	void Init();
};
