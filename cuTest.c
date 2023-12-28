#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "cuTest.h"

void cuTest_destroy(TestResult* result) {
	if (result->name != NULL) {
		free(result->name);
	}
	if (result->message != NULL) {
		free(result->message);
	}
	free(result);
}

static void cuTest_assertCore(TestEnvironment* environment, int conditionResult, const char* message) {
	environment->result->message = _strdup(message);
	if (environment->result->message == NULL) {
		environment->result->result = CU_TEST_ERROR;
		longjmp(environment->assertBuf, CU_TEST_ERROR);
	}
	if (!conditionResult) {
		environment->result->result = CU_TEST_FAILED;
		longjmp(environment->assertBuf, CU_TEST_FAILED);
	}
};

int cuTest_equalInt(const void* actualValue, const void* expectedValue) {
	return *(const int *) actualValue == *(const int*) expectedValue;
}

void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const void* actualValue, const void* expectedValue), 
	char* (*formatMessage)(int asserted, const char* fileName, int line, const char* message),
	const void* actualValue, const void* expectedValue, 
	const char* fileName, int line, const char* message
) {
	char buffer[1024];
	if (!(assertFunc(actualValue, expectedValue))) {
		sprintf_s(buffer, 1024, "%s:%d %s", fileName, line, message);
	}
	else {
		sprintf_s(buffer, 1024, "");
	}
	cuTest_assertCore(environment, assertFunc(actualValue, expectedValue), buffer);
}

TestResult* cuTest_run(void (*testFunc)(TestEnvironment* environment), const char* name) {
	TestEnvironment environment;

	TestResult* result = (TestResult*)malloc(sizeof(TestResult));
	if (result == NULL) {
		printf("E");
		return NULL;
	}
	result->result = CU_TEST_PASSED;
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
		case CU_TEST_FAILED: {
			printf("F");
			break;
		}
		case CU_TEST_ERROR: {
			printf("E");
			break;
		}
		}
	} while (0);
	return result;
}
