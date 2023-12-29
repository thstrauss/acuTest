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

#define cuTest_PrepareParameter(type, actualValue, expectedValue, messageValue) \
			TestParameter parameter; \
			type actual = (actualValue); \
 			type expected = (expectedValue); \
			parameter.actual = &actual; \
			parameter.expected = &expected; \
			parameter.fileName = __FILE__; \
			parameter.line = __LINE__; \
			parameter.message = (messageValue); 

#define cuTest_assertEqualInt(environment, actualValue, expectedValue, messageValue) { \
			cuTest_PrepareParameter(int, actualValue, expectedValue, messageValue) \
			cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter); \
			};

#define cuTest_assertNotEqualInt(environment, actualValue, expectedValue, messageValue) {\
			cuTest_PrepareParameter(int, actualValue, expectedValue, messageValue) \
			cuTest_assert(environment, &cuTest_notEqualInt, &cuTest_notEqualIntFormatMessage, &parameter); \
			};

#define cuTest_assertPtrEqual(environment, actualValue, expectedValue, message) {\
			cuTest_PrepareParameter(void*, actualValue, expectedValue, messageValue) \
			cuTest_assert(environment, &cuTest_equalPtr, &cuTest_equalPtrFormatMessage, &parameter); \
			};

void cuTest_destroy(TestResult* result);
void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const void* actualValue, const void* expectedValue),
	void (*formatMessage)(char* buffer, int bufferSize, const TestParameter* parameter),
	const TestParameter* parameter);

int cuTest_equalInt(const void* actualValue, const void* expectedValue);
void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_notEqualInt(const void* actualValue, const void* expectedValue);
void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_equalPtr(const void* actualValue, const void* expectedValue);
void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);


TestResult* cuTest_run(void (*testFunc)(TestEnvironment* environment), const char* name);

#endif