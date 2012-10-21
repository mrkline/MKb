#pragma once

#include "TestUnit.hpp"
#include "IPTest.hpp"
#include "TCPTest.hpp"

namespace UnitTesting {
	class NetworkTestUnit : public TestUnit {
	public:
		const char* getUnitName() const { return "Network"; }

		void loadTests()
		{
			unrunTests.push_back(new IPTest());
			unrunTests.push_back(new TCPTest());
		}

	};
} // end namespace Unit Testing
