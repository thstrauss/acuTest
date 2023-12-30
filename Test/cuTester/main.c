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
	cuTest_assertEqualStr(environment, "str", "abc", "assert2");
}

int main() {
	TestResult* result = cuTest_run(test, "test");

	TestResult* result2 = cuTest_run(test2, "test2");

	TestResult* result3 = cuTest_run(test3, "test3");
	printf("\n\r");
	printf("%s: %s\n\r", result->name, result->message);

	printf("%s: %s\n\r", result2->name, result2->message);

	printf("%s: %s\n\r", result3->name, result3->message);

	cuTest_destroy(result);
	cuTest_destroy(result2);
	cuTest_destroy(result3);
	return 0;
}