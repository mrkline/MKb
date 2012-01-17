#pragma once

#include "Exceptions.hpp"

namespace UnitTesting
{
	//! Thrown out of Test::Run() if a test fails in an anticipated manner
	class TestFailedException : public Exceptions::Exception
	{
	public:
		TestFailedException(const char* exceptionMessage)
				: Exception(exceptionMessage, nullptr)
		{ }
	};

	//! Represents a single test, a series of which is run by each TestUnit
	class Test
	{
	public:
		//! Returns the name of the test
		virtual const char* GetName() const = 0;

		/*!
		\brief Runs the test. Error reporting is done via exceptions
		\throws TestFailedException if the test fails in an anticipated manner.
		*/
		virtual void Run() = 0;
	};
} // end namespace UnitTesting
