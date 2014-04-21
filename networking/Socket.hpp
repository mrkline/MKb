#pragma once

#include "Portability.hpp"

/// A base class for networking sockets (excluding TCP listener sockets)
class Socket
{
public:
	Socket(SockDesc s = invalidSocket) : sock(s) { }
	SockDesc getSocket() { return sock; }

protected:
	SockDesc sock;
};
