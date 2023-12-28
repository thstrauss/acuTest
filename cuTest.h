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

typedef struct TestParameter_ {
	void* actual;
	void* expected;
	const char* fileName;
	int line;
	char* message;
} TestParameter;

void cuTest_destroy(TestResult* result);
void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const void* actualValue, const void* expectedValue),
	void (*formatMessage)(char* buffer, int bufferSize, const TestParameter* parameter),
	const TestParameter* parameter);

int cuTest_equalInt(const void* actualValue, const void* expectedValue);
void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

TestResult* cuTest_run(void (*testFunc)(TestEnvironment* environment), const char* name);

#endif