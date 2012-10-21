#pragma once

#include <cstdio>
#include <memory>
#include <list>
#include <queue>

#include "Test.hpp"

namespace UnitTesting {
	/*!
	\brief Provides a base abstract class for test units.

	A test unit is a group of related tests related to a given component.
	*/
	class TestUnit {
	public:

		//! Returns this unit's name
		virtual const char* getUnitName() const = 0;

		//! Executes all Tests and prints a summary of the unit
		void runUnit()
		{
			const char* name = getUnitName();

			loadTests();

			while (!unrunTests.empty()) {
				auto& curr = unrunTests.front();
				const char* currName = curr->getName();
				try {
					curr->run();
					printf("SUCCESS: %s test succeeded\n", currName);
					succeededTests.push_back(std::move(curr));
				}
				catch (const TestFailedException& ex) {
					printf("FAILURE: %s test failed with the message:\n\t%s\n", currName, ex.message.c_str());
					failedTests.push_back(std::move(curr));
				}
				catch (const Exceptions::Exception& ex) {
					printf("FAILURE: The method %s threw an exception with the following message:\n\t%s\n",
					       ex.callingFunction.c_str(), ex.message.c_str());
				}
				catch (...) {
					printf("FAILURE: %s test failed and thew an unknown exception\n", currName);
					failedTests.push_back(std::move(curr));
				}
				unrunTests.pop();
			}

			printf("\nResults for %s\n", name);

			if (!succeededTests.empty()) {
				printf("\tSucceeded Tests:\n");
				for (const auto& test : succeededTests)
					printf("\t\t%s\n", test->getName());
			}

			if (!failedTests.empty()) {
				printf("\tFailed Tests:\n");
				for (const auto& test : failedTests)
					printf("\t\t%s\n", test->getName());
			}
		}

	protected:
		//! Loads tests into unrunTests.
		//Tests will automatically be deleted on this unit's destruction
		virtual void loadTests() = 0;

		std::queue<std::unique_ptr<Test>> unrunTests;

	private:
		std::list<std::unique_ptr<Test>> succeededTests;
		std::list<std::unique_ptr<Test>> failedTests;
	};
} // end namespace UnitTesting
