## Directory Contents

- IP represents an IPv4 address and is used by most of the other classes in this
  folder.
- TCPListener provides facilities to listen for and accept incoming TCP
  connection requests.
- TCPConnection is used to connect to a TCP server. It is also returned by
  TCPListener when a connection is accepted.
- UDPClient is used to send data via the UDP protocol and, when bound to a port,
  receive data via UDP.
- SocketMultiplexer is a C++ wrapper of the *select* function, allowing a single
  thread to check if multiple sockets are ready for non-blocking sending and
  receiving.
- Portability.hpp contains typedefs and #defines needed to make the code compile
  for both WinSock and \*nix socket libraries.
- WinsockRequirement is an RAII class used to initialize and shutdown
  Winsock automatically when using other classes in this library. All classes
  with a Winsock dependency have an instance of it as a private member.

  **Note**: WinsockRequirement.hpp/.cpp should not be compiled for
  non-windows projects.

## Notes

- These files rely on Exceptions.hpp, one directory up.
