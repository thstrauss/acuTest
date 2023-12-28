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

void cuTest_destroy(TestResult* result);
void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const void* actualValue, const void* expectedValue),
	char* (*formatMessage)(int asserted, const char* fileName, int line, const char* message),
	const void* actualValue, const void* expectedValue,
	const char* fileName,
	int line, 
	const char* message);

int cuTest_equalInt(const void* actualValue, const void* expectedValue);

TestResult* cuTest_run(void (*testFunc)(TestEnvironment* environment), const char* name);

#endif