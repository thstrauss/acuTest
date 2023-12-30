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

int cuTest_equalInt(const TestParameter* parameter) {
	return *(const int *) parameter->actual == *(const int*) parameter->expected;
}

void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d not equal to expected value %d: %s", parameter->fileName, parameter->line, *(const int*)parameter->actual, *(const int*)parameter->expected, parameter->message);
}

int cuTest_notEqualInt(const TestParameter* parameter) {
	return *(const int*)parameter->actual != *(const int*)parameter->expected;
}

void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d equal to expected value %d: %s", parameter->fileName, parameter->line, * (const int *) parameter->actual, * (const int *) parameter->expected, parameter->message);
}

int cuTest_equalPtr(const TestParameter* parameter) {
	return parameter->actual == parameter->expected;
}

void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_notEqualPtr(const TestParameter* parameter) {
	return parameter->actual != parameter->expected;
}

void cuTest_notEqualPtrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_equalStr(const TestParameter* parameter) {
	return strcmp((const char*) (parameter->actual), (const char*)(parameter->expected)) == 0;
}

void cuTest_equalStrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*) parameter->actual, (const char*) parameter->expected, parameter->message);
}

int cuTest_notEqualStr(const TestParameter* parameter) {
	return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const TestParameter* parameter),
	void (*formatMessage)(char* buffer, int bufferSize, const TestParameter* parameter),
	const TestParameter* parameter
) {
	char buffer[1024];
	int assertResult = assertFunc(parameter);
	
	sprintf_s(buffer, sizeof(buffer), "");
	if (!assertResult) {
		if (formatMessage != NULL) {
			formatMessage(buffer, sizeof(buffer), parameter);
		}
	}
	cuTest_assertCore(environment, assertResult, buffer);
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
