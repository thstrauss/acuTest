#include <stdio.h>
#include "..\..\Source\cuTest.h"

void test(TestEnvironment* environment) {
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
	TestCase case1;
	case1.name = "test";
	case1.testFunc = &test;

	TestFixtureInit(&fixture, "testFixture");

	TestFixtureAdd(&fixture, &case1);

	TestFixtureExecute(&fixture);

	printf("\n\r");
	printf("%s: %s\n\r", case1.name, case1.result->message);

	cuTest_destroy(case1.result);

	return 0;
}