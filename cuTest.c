#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "cuTest.h"

void freeTestResult(TestResult* result) {
	if (result->name != NULL) {
		free(result->name);
	}
	if (result->message != NULL) {
		free(result->message);
	}
	free(result);
}

static void assertCore(TestEnvironment* environment, int conditionResult, const char* message) {
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

void assertEqual(TestEnvironment* environment, int i1, int i2, const char* message) {
	char buffer[1024];
	if (!(i1 == i2)) {
		sprintf_s(buffer, 1024, "expected %d to be equal to %d: %s", i1, i2, message);
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
