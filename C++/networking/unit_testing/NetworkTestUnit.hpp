#pragma once

#include "TestUnit.hpp"
#include "IPTest.hpp"
#include "TCPTest.hpp"
#include "UDPTest.hpp"

namespace UnitTesting {
	class NetworkTestUnit : public TestUnit {
	public:
		const char* getUnitName() const { return "Network"; }

		void loadTests()
		{
			unrunTests.push(std::unique_ptr<Test>(new IPTest()));
			unrunTests.push(std::unique_ptr<Test>(new TCPTest()));
			unrunTests.push(std::unique_ptr<Test>(new UDPTest()));
		}

	};
} // end namespace Unit Testing
