#include <stdio.h>
#include "cuTest.h"



void test(TestEnvironment* environment) {
	TestParameter parameter;
	int actual = 1;
	int expected = 2;
	parameter.actual = &actual;
	parameter.expected = &expected;
	parameter.fileName = __FILE__;
	parameter.line = __LINE__;
	parameter.message = "assert";
	cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter);
}

//void test2(TestEnvironment* environment) {
//	assertEqual(environment, &equalInt, 1, 1, "assert2");
//}

int main() {
	TestResult* result = cuTest_run(test, "test");

	//TestResult* result2 = runTest(test2, "test2");
	printf("\n\r");
	printf("%s: %s\n\r", result->name, result->message);

	//printf("%s: %s\n\r", result2->name, result2->message);

	cuTest_destroy(result);
	//freeTestResult(result2);

	return 0;
}