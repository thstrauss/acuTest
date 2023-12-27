#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include <setjmp.h>

#define CU_TEST_PASSED 0
#define CU_TEST_FAILED 1
#define CU_TEST_ERROR  2

typedef struct TestResult_ {
	int result;
	char* message;
	char* name;
} TestResult;

typedef struct TestEnvironment_ {
	jmp_buf assertBuf;
	TestResult* result;
} TestEnvironment;

void freeTestResult(TestResult* result);
void assertEqual(TestEnvironment* environment, int i1, int i2, const char* message);

TestResult* runTest(void (*testFunc)(TestEnvironment* environment), const char* name);

#endif