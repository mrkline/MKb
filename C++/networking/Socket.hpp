#pragma once

#include "Portability.hpp"

//! A base class for networking sockets (excluding TCP listener sockets)
class Socket
{
public:
	Socket() : sock(INVALID_SOCKET) { }
	SockDesc getSocket() { return sock; }

protected:
	SockDesc sock;
};
