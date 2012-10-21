#include <list>

#include "Test.hpp"
#include "TestUnit.hpp"
#include "NetworkTestUnit.hpp"

using namespace UnitTesting;
using namespace std;

list<TestUnit*> testUnits;

int main()
{
	//! \todo Load units into testUnits here
	testUnits.push_back(new NetworkTestUnit());

	for (TestUnit* curr : testUnits) {
		try {
			printf("Beginning test unit %s...\n\n", curr->getUnitName());
			curr->runUnit();
		}
		catch (...) {
			printf("The test unit %s threw an unexpected exception.\n",
			       curr->getUnitName());
		}
		delete curr;
	}
	printf("\n");
	return 0;
}
