#pragma once
#ifndef _ACU_ASSERT_H_
#define _ACU_ASSERT_H_

#include <string.h>

#include <acu_util.h>
#include <tryctch.h>

typedef struct ACU_ExecuteEnv_ ACU_ExecuteEnv;

typedef struct ACU_AssertParameter_ {
    void* actual;
    void* expected;
    const char* fileName;
    int line;
    char* message;
} ACU_AssertParameter;

void acu_assert(
    ACU_ExecuteEnv* environment,
    int (*assertFunc)(const ACU_AssertParameter* parameter),
    void (*formatMessage)(char* buffer, int bufferSize, const ACU_AssertParameter* parameter),
    const ACU_AssertParameter* parameter);

#define ACU_PrepareParameter(type, actualValue, expectedValue, messageValue, lineValue) \
            ACU_AssertParameter parameter; \
            type __actual = (actualValue); \
            type __expected = (expectedValue); \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.fileName = __FILE__; \
            parameter.line = (lineValue); \
            parameter.message = (messageValue); 

#ifdef __ACU_EMIT_ASSERT_FUNCS__
#define CREATE_ASSERT_FUNC(type, op, opcode, format) \
int acu_##type##op(const ACU_AssertParameter* parameter) { \
    return *(const type*)parameter->actual opcode *(const type*)parameter->expected; \
} \
void acu_##type##op##FormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) { \
char formatBuffer[128]; \
acu_sprintf_s(formatBuffer, sizeof(formatBuffer), "%%s:%%d -> actual value %s not %%s to expected value %s: %%s", (format), (format)); \
acu_sprintf_s(buffer, bufferSize, formatBuffer, parameter->fileName, parameter->line, *(const type*)parameter->actual, #opcode, *(const type*)parameter->expected, parameter->message); \
} \
void acu_assert_##type##op(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) { \
    acu_assert(environment, &acu_##type##op, &acu_##type##op##FormatMessage, parameter); \
}
#else
#define CREATE_ASSERT_FUNC(type, op, opCode, format) \
int acu_##type##op(const ACU_AssertParameter* parameter); \
void acu_##type##op##FormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter); \
void acu_assert_##type##op(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter);
#endif

CREATE_ASSERT_FUNC(char, Equal, ==, "%c")
CREATE_ASSERT_FUNC(char, NotEqual, != , "%c")
CREATE_ASSERT_FUNC(char, Less, <, "%c")
CREATE_ASSERT_FUNC(char, Greater, >, "%c")
CREATE_ASSERT_FUNC(char, LessEqual, <=, "%c")
CREATE_ASSERT_FUNC(char, GreaterEqual, >= , "%c")

CREATE_ASSERT_FUNC(int, Equal, ==, "%d")
CREATE_ASSERT_FUNC(int, NotEqual, != , "%d")
CREATE_ASSERT_FUNC(int, Less, <, "%d")
CREATE_ASSERT_FUNC(int, Greater, >, "%d")
CREATE_ASSERT_FUNC(int, LessEqual, <=, "%d")
CREATE_ASSERT_FUNC(int, GreaterEqual, >= , "%d")

CREATE_ASSERT_FUNC(long, Equal, ==, "%d")
CREATE_ASSERT_FUNC(long, NotEqual, != , "%d")
CREATE_ASSERT_FUNC(long, Less, <, "%d")
CREATE_ASSERT_FUNC(long, Greater, >, "%d")
CREATE_ASSERT_FUNC(long, LessEqual, <=, "%d")
CREATE_ASSERT_FUNC(long, GreaterEqual, >= , "%d")

CREATE_ASSERT_FUNC(float, Equal, ==, "%f")
CREATE_ASSERT_FUNC(float, NotEqual, != , "%f")
CREATE_ASSERT_FUNC(float, Less, <, "%f")
CREATE_ASSERT_FUNC(float, Greater, >, "%f")
CREATE_ASSERT_FUNC(float, LessEqual, <=, "%f")
CREATE_ASSERT_FUNC(float, GreaterEqual, >= , "%f")

CREATE_ASSERT_FUNC(double, Equal, ==, "%lf")
CREATE_ASSERT_FUNC(double, NotEqual, == , "%lf")
CREATE_ASSERT_FUNC(double, Less, <, "%lf")
CREATE_ASSERT_FUNC(double, Greater, >, "%lf")
CREATE_ASSERT_FUNC(double, LessEqual, <=, "%lf")
CREATE_ASSERT_FUNC(double, GreaterEqual, >= , "%lf")

#define ACU_assert(environment, type, op, actualValue, expectedValue, messageValue) { \
            ACU_PrepareParameter(type, actualValue, expectedValue, messageValue, __LINE__) \
            acu_assert_##type##op(environment, &parameter); \
            };

#define ACU_assert_ptrEqual(environment, actualValue, expectedValue, messageValue) {\
            ACU_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            acu_assert(environment, &acu_equalPtr, &acu_equalPtrFormatMessage, &parameter); \
            };

#define ACU_assert_ptrNotEqual(environment, actualValue, expectedValue, messageValue) {\
            ACU_PrepareParameter(void*, actualValue, expectedValue, messageValue, __LINE__) \
            acu_assert(environment, &acu_notEqualStr, &acu_notEqualPtrFormatMessage, &parameter); \
            };

#define __ACU_assert_str(environment, actualValue, expectedValue, messageValue, assertFunc) {\
            char* __actual = NULL; \
            char* __expected = NULL; \
            TRY \
                ACU_AssertParameter parameter; \
                __actual = acu_estrdup(actualValue); \
                __expected = acu_estrdup(expectedValue); \
                parameter.actual = __actual; \
                parameter.expected = __expected; \
                parameter.fileName = __FILE__; \
                parameter.line = __LINE__; \
                parameter.message = (messageValue); \
                acu_assert(environment, &assertFunc, &assertFunc##FormatMessage, &parameter); \
            FINALLY \
                free(__expected); \
                free(__actual); \
            ETRY; \
            };

#define ACU_assert_strEqual(environment, actualValue, expectedValue, messageValue) {\
                __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_equalStr) \
            };

#define ACU_assert_strNotEqual(environment, actualValue, expectedValue, messageValue) {\
                __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_notEqualStr) \
            };

int acu_equalPtr(const ACU_AssertParameter* parameter);
void acu_equalPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

int acu_notEqualPtr(const ACU_AssertParameter* parameter);
void acu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

int acu_equalStr(const ACU_AssertParameter* parameter);
void acu_equalStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

int acu_notEqualStr(const ACU_AssertParameter* parameter);
void acu_notEqualStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

#endif