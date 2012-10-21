#pragma once

#include "TCPConnection.hpp"
#include "TCPListener.hpp"
#include "Test.hpp"

namespace UnitTesting {
	class TCPTest : public Test {
	public:
		const char* getName() const { return "TCP"; }

		void run()
		{
			const char* testData = "Twenty-five or six to four";
			const size_t testDataLen = strlen(testData);
			char* buff = reinterpret_cast<char*>(calloc(testDataLen + 1, 1));

			const int port = 1337;

			try {
				// Create a listener and a client connection
				TCPListener server(port);
				TCPConnection client;

				// Start the server and connect to it
				server.start();
				client.connect(IPEndPoint(IP(127, 0, 0, 1), port));

				// Accept the connection
				TCPConnection* serverConn = server.accept();

				// Test sending from the server to the client
				serverConn->send(testData, testDataLen);
				client.receive(buff, testDataLen);

				if (strcmp(testData, buff) != 0)
					throw TestFailedException("Data sent from server to client didn't go through properly");

				// Reset the buffer and try sending from
				// the client to the server
				memset(buff, 0, testDataLen);

				serverConn->shutDownSending();
				client.shutDownReceiving();

				client.send(testData, testDataLen);
				serverConn->receive(buff, testDataLen);

				if (strcmp(testData, buff) != 0)
					throw TestFailedException("Data sent from client to server didn't go through properly");

				// Shut down the connections
				client.disconnect();

				if (serverConn->receive(buff, testDataLen) != 0)
					throw TestFailedException("The server was not notified when the client disconnected");

				serverConn->disconnect();

				delete serverConn;
				free(buff);
			}
			catch (...) {
				free(buff);
				throw;
			}
		}

	};
} // end namespace UnitTesting
