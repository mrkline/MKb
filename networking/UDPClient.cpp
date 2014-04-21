#include "UDPClient.hpp"

#include <cstring>

using namespace Exceptions;

UDPClient::UDPClient()
	: bound(false)
{
	init();
}

UDPClient::UDPClient(int port)
{
	init();
	bind(port);
}

UDPClient::~UDPClient()
{
	close(sock);
}

void UDPClient::bind( int port )
{
	struct sockaddr_in ai;

	memset(&ai, 0, sizeof(ai));
	ai.sin_family = AF_INET;
	ai.sin_addr.s_addr = htonl(INADDR_ANY);
	ai.sin_port = htons(port);

	if (::bind(sock, (struct sockaddr*)&ai, sizeof(ai)) != 0)
		throw NetworkException( "UDP socket could not be bound to the given port", __FUNCTION__);

	bound = true;
}

void UDPClient::setDefaultDestination( const IPEndPoint& destination )
{
	defaultDest.reset(new sockaddr_in);

	memset(defaultDest.get(), 0, sizeof(*defaultDest));
	defaultDest->sin_family = AF_INET;
	defaultDest->sin_port = htons(destination.Port);
	defaultDest->sin_addr.s_addr = destination.Address.getAsBinary(IP::BO_NET);
}

size_t UDPClient::send(const char* data, size_t dataLen )
{
	if (defaultDest == nullptr)
	{
		throw InvalidOperationException("A default destination must be set before sending"
		                                " without a specified destination",
		                                __FUNCTION__);
	}

	ssize_t ret = sendto(sock, data, dataLen, 0, (sockaddr*)defaultDest.get(), sizeof(*defaultDest));

	if (0 > ret)
		throw NetworkException("Sending failed.", __FUNCTION__);

	return ret;
}

size_t UDPClient::send(const char* data, size_t dataLen,
                       const IPEndPoint& destination )
{
	sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(destination.Port);
	dest.sin_addr.s_addr = destination.Address.getAsBinary(IP::BO_NET);

	ssize_t ret = sendto(sock, data, dataLen, 0, (sockaddr*)&dest, sizeof(dest));

	if (0 > ret)
		throw NetworkException("Sending failed.", __FUNCTION__);

	return ret;
}

size_t UDPClient::receive( char* recvBuff, size_t recvBuffLen, IP* from /*= nullptr*/ )
{
	if (!bound)
		throw InvalidOperationException( "To receive from a UDP client, it must first be bound.", __FUNCTION__);

	int ret;

#ifdef _WIN32
	// Clear the LastError flag so we can check it after we run recvfrom
	WSASetLastError(0);
#else
	errno = 0;
#endif

	if (from == nullptr) {
		ret = recvfrom(sock, recvBuff, recvBuffLen, 0, nullptr, nullptr);
	}
	else {
		sockaddr_in fa;
		socklen_t fasz = sizeof(fa);
		ret = recvfrom(sock, recvBuff, recvBuffLen, 0, (sockaddr*)&fa, &fasz);
		*from = fa.sin_addr.s_addr;
	}

#ifdef _WIN32
	// Check if we didn't have a large enough buffer
	if (WSAGetLastError() == WSAEMSGSIZE) {
		throw InsufficientBufferException("In UDP, the receiving buffer must be as large as the incoming datagram",
		                                  __FUNCTION__);
	}
#else
	if (errno == ENOBUFS)
		throw InsufficientBufferException("The buffer may have been smaller than the incoming datagram", __FUNCTION__);
#endif

	if (0 > ret)
		throw NetworkException("Receiving failed", __FUNCTION__);

	return ret;
}

void UDPClient::init()
{
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (!socketIsValid(sock))
		throw NetworkException("UDP socket could not be created", __FUNCTION__);

	defaultDest = nullptr;
}
