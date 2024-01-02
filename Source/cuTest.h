#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include "try_catch_throw.h"
#include "list.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

#define CU_TEST_PASSED 0
#define CU_TEST_FAILED 1
#define CU_TEST_ERROR  2

typedef struct TestResult_ {
	int result;
	char* message;
	
} TestResult;

typedef struct TestEnvironment_ {
	jmp_buf assertBuf;
	TestResult* result;
} TestEnvironment;

typedef struct AssertParameter_ {
	void* actual;
	void* expected;
	const char* fileName;
	int line;
	char* message;
} AssertParameter;

typedef struct TestCase_ {
	char* name;
	void (*testFunc)(TestEnvironment* environment);
	TestResult* result;
} TestCase;

typedef struct TestFixture_ {
	const char* name;
	List* testCases;
} TestFixture;

#define cuTest_PrepareParameter(type, actualValue, expectedValue, messageValue, lineValue) \
			AssertParameter parameter; \
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
				AssertParameter parameter; \
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
				AssertParameter parameter; \
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

void cuTest_assert(
	TestEnvironment* environment, 
	int (*assertFunc)(const AssertParameter* parameter),
	void (*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter),
	const AssertParameter* parameter);

int cuTest_equalInt(const AssertParameter* parameter);
void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cuTest_notEqualInt(const AssertParameter* parameter);
void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cuTest_equalPtr(const AssertParameter* parameter);
void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cuTest_notEqualPtr(const AssertParameter* parameter);
void cuTest_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cuTest_equalStr(const AssertParameter* parameter);
void cuTest_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cuTest_notEqualStr(const AssertParameter* parameter);
void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

void cuTest_addTestCase(TestFixture* fixture, const char *name, void (*testFunc)(TestEnvironment* environment));

void cuTest_init(TestFixture* fixture, const char* name);

void cuTest_execute(TestFixture* fixture);

int cuTest_report(TestFixture* fixture);

void cuTest_destroy(TestFixture* fixture);

#endif