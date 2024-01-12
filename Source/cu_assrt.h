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

void cu_assert(
    CU_ExecuteEnv* environment,
    int (*assertFunc)(const AssertParameter* parameter),
    void (*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter),
    const AssertParameter* parameter);

#define CU_PrepareParameter(type, actualValue, expectedValue, messageValue, lineValue) \
            AssertParameter parameter; \
            type __actual = (actualValue); \
            type __expected = (expectedValue); \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.fileName = __FILE__; \
            parameter.line = (lineValue); \
            parameter.message = (messageValue); 

#ifdef __emit
#define CREATE_ASSERT_FUNC(name, type, op, format) \
int cu_##name(const AssertParameter* parameter) { \
    return *(const type*)parameter->actual op *(const type*)parameter->expected; \
} \
void cu_##name##FormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) { \
char formatBuffer[128]; \
sprintf_s(formatBuffer, sizeof(formatBuffer), "%%s:%%d -> actual value %s not %%s to expected value %s: %%s", #format, #format); \
sprintf_s(buffer, bufferSize, formatBuffer, parameter->fileName, parameter->line, *(const type*)parameter->actual, #op, *(const type*)parameter->expected, parameter->message); \
}
#else
#define CREATE_ASSERT_FUNC(name, type, op, format) \
int cu_##name(const AssertParameter* parameter); \
void cu_##name##FormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter); 
#endif

CREATE_ASSERT_FUNC(equalInt, int, ==, %d )
CREATE_ASSERT_FUNC(lessInt, int, <, %d)
CREATE_ASSERT_FUNC(greaterInt, int, >, %d )
CREATE_ASSERT_FUNC(notEqualInt, int, != , %d)
CREATE_ASSERT_FUNC(equalfloat, float, ==, %f )
CREATE_ASSERT_FUNC(equaldouble, double, ==, %lf )

#define CU_assertEqualInt(environment, actualValue, expectedValue, messageValue) { \
            CU_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_equalInt, &cu_equalIntFormatMessage, &parameter); \
            };

#define CU_assertNotEqualInt(environment, actualValue, expectedValue, messageValue) {\
            CU_PrepareParameter(int, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_notEqualInt, &cu_notEqualIntFormatMessage, &parameter); \
            };

#define CU_assertEqualPtr(environment, actualValue, expectedValue, messageValue) {\
            CU_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_equalPtr, &cu_equalPtrFormatMessage, &parameter); \
            };

#define CU_assertNotEqualPtr(environment, actualValue, expectedValue, messageValue) {\
            CU_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            cu_assert(environment, &cu_notEqualStr, &cu_notEqualPtrFormatMessage, &parameter); \
            };

#define __CU_assertStr(environment, actualValue, expectedValue, messageValue, assertFunc) {\
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
                cu_assert(environment, &assertFunc, &assertFunc##FormatMessage, &parameter); \
            FINALLY \
                free(__expected); \
                free(__actual); \
            ETRY; \
            };

#define CU_assertEqualStr(environment, actualValue, expectedValue, messageValue) {\
                __CU_assertStr(environment, actualValue, expectedValue, messageValue, cu_equalStr) \
            };

#define CU_assertNotEqualStr(environment, actualValue, expectedValue, messageValue) {\
                __CU_assertStr(environment, actualValue, expectedValue, messageValue, cu_notEqualStr) \
            };

int cu_equalPtr(const AssertParameter* parameter);
void cu_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_notEqualPtr(const AssertParameter* parameter);
void cu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_equalStr(const AssertParameter* parameter);
void cu_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

int cu_notEqualStr(const AssertParameter* parameter);
void cu_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter);

#endif