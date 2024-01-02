#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "cuTest.h"

static void cuTest_destroyTestResult(TestResult* result) {
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

int cuTest_equalInt(const AssertParameter* parameter) {
	return *(const int *) parameter->actual == *(const int*) parameter->expected;
}

void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d not equal to expected value %d: %s", parameter->fileName, parameter->line, *(const int*)parameter->actual, *(const int*)parameter->expected, parameter->message);
}

int cuTest_notEqualInt(const AssertParameter* parameter) {
	return *(const int*)parameter->actual != *(const int*)parameter->expected;
}

void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d equal to expected value %d: %s", parameter->fileName, parameter->line, * (const int *) parameter->actual, * (const int *) parameter->expected, parameter->message);
}

int cuTest_equalPtr(const AssertParameter* parameter) {
	return parameter->actual == parameter->expected;
}

void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_notEqualPtr(const AssertParameter* parameter) {
	return parameter->actual != parameter->expected;
}

void cuTest_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_equalStr(const AssertParameter* parameter) {
	return strcmp((const char*) (parameter->actual), (const char*)(parameter->expected)) == 0;
}

void cuTest_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*) parameter->actual, (const char*) parameter->expected, parameter->message);
}

int cuTest_notEqualStr(const AssertParameter* parameter) {
	return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
	sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const AssertParameter* parameter),
	void (*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter),
	const AssertParameter* parameter
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

static void cuTest_run(TestCase* testCase) {
	TestEnvironment environment;

	TestResult* result = (TestResult*)malloc(sizeof(TestResult));
	if (result == NULL) {
		printf("E");
		return;
	}
	result->result = CU_TEST_PASSED;
	result->message = NULL;

	environment.result = result;

	do {
		switch (setjmp(environment.assertBuf)) {
		case 0: while (1) {
			testCase->testFunc(&environment);
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
	testCase->result = result;
}

static void cuTest_destroyTestCase(void* data) {
	TestCase* testCase = (TestCase*) data;
	free(testCase->name);
	if (testCase->result != NULL) {
		cuTest_destroyTestResult(testCase->result);
	}
}

void cuTest_init(TestFixture* fixture, const char* name) {
	List* testCases = (List*) malloc(sizeof(List));
	cu_listInit(testCases, cuTest_destroyTestCase);
	fixture->testCases = testCases;
	fixture->name = _strdup(name);
}

void cuTest_addTestCase(TestFixture* fixture, const char* name, void (*testFunc)(TestEnvironment* environment)) {
	TestCase* testCase = (TestCase*)malloc(sizeof(TestCase));
	if (testCase != NULL) {
		testCase->name = _strdup(name);
		testCase->testFunc = testFunc;
		cu_listAppend(fixture->testCases, (void*)testCase);
	}
}

void cuTest_execute(TestFixture* fixture) {
	ListElement* testElement = cu_listHead(fixture->testCases);

	while (testElement != NULL) {
		cuTest_run((TestCase*) testElement->data);
		testElement = cu_listNext(testElement);
	}
}

int cuTest_report(TestFixture* fixture) {
	ListElement* testElement = cu_listHead(fixture->testCases);

	int accumulatedResult = CU_TEST_PASSED;
	
	printf("\n\r");
	while (testElement != NULL) {
		TestCase* testCase = (TestCase*) testElement->data;
		if (testCase->result->result != CU_TEST_PASSED) {
			printf("%s: %s\n\r", testCase->name, testCase->result->message);
			accumulatedResult = testCase->result->result;
		}
		testElement = cu_listNext(testElement);
	}

	return accumulatedResult;
}

void cuTest_destroy(TestFixture* fixture) {
	cu_listDestroy(fixture->testCases);
}
