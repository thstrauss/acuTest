#include <stdio.h>
#include "cuTest.h"



void test(TestEnvironment* environment) {
	int actual = 1;
	int expected = 2;
	TestParameter parameter = {
		.actual = &actual,
		.expected = &expected,
		.fileName = __FILE__,
		.line = __LINE__+3,
		.message = "assert"
	};
	cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter);
}

void test2(TestEnvironment* environment) {
	cuTest_assertEqualInt(environment, 1, 1, "assert2");
	cuTest_assertNotEqualInt(environment, 1, 1, "xxx");
}

int main() {
	TestResult* result = cuTest_run(test, "test");

	TestResult* result2 = cuTest_run(test2, "test2");
	printf("\n\r");
	printf("%s: %s\n\r", result->name, result->message);

	printf("%s: %s\n\r", result2->name, result2->message);

	cuTest_destroy(result);
	cuTest_destroy(result2);

	return 0;
}