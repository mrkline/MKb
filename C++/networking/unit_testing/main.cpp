#include <memory>
#include <queue>

#include "Test.hpp"
#include "TestUnit.hpp"
#include "NetworkTestUnit.hpp"

using namespace UnitTesting;
using namespace std;

queue<unique_ptr<TestUnit>> testUnits;

int main()
{
	/// \todo Load units into testUnits here
	testUnits.push(unique_ptr<TestUnit>(new NetworkTestUnit()));

	while (!testUnits.empty()) {
		const auto& curr = testUnits.front();
		try {
			printf("Beginning test unit %s...\n\n", curr->getUnitName());
			curr->runUnit();
		}
		catch (...) {
			printf("The test unit %s threw an unexpected exception.\n",
			       curr->getUnitName());
		}
		testUnits.pop();
	}
	printf("\n");
	return 0;
}
