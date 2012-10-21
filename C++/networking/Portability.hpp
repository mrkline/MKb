// This file contains kludgy magic that lets us build for both Windows and
// Posix.

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  define SockDesc                SOCKET
#  define SOCKET_IS_VALID(s)      ((s) != INVALID_SOCKET)
#else
#  include <arpa/inet.h>
#  include <netinet/in.h>
#  include <netdb.h>
#  include <resolv.h>
#  include <sys/socket.h>
#  include <sys/select.h>
#  include <unistd.h>
#  define SockDesc              int
#  define SOCKET_IS_VALID(s)    ((s) >= 0)
#  define INVALID_SOCKET        (-1)
#  define SD_BOTH               SHUT_RDWR
#  define SD_SEND               SHUT_WR
#  define SD_RECEIVE            SHUT_RD
#  define closesocket(fd)       close(fd)
#endif
