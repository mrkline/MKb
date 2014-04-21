#ifndef __NETWORK_PORTABILITY_HPP__
#define __NETWORK_PORTABILITY_HPP__

// This file contains kludgy magic that lets us build for both Windows and
// Posix.
#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

namespace {

#define SHUT_RDWR SD_BOTH
#define SHUT_WR SD_SEND
#define SHUT_RD SD_RECEIVE

typedef SOCKET SockDesc;
const SockDesc invalidSocket = INVALID_SOCKET;
inline bool socketIsValid(SockDesc s) { return s != invalidSocket; }
inline int close(SockDesc s) { return closesocket(s); }

}

#else

#include <netinet/in.h>
#include <netdb.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

namespace {

typedef int SockDesc;
const SockDesc invalidSocket = -1;
inline bool socketIsValid(SockDesc s) { return s >= 0; }

}

#endif

#endif
