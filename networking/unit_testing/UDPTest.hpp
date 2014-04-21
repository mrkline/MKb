#pragma once

#include "Test.hpp"
#include "UDPClient.hpp"

namespace UnitTesting {
	class UDPTest : public Test {
	public:
		const char* getName() const { return "UDP"; }

		void run()
		{
			const char* testData = "Twenty-five or six to four";
			const size_t testDataLen = strlen(testData);
			char* buff = reinterpret_cast<char*>(calloc(testDataLen + 1, 1));

			const int serverPort = 1338;
			const int clientPort = 1339;

			try {
				// Create a listener and a client connection
				UDPClient server(serverPort);
				UDPClient client(clientPort);

				client.setDefaultDestination(IPEndPoint(IP(127, 0, 0, 1), serverPort));
				server.setDefaultDestination(IPEndPoint(IP(127, 0, 0, 1), clientPort));

				// Test sending from the server to the client
				if (server.send(testData, testDataLen) != testDataLen)
					throw TestFailedException("UDP send failed");

				if (client.receive(buff, testDataLen) != testDataLen)
					throw TestFailedException("UDP receive failed");

				if (strcmp(testData, buff) != 0)
					throw TestFailedException("Data sent from server to client didn't go through properly");

				// Reset the buffer and try sending from
				// the client to the server
				memset(buff, 0, testDataLen);

				if (client.send(testData, testDataLen) != testDataLen)
					throw TestFailedException("UDP send failed");

				if (server.receive(buff, testDataLen) != testDataLen)
					throw TestFailedException("UDP receive failed");

				if (strcmp(testData, buff) != 0)
					throw TestFailedException("Data sent from client to server didn't go through properly");

				free(buff);
			}
			catch (...) {
				free(buff);
				throw;
			}
		}

	};
} // end namespace UnitTesting
