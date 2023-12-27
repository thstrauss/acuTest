#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "cuTest.h"



void test(TestEnvironment* environment) {
	assertEqual(environment, 1, 2, "assert");
}

void test2(TestEnvironment* environment) {
	assertEqual(environment, 1, 1, "assert2");
}

int main() {
	TestResult* result = runTest(test, "test");

	TestResult* result2 = runTest(test2, "test2");
	printf("\n\r");
	printf("%s: %s\n\r", result->name, result->message);

	printf("%s: %s\n\r", result2->name, result2->message);

	freeTestResult(result);
	freeTestResult(result2);

	return 0;
}