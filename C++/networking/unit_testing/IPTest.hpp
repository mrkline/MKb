#pragma once

#include "IP.hpp"
#include "Test.hpp"

#include <cstring>

namespace UnitTesting
{
	class IPTest : public Test
	{
		const char* GetName() const { return "IP"; }

		void Run()
		{
			// Test IP construction
			IP fromString("192.168.0.1");

			if (fromString.GetOctet(0) != 192
			        || fromString.GetOctet(1) != 168
			        || fromString.GetOctet(2) != 0
			        || fromString.GetOctet(3) != 1)
			{
				throw TestFailedException("An IP could not be"
				                          " properly constructed from a string.");
			}

			// Test provided octet construction
			IP fromOctets(192, 168, 0, 1);

			if (fromOctets.GetOctet(0) != 192
			        || fromOctets.GetOctet(1) != 168
			        || fromOctets.GetOctet(2) != 0
			        || fromOctets.GetOctet(3) != 1)
			{
				throw TestFailedException(
				    "An IP could not be properly constructed from octets.");
			}

			// Test string representation
			if (strcmp("192.168.0.1", fromOctets.GetAsString().c_str()) != 0)
			{
				throw TestFailedException(
				    "An IP's string representation was incorrect.");
			}

			// Test inequality
			if (fromString != fromOctets)
				throw TestFailedException("Inequality operator failed");

			// Test equality
			IP different = (std::string) "127.0.0.1";

			if (different.GetOctet(0) != 127 || different.GetOctet(1) != 0
			        || different.GetOctet(2) != 0 || different.GetOctet(3) != 1)
			{
				throw TestFailedException(
				    "An IP could not be constructed from a string.");
			}

			if (fromOctets == different)
				throw TestFailedException("Equality operator failed");

			// Test assignment from string
			different = "74.125.113.99";

			if (different.GetOctet(0) != 74
			        || different.GetOctet(1) != 125
			        || different.GetOctet(2) != 113
			        || different.GetOctet(3) != 99)
			{
				throw TestFailedException(
				    "An IP could not be assigned from a string.");
			}

			// Test binary representation
			unsigned int binRep = (208 << 24) + (47 << 16) + (17 << 8) + 18;
			IP fromBin(binRep);

			if (binRep != fromBin.GetAsBinary())
			{
				throw TestFailedException(
				    "An IP's binary representation was incorrect.");
			}
		}

	};

} // end namespace UnitTesting
