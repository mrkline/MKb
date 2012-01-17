#pragma once

#include "TCPConnection.hpp"
#include "TCPListener.hpp"
#include "Test.hpp"

namespace UnitTesting
{
	class TCPTest : public Test
	{
		const char* GetName() const { return "TCP"; }

		void Run()
		{
			const char* testData = "Twenty-five or six to four";
			const size_t testDataLen = strlen(testData);
			char* buff = reinterpret_cast<char*>(calloc(testDataLen + 1, 1));

			const int port = 1337;

			try
			{
				// Create a listener and a client connection
				TCPListener server(port);
				TCPConnection client;

				// Start the server and connect to it
				server.Start();
				client.Connect(IPEndPoint(IP(127, 0, 0, 1), port));

				// Accept the connection
				TCPConnection* serverConn = server.Accept();

				// Test sending from the server to the client
				serverConn->Send(testData, testDataLen);
				client.Receive(buff, testDataLen);

				if (strcmp(testData, buff) != 0)
				{
					throw TestFailedException("Data sent from server to client"
					                          " didn't go through properly");
				}

				// Reset the buffer and try sending from
				// the client to the server
				memset(buff, 0, testDataLen);

				serverConn->ShutDownSending();
				client.ShutDownReceiving();

				client.Send(testData, testDataLen);
				serverConn->Receive(buff, testDataLen);

				if (strcmp(testData, buff) != 0)
				{
					throw TestFailedException("Data sent from client to server"
					                          " didn't go through properly");
				}

				// Shut down the connections
				client.Disconnect();

				if (serverConn->Receive(buff, testDataLen) != 0)
				{
					throw TestFailedException("The server was not notified"
					                          " when the client disconnected");
				}

				serverConn->Disconnect();

				delete serverConn;
				free(buff);
			}
			catch (...)
			{
				free(buff);
				throw;
			}
		}

	};
} // end namespace UnitTesting
