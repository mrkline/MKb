#pragma once

#include <cstdio>
#include <list>

#include "Test.hpp"

namespace UnitTesting
{
	/*!
	\brief Provides a base abstract class for test units.

	A test unit is a group of related tests related to a given component.
	*/
	class TestUnit
	{
	public:
		//! Deletes all tests
		virtual ~TestUnit()
		{
			for (auto iter = succeededTests.begin();
			        iter != succeededTests.end(); ++iter)
			{
				delete *iter;
			}
			for (auto iter = failedTests.begin();
			        iter != failedTests.end(); ++iter)
			{
				delete *iter;
			}
		}

		//! Returns this unit's name
		virtual const char* getUnitName() const = 0;

		//! Executes all Tests and prints a summary of the unit
		void runUnit()
		{
			const char* name = getUnitName();

			loadTests();

			for (auto iter = unrunTests.begin();
			        iter != unrunTests.end(); ++iter)
			{
				Test* curr = *iter;
				const char* currName = curr->getName();
				try
				{
					curr->run();
					printf("SUCCESS: %s test succeeded\n", currName);
					succeededTests.push_back(curr);
				}
				catch (const TestFailedException& ex)
				{
					printf("FAILURE: %s test failed with the message:\n\t%s\n",
					       currName, ex.message.c_str());
					failedTests.push_back(curr);
				}
				catch (const Exceptions::Exception& ex)
				{
					printf("FAILURE: The method %s threw an exception"
					       "with the following message:\n\t%s\n",
					       ex.callingFunction.c_str(), ex.message.c_str());
				}
				catch (...)
				{
					printf("FAILURE: %s test failed and thew an"
					       " unknown exception\n", currName);
					failedTests.push_back(curr);
				}
			}

			printf("\nResults for %s\n", name);

			if (!succeededTests.empty())
			{
				printf("\tSucceeded Tests:\n");
				for (auto iter = succeededTests.begin();
				        iter != succeededTests.end(); ++iter)
				{
					printf("\t\t%s\n", (*iter)->getName());
				}
			}

			if (!failedTests.empty())
			{
				printf("\tFailed Tests:\n");
				for (auto iter = failedTests.begin();
				        iter != failedTests.end(); ++iter)
				{
					printf("\t\t%s\n", (*iter)->getName());
				}
			}
		}

	protected:
		//! Loads tests into unrunTests.
		//Tests will automatically be deleted on this unit's destruction
		virtual void loadTests() = 0;

		std::list<Test*> unrunTests;

	private:
		std::list<Test*> succeededTests;
		std::list<Test*> failedTests;
	};
} // end namespace UnitTesting
