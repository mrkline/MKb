// This file contains kludgy magic that lets us build for both Windows and
// Posix.

#ifdef _WIN32
#  define FMS_WINDOWS_BUILD //! Use this if #ifdefs are needed in other files
#  include <winsock2.h>
#  include <ws2tcpip.h>
#  define SockDesc                SOCKET
#  define SOCKET_IS_VALID(s)      ((s) != INVALID_SOCKET)
#else
#  define FMS_POSIX_BUILD //! Use this if #ifdefs are needed in other files
#  if HAVE_SYS_WAIT_H
#    include <sys/wait.h>
#  endif
#  include <sys/socket.h>
#  include <sys/un.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <resolv.h>
#  include <netdb.h>
#  include <sys/stat.h>
#  define SockDesc              int
#  define SOCKET_IS_VALID(s)    ((s) >= 0)
#  define INVALID_SOCKET        (-1)
#  define SD_BOTH               SHUT_RDWR
#  define SD_SEND               SHUT_WR
#  define SD_RECEIVE            SHUT_RD
#  define closesocket(fd)       close(fd)
#endif
