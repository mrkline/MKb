#include "UDPClient.h"

UDPClient::UDPClient()
		: bound(false), canSend(true), canReceive(true)
{
	Init();
}

UDPClient::UDPClient(int port)
		: canSend(true), canReceive(true)
{
	Init();
	Bind(port);
}

UDPClient::~UDPClient()
{
	shutdown(sock, SD_BOTH);
	closesocket(sock);

	if (defaultDest != nullptr)
		delete defaultDest;
}

void UDPClient::Bind( int port )
{
	SOCKADDR_IN ai;

	memset(&ai, 0, sizeof(ai));
	ai.sin_family = AF_INET;
	ai.sin_addr.s_addr = htonl(INADDR_ANY);
	ai.sin_port = htons(port);

	if (bind(sock, (sockaddr*)&ai, sizeof(ai)) != 0)
		throw NetworkException("UDP socket could not be bound to the given port", __FUNCTION__);

	bound = true;
}

void UDPClient::SetDefaultDestination( const IPEndPoint& destination )
{
	defaultDest = new sockaddr_in;

	memset(defaultDest, 0, sizeof(*defaultDest));
	defaultDest->sin_family = AF_INET;
	defaultDest->sin_port = htons(destination.Port);
	defaultDest->sin_addr.s_addr = destination.Address.GetAsBinary();
}

size_t UDPClient::Send(const char* data, size_t dataLen )
{
	if (defaultDest == nullptr)
	{
		throw InvalidOperationException(
		    "A default destination must be set before sending without a specified destination",
		    __FUNCTION__);
	}

	if (!canSend)
		throw InvalidOperationException("Sending has been shut down on this connection.", __FUNCTION__);

	int ret = sendto(sock, data, dataLen, 0, (sockaddr*)defaultDest, sizeof(*defaultDest));

	if (ret == SOCKET_ERROR)
	{
		throw NetworkException("Sending failed.", __FUNCTION__);
	}

	return ret;
}

size_t UDPClient::Send(const char* data, size_t dataLen, const IPEndPoint& destination )
{
	if (!canSend)
		throw InvalidOperationException("Sending has been shut down on this connection.", __FUNCTION__);

	sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(destination.Port);
	dest.sin_addr.s_addr = destination.Address.GetAsBinary();

	int ret = sendto(sock, data, dataLen, 0, (sockaddr*)&dest, sizeof(dest));

	if (ret == SOCKET_ERROR)
	{
		throw NetworkException("Sending failed.", __FUNCTION__);
	}

	return ret;
}

size_t UDPClient::Receive( char* recvBuff, size_t recvBuffLen, IP* from /*= nullptr*/ )
{
	if (!bound)
	{
		throw InvalidOperationException("To receive from a UDP client, it must first be bound.",
		                                __FUNCTION__);
	}

	if (!canReceive)
		throw InvalidOperationException("Receiving has been shut down on this connection.", __FUNCTION__);

	int ret;

	// Clear the LastError flag so we can check it after we run recvfrom
	WSASetLastError(0);

	if (from == nullptr)
	{
		ret = recvfrom(sock, recvBuff, recvBuffLen, 0, nullptr, nullptr);
	}
	else
	{
		sockaddr_in fa;
		int fasz = sizeof(fa);
		ret = recvfrom(sock, recvBuff, recvBuffLen, 0, (sockaddr*)&fa, &fasz);
		*from = fa.sin_addr.s_addr;
	}

	// \todo Does recvfrom return an error in this case?

	// Check if we didn't have a large enough buffer
	if (WSAGetLastError() == WSAEMSGSIZE)
	{
		throw InsufficientBufferException(
		    "In UDP, the receiving buffer must be as large as the incoming datagram",
		    __FUNCTION__);
	}

	if (ret == SOCKET_ERROR)
		throw NetworkException("Receiving failed", __FUNCTION__);

	return ret;
}

void UDPClient::ShutDownSending()
{
	if (!canSend)
	{
		throw InvalidOperationException("Sending has already been shut down on this connection.",
		                                __FUNCTION__);
	}

	if (shutdown(sock, SD_SEND) != 0)
		throw NetworkException("Sending shutdown failed.", __FUNCTION__);

	canSend = false;
}

void UDPClient::ShutDownReceiving()
{
	if (!canReceive)
	{
		throw InvalidOperationException("Receiving has already been shut down on this connection.",
		                                __FUNCTION__);
	}

	if (shutdown(sock, SD_RECEIVE) != 0)
		throw NetworkException("Receiving shutdown failed.", __FUNCTION__);

	canReceive = false;
}

void UDPClient::Init()
{
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET)
		throw NetworkException("UDP socket could not be created", __FUNCTION__);

	defaultDest = nullptr;
}
