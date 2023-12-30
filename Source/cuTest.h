#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include "try_catch_throw.h"

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

#define cuTest_PrepareParameter(type, actualValue, expectedValue, messageValue, lineValue) \
			TestParameter parameter; \
			type actual = (actualValue); \
 			type expected = (expectedValue); \
			parameter.actual = &actual; \
			parameter.expected = &expected; \
			parameter.fileName = __FILE__; \
			parameter.line = (lineValue); \
			parameter.message = (messageValue); 

#define cuTest_assertEqualInt(environment, actualValue, expectedValue, messageValue) { \
			cuTest_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
			cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter); \
			};

#define cuTest_assertNotEqualInt(environment, actualValue, expectedValue, messageValue) {\
			cuTest_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
			cuTest_assert(environment, &cuTest_notEqualInt, &cuTest_notEqualIntFormatMessage, &parameter); \
			};

#define cuTest_assertEqualPtr(environment, actualValue, expectedValue, messageValue) {\
			cuTest_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
			cuTest_assert(environment, &cuTest_equalPtr, &cuTest_equalPtrFormatMessage, &parameter); \
			};

#define cuTest_assertNotEqualPtr(environment, actualValue, expectedValue, messageValue) {\
			cuTest_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
			cuTest_assert(environment, &cuTest_notEqualStr, &cuTest_notEqualPtrFormatMessage, &parameter); \
			};

#define cuTest_assertEqualStr(environment, actualValue, expectedValue, messageValue) {\
			char* actual = NULL; \
			char* expected = NULL; \
			TRY \
				TestParameter parameter; \
				actual = _strdup(actualValue); \
 				expected = _strdup(expectedValue); \
				parameter.actual = actual; \
				parameter.expected = expected; \
				parameter.fileName = __FILE__; \
				parameter.line = __LINE__; \
				parameter.message = (messageValue); \
				cuTest_assert(environment, &cuTest_equalStr, &cuTest_equalStrFormatMessage, &parameter); \
			FINALLY \
				free(expected); \
				free(actual); \
			ETRY; \
			};

#define cuTest_assertNotEqualStr(environment, actualValue, expectedValue, messageValue) {\
			char* actual = NULL; \
			char* expected = NULL; \
			TRY \
				TestParameter parameter; \
				actual = _strdup(actualValue); \
 				expected = _strdup(expectedValue); \
				parameter.actual = actual; \
				parameter.expected = expected; \
				parameter.fileName = __FILE__; \
				parameter.line = __LINE__; \
				parameter.message = (messageValue); \
				cuTest_assert(environment, &cuTest_notEqualStr, &cuTest_notEqualStrFormatMessage, &parameter); \
			FINALLY \
				free(expected); \
				free(actual); \
			ETRY; \
			};

void cuTest_destroy(TestResult* result);
void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const TestParameter* parameter),
	void (*formatMessage)(char* buffer, int bufferSize, const TestParameter* parameter),
	const TestParameter* parameter);

int cuTest_equalInt(const TestParameter* parameter);
void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_notEqualInt(const TestParameter* parameter);
void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_equalPtr(const TestParameter* parameter);
void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_notEqualPtr(const TestParameter* parameter);
void cuTest_notEqualPtrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_equalStr(const TestParameter* parameter);
void cuTest_equalStrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

int cuTest_notEqualStr(const TestParameter* parameter);
void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const TestParameter* parameter);

TestResult* cuTest_run(void (*testFunc)(TestEnvironment* environment), const char* name);

#endif