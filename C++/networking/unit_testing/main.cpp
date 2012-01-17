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

	for (auto iter = testUnits.begin(); iter != testUnits.end(); ++iter)
	{
		TestUnit* curr = *iter;
		try
		{
			printf("Beginning test unit %s...\n\n", curr->GetUnitName());
			curr->RunUnit();
		}
		catch (...)
		{
			printf("The test unit %s threw an unexpected exception.\n",
			       curr->GetUnitName());
		}
		delete *iter;
	}
	printf("\n");
	return 0;
}
