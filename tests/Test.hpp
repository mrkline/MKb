#ifndef __MKB_TEST_HPP__
#define __MKB_TEST_HPP__

#include <cassert>
#include <cstdio>
#include <functional>

#include "Exceptions.hpp"

namespace Testing {

template <typename T, typename E>
inline void assertThrown(E expr)
{
	try { expr(); }
	catch (const T& thrown) {
		return;
	}

	// We didn't catch it
	assert(false);
}

/// Just prints a "starting test unit Foo"
inline void beginUnit(const char* unitName)
{
	printf("\nStarting test unit %s\n", unitName);
}

/// Runs a test and prints the success message
template <typename T>
inline void test(const char* testName, T theTest)
{
	theTest();
	// Since tests are assumed to fail an assert() and core dump if they fail,
	// yay! We made it if we get this far.
	printf("SUCCESS: %s test succeeded\n", testName);
}

} // end namespace Testing

#endif
