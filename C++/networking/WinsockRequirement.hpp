#pragma once

#include <WinSock2.h>

#include "Exceptions.hpp"

/**
\brief Wraps the startups and shutdown functions of Winsock.
	Classes which use Winsock should have a member instance of this class

Winsock will be initialized
*/
class WinsockRequirement
{
public:
	WinsockRequirement()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
			throw NetworkException("Winsock failed during initialization", __FUNCTION__);
	}

	~WinsockRequirement()
	{
		WSACleanup();
	}

	static const WSADATA& GetWinsockData() { return wsd; }

private:
	/// Made static so that an instance isn't generated for each instance
	/// of the object
	static WSADATA wsd;
};
