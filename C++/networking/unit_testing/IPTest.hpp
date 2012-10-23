#pragma once

#include "IP.hpp"
#include "Test.hpp"

#include <cstring>

namespace UnitTesting {
	class IPTest : public Test {
	public:
		const char* getName() const { return "IP"; }

		void run()
		{
			// Test IP construction
			IP fromString("192.168.0.1");

			if (fromString.getOctet(0) != 192
			        || fromString.getOctet(1) != 168
			        || fromString.getOctet(2) != 0
			        || fromString.getOctet(3) != 1)
				throw TestFailedException("An IP could not be properly constructed from a string.");

			// Test provided octet construction
			IP fromOctets(192, 168, 0, 1);

			if (fromOctets.getOctet(0) != 192
			        || fromOctets.getOctet(1) != 168
			        || fromOctets.getOctet(2) != 0
			        || fromOctets.getOctet(3) != 1)
				throw TestFailedException("An IP could not be properly constructed from octets.");

			// Test string representation
			if (strcmp("192.168.0.1", fromOctets.getAsString().c_str()) != 0)
				throw TestFailedException("An IP's string representation was incorrect.");

			// Test inequality
			if (fromString != fromOctets)
				throw TestFailedException("Inequality operator failed");

			// Test equality
			IP different = (std::string) "127.0.0.1";

			if (different.getOctet(0) != 127 || different.getOctet(1) != 0
			        || different.getOctet(2) != 0 || different.getOctet(3) != 1)
				throw TestFailedException("An IP could not be constructed from a string.");

			if (fromOctets == different)
				throw TestFailedException("Equality operator failed");

			// Test assignment from string
			different = "74.125.113.99";

			if (different.getOctet(0) != 74
			        || different.getOctet(1) != 125
			        || different.getOctet(2) != 113
			        || different.getOctet(3) != 99)
				throw TestFailedException("An IP could not be assigned from a string.");

			// Test binary representation
			unsigned int binRep = (208 << 24) + (47 << 16) + (17 << 8) + 18;
			IP fromBin(binRep, IP::BO_HOST);

			if (binRep != fromBin.getAsBinary(IP::BO_HOST))
				throw TestFailedException("An IP's binary representation was incorrect.");
		}

	};

} // end namespace UnitTesting
