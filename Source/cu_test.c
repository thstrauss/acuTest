#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "cu_test.h"
#include "cu_tcse.h"
#include "cu_cmmn.h"
#include "cu_list.h"

static void cuTest_destroyTestResult(TestResult* result) {
	if (result->message != NULL) {
		free(result->message);
	}
	free(result);
}

static void cuTest_run(TestCase* testCase) {
	ExecuteEnv environment;

	TestResult* result = (TestResult*)malloc(sizeof(TestResult));
	if (result == NULL) {
		printf("E");
		return;
	}
	result->status = CU_TEST_PASSED;
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

void cuTest_init(Fixture* fixture, const char* name) {
	CU_List* testCases = (CU_List*) malloc(sizeof(CU_List));
	cu_listInit(testCases, cuTest_destroyTestCase);
	fixture->testCases = testCases;
	fixture->name = _strdup(name);
}

void cuTest_addTestCase(Fixture* fixture, const char* name, void (*testFunc)(ExecuteEnv* environment)) {
	TestCase* testCase = (TestCase*)malloc(sizeof(TestCase));
	if (testCase != NULL) {
		testCase->name = _strdup(name);
		testCase->testFunc = testFunc;
		cu_listAppend(fixture->testCases, (void*)testCase);
	}
}

void cuTest_execute(Fixture* fixture) {
	CU_ListElement* testElement = cu_listHead(fixture->testCases);

	while (testElement != NULL) {
		cuTest_run((TestCase*) testElement->data);
		testElement = cu_listNext(testElement);
	}
}

int cuTest_report(Fixture* fixture) {
	CU_ListElement* testElement = cu_listHead(fixture->testCases);

	int accumulatedResult = CU_TEST_PASSED;
	
	printf("\n\r");
	while (testElement != NULL) {
		TestCase* testCase = (TestCase*) testElement->data;
		if (testCase->result->status != CU_TEST_PASSED) {
			printf("%s: %s\n\r", testCase->name, testCase->result->message);
			accumulatedResult = testCase->result->status;
		}
		testElement = cu_listNext(testElement);
	}

	return accumulatedResult;
}

void cuTest_destroy(Fixture* fixture) {
	cu_listDestroy(fixture->testCases);
}
