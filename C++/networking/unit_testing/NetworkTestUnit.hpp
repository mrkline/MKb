#pragma once

#include "TestUnit.hpp"
#include "IPTest.hpp"
#include "TCPTest.hpp"

namespace UnitTesting
{
	class NetworkTestUnit : public TestUnit
	{
		const char* GetUnitName() const { return "Network"; }

		void LoadTests()
		{
			unrunTests.push_back(new IPTest());
			unrunTests.push_back(new TCPTest());
		}

	};
} // end namespace Unit Testing
