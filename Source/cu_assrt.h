#pragma once
#ifndef _CU_ASSERT_H_
#define _CU_ASSERT_H_

#include <string.h>
#include <stdlib.h>

#include "cu_utils.h"
#include "tryctch.h"

typedef struct CU_ExecuteEnv_ CU_ExecuteEnv;

typedef struct AssertParameter_ {
    void* actual;
    void* expected;
    const char* fileName;
    int line;
    char* message;
} AssertParameter;

#define cu_PrepareParameter(type, actualValue, expectedValue, messageValue, lineValue) \
            AssertParameter parameter; \
            type __actual = (actualValue); \
            type __expected = (expectedValue); \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.fileName = __FILE__; \
            parameter.line = (lineValue); \
            parameter.message = (messageValue); 

#define cu_assertEqualInt(environment, actualValue, expectedValue, messageValue) { \
            cu_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_equalInt, &cu_equalIntFormatMessage, &parameter); \
            };

#define cu_assertNotEqualInt(environment, actualValue, expectedValue, messageValue) {\
            cu_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_notEqualInt, &cu_notEqualIntFormatMessage, &parameter); \
            };

#define cu_assertEqualPtr(environment, actualValue, expectedValue, messageValue) {\
            cu_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_equalPtr, &cu_equalPtrFormatMessage, &parameter); \
            };

#define cu_assertNotEqualPtr(environment, actualValue, expectedValue, messageValue) {\
            cu_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_notEqualStr, &cu_notEqualPtrFormatMessage, &parameter); \
            };

#define cu_assertEqualStr(environment, actualValue, expectedValue, messageValue) {\
            char* __actual = NULL; \
            char* __expected = NULL; \
            TRY \
                AssertParameter parameter; \
                __actual = cu_estrdup(actualValue); \
                __expected = cu_estrdup(expectedValue); \
                parameter.actual = __actual; \
                parameter.expected = __expected; \
                parameter.fileName = __FILE__; \
                parameter.line = __LINE__; \
                parameter.message = (messageValue); \
                cu_assert(environment, &cu_equalStr, &cu_equalStrFormatMessage, &parameter); \
            FINALLY \
                free(__expected); \
                free(__actual); \
            ETRY; \
            };

#define cu_assertNotEqualStr(environment, actualValue, expectedValue, messageValue) {\
            char* __actual = NULL; \
            char* __expected = NULL; \
            TRY \
                AssertParameter parameter; \
                __actual = cu_estrdup(actualValue); \
                __expected = cu_estrdup(expectedValue); \
                parameter.actual = __actual; \
                parameter.expected = __expected; \
                parameter.fileName = __FILE__; \
                parameter.line = __LINE__; \
                parameter.message = (messageValue); \
                cu_assert(environment, &cu_notEqualStr, &cuTest_notEqualStrFormatMessage, &parameter); \
            FINALLY \
                free(__expected); \
                free(__actual); \
            ETRY; \
            };

int cu_equalInt(const AssertParameter* parameter);
void cu_equalIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_notEqualInt(const AssertParameter* parameter);
void cu_notEqualIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_equalPtr(const AssertParameter* parameter);
void cu_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_notEqualPtr(const AssertParameter* parameter);
void cu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_equalStr(const AssertParameter* parameter);
void cu_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_notEqualStr(const AssertParameter* parameter);
void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

void cu_assert(
    CU_ExecuteEnv* environment,
    int (*assertFunc)(const AssertParameter* parameter),
    void (*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter),
    const AssertParameter* parameter);

#endif