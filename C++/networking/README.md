## Directory Contents

- IP represents an IPv4 address and is used by most of the other classes in this
  folder.
- WinsockRequirement is an RAII class used to initialize and shutdown
  Winsock automatically when using other classes in this library. All classes
  with a Winsock dependency have an instance of it as a private member.
- TCPListener provides facilities to listen for and accept incoming TCP
  connection requests.
- TCPConnection is used to connect to a TCP server. It is also returned by
  TCPListener when a connection is accepted.
- UDPClient is used to send data via the UDP protocol and, when bound to a port,
  receive data via UDP.

## Todo

- Make any needed changes now that exceptions are in a namespace
- Try to get lines to 80 columns max
