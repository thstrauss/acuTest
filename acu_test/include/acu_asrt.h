/*
 * Copyright (c) 2024 Thomas Strauﬂ
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall 
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#ifndef _ACU_ASSERT_H_
#define _ACU_ASSERT_H_

#include <stdlib.h>
#include <string.h>

#include <acu_cmmn.h>
#include <tryctch.h>

typedef struct ACU_AssertParameter_ {
    void* actual;
    void* expected;
    char* message;
} ACU_AssertParameter;

__EXPORT void acu_assert(
    ACU_ExecuteEnv* environment,
    int (*assertFunc)(const ACU_AssertParameter* parameter),
    void (*formatMessage)(char* buffer, int bufferSize, const ACU_AssertParameter* parameter),
    const ACU_AssertParameter* parameter);

#define ACU_PrepareParameter(type, actualValue, expectedValue, messageValue) \
            ACU_AssertParameter parameter; \
            type __actual = (actualValue); \
            type __expected = (expectedValue); \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.message = (messageValue); 

#ifdef __ACU_EMIT_ASSERT_FUNCS__
#define CREATE_ASSERT_FUNC(type, op, opcode, format) \
static int acu_##type##op(const ACU_AssertParameter* parameter) { \
    return *(type*)parameter->actual opcode *(type*)parameter->expected; \
} \
static void acu_##type##op##FormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) { \
char formatBuffer[128]; memset(formatBuffer, 0, 128); \
acu_sprintf_s(formatBuffer, sizeof(formatBuffer), "actual value %s not %%s to value %s: %%s", #format, #format); \
acu_sprintf_s(buffer, bufferSize, formatBuffer, *(const type*)parameter->actual, #opcode, *(const type*)parameter->expected, parameter->message); \
} \
__EXPORT void acu_assert_##type##op(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) { \
    acu_assert(environment, acu_##type##op, acu_##type##op##FormatMessage, parameter); \
}
#else
#define CREATE_ASSERT_FUNC(type, op, opCode, format) \
void acu_assert_##type##op(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter);
#endif

CREATE_ASSERT_FUNC(char, Equal, ==, %c)
CREATE_ASSERT_FUNC(char, NotEqual, != , %c)
CREATE_ASSERT_FUNC(char, Less, <, %c)
CREATE_ASSERT_FUNC(char, Greater, >, %c)
CREATE_ASSERT_FUNC(char, LessEqual, <=, %c)
CREATE_ASSERT_FUNC(char, GreaterEqual, >= , %c)

CREATE_ASSERT_FUNC(int, Equal, ==, %d)
CREATE_ASSERT_FUNC(int, NotEqual, != , %d)
CREATE_ASSERT_FUNC(int, Less, <, %d)
CREATE_ASSERT_FUNC(int, Greater, >, %d)
CREATE_ASSERT_FUNC(int, LessEqual, <=, %d)
CREATE_ASSERT_FUNC(int, GreaterEqual, >= , %d)

CREATE_ASSERT_FUNC(long, Equal, ==, %ld)
CREATE_ASSERT_FUNC(long, NotEqual, != , %ld)
CREATE_ASSERT_FUNC(long, Less, <, %ld)
CREATE_ASSERT_FUNC(long, Greater, >, %ld)
CREATE_ASSERT_FUNC(long, LessEqual, <=, %ld)
CREATE_ASSERT_FUNC(long, GreaterEqual, >= , %ld)

CREATE_ASSERT_FUNC(float, Equal, ==, %f)
CREATE_ASSERT_FUNC(float, NotEqual, != , % f)
CREATE_ASSERT_FUNC(float, Less, <, %f)
CREATE_ASSERT_FUNC(float, Greater, >, %f)
CREATE_ASSERT_FUNC(float, LessEqual, <=, %f)
CREATE_ASSERT_FUNC(float, GreaterEqual, >= , %f)

CREATE_ASSERT_FUNC(double, Equal, ==, %lf)
CREATE_ASSERT_FUNC(double, NotEqual, != , % lf)
CREATE_ASSERT_FUNC(double, Less, <, %lf)
CREATE_ASSERT_FUNC(double, Greater, >, %lf)
CREATE_ASSERT_FUNC(double, LessEqual, <=, %lf)
CREATE_ASSERT_FUNC(double, GreaterEqual, >= , %lf)

#define ACU_assert(environment, type, op, actualValue, expectedValue, messageValue) { \
            ACU_PrepareParameter(type, actualValue, expectedValue, messageValue) \
            if (environment->result->file != NULL) free(environment->result->file); \
            environment->result->file = acu_estrdup(__FILE__); \
            environment->result->line = __LINE__; \
            acu_assert_##type##op(environment, &parameter); \
            };

#define ACU_assert_ptrEqual(environment, actualValue, expectedValue, messageValue) {\
            ACU_PrepareParameter(void*, actualValue, expectedValue, messageValue) \
            if (environment->result->file != NULL) free(environment->result->file); \
            environment->result->file = acu_estrdup(__FILE__); \
            environment->result->line = __LINE__; \
            acu_assert(environment, acu_equalPtr, acu_equalPtrFormatMessage, &parameter); \
            };

#define ACU_assert_ptrNotEqual(environment, actualValue, expectedValue, messageValue) {\
            ACU_PrepareParameter(void*, actualValue, expectedValue, messageValue) \
            if (environment->result->file != NULL) free(environment->result->file); \
            environment->result->file = acu_estrdup(__FILE__); \
            environment->result->line = __LINE__; \
            acu_assert(environment, acu_notEqualStr, acu_notEqualPtrFormatMessage, &parameter); \
            };

#define __ACU_assert_str(environment, actualValue, expectedValue, messageValue, assertFunc) {\
            char* __actual = NULL; \
            char* __expected = NULL; \
            TRY_CTX(_ACU_assert_str_) \
                ACU_AssertParameter parameter; \
                __actual = acu_estrdup(actualValue); \
                __expected = acu_estrdup(expectedValue); \
                parameter.actual = __actual; \
                parameter.expected = __expected; \
                if (environment->result->file != NULL) free(environment->result->file); \
                environment->result->file = acu_estrdup(__FILE__); \
                environment->result->line = __LINE__; \
                parameter.message = (messageValue); \
                acu_assert(environment, assertFunc, assertFunc##FormatMessage, &parameter); \
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

__EXPORT int acu_equalPtr(const ACU_AssertParameter* parameter);
__EXPORT void acu_equalPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

__EXPORT int acu_notEqualPtr(const ACU_AssertParameter* parameter);
__EXPORT void acu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

__EXPORT int acu_equalStr(const ACU_AssertParameter* parameter);
__EXPORT void acu_equalStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

__EXPORT int acu_notEqualStr(const ACU_AssertParameter* parameter);
__EXPORT void acu_notEqualStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter);

#endif