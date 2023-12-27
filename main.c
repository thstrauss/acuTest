#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "try_catch_throw.h"

#define PASSED 0
#define FAILED 1
#define ERROR  2

typedef struct TestResult_ {
	int result;
	const char* message;
	const char* name;
} TestResult;

typedef struct TestEnvironment_ {
	jmp_buf assertBuf;
	TestResult* result;
} TestEnvironment;

void freeTestResult(TestResult* result) {
	if (result->name != NULL) {
		free(result->name);
	}
	if (result->message != NULL) {
		free(result->message);
	}
	free(result);
}

void assertCore(TestEnvironment* environment, int conditionResult, const char* message) {
	environment->result->message = _strdup(message);
	if (environment->result->message == NULL) {
		environment->result->result = ERROR;
		longjmp(environment->assertBuf, ERROR);
	}
	if (!conditionResult) {
		environment->result->result = FAILED;
		longjmp(environment->assertBuf, FAILED);
	}
};

void assertEqual(TestEnvironment* environment, int i1, int i2, const char* message) {
	char buffer[1024];
	if (i1 != i2) {
		sprintf_s(buffer, 1024, "%d == %d: %s", i1, i2, message);
	}
	else {
		sprintf_s(buffer, 1024, "");
	}
	assertCore(environment, i1 == i2, buffer);
}

TestResult* runTest(void (*testFunc)(TestEnvironment* environment), const char* name) {
	TestEnvironment environment;

	TestResult* result = (TestResult*)malloc(sizeof(TestResult));
	if (result == NULL) {
		printf("E");
		return NULL;
	}
	result->result = PASSED;
	result->message = NULL;
	result->name = _strdup(name);

	environment.result = result;

	do {
		switch (setjmp(environment.assertBuf)) {
			case 0: while (1) {
				testFunc(&environment);
				break;
			}
			printf(".");
			break;
			case FAILED: {
				printf("F");
				break;
			}
			case ERROR: {
				printf("E");
				break;
			}
		}
	} while (0);
	return result;
}

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