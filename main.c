#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cuTest.h"



void test(TestEnvironment* environment) {
	int actual = 1;
	int expected = 2;
	cuTest_assert(environment, &cuTest_equalInt, NULL, &actual, &expected, __FILE__, __LINE__, "assert");
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