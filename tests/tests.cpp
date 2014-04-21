#include <cstdio>

#include "Test.hpp"
#include "PoolTests.hpp"

int main()
{
	using namespace Testing;

	printf("Running unit tests...\n");
	runPoolTests();
	return 0;
}
