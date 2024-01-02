#include <stdio.h>
#include "..\..\Source\cu_test.h"

void test1(TestEnvironment* environment) {
	cuTest_PrepareParameter(int, 1, 2, "assert", __LINE__ + 1);
	cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter);
}

void test2(TestEnvironment* environment) {
	cuTest_assertEqualInt(environment, 1, 1, "assert2");
	cuTest_assertNotEqualInt(environment, 1, 2, "xxx");
}

void test3(TestEnvironment* environment) {
	char str[] = "abc";
	cuTest_assertEqualStr(environment, str, str, "assert2");
	cuTest_assertEqualStr(environment, "abc", "abc", "assert2");
	cuTest_assertNotEqualStr(environment, "str", "abc", "assert2");
}

int main() {
	TestFixture fixture;
	int returnValue;

	cuTest_init(&fixture, "testFixture");

	cuTest_addTestCase(&fixture, "test1", test1);
	cuTest_addTestCase(&fixture, "test2", test2);
	cuTest_addTestCase(&fixture, "test3", test3);

	cuTest_execute(&fixture);

	returnValue = cuTest_report(&fixture) == CU_TEST_PASSED ? 0 : 2;

	cuTest_destroy(&fixture);

	return returnValue;
}