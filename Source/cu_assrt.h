#pragma once
#ifndef _CU_ASSERT_H_
#define _CU_ASSERT_H_

#include "tryctch.h"
#include <string.h>
#include <stdlib.h>

typedef struct CU_ExecuteEnv_ CU_ExecuteEnv;

typedef struct AssertParameter_ {
	void* actual;
	void* expected;
	const char* fileName;
	int line;
	char* message;
} AssertParameter;

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

void cuTest_assert(
	CU_ExecuteEnv* environment,
	int (*assertFunc)(const AssertParameter* parameter),
	void (*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter),
	const AssertParameter* parameter);

#endif