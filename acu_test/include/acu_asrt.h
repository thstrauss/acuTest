/*
 * Copyright (c) 2024 Thomas Strauss
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

typedef enum ACU_TestResult(assertFunc)(const struct ACU_AssertParameter_* parameter);
typedef char* (formatMessageFunc)(const struct ACU_AssertParameter_* parameter);

typedef struct ACU_AssertParameter_ {
    assertFunc* assert;
    formatMessageFunc* formatFailedMessage;
    formatMessageFunc* formatErrorMessage;
    const void* actual;
    const void* expected;
    const char* message;
    const char* fileName;
    int line;
} ACU_AssertParameter;

__EXPORT void acu_assert(
    ACU_ExecuteEnv* environment,
    const ACU_AssertParameter* parameter);

__EXPORT char* acu_sformatMessage(const char* format, ...);


#ifdef __ACU_EMIT_ASSERT_FUNCS__
#define STR(str) #str
#define CREATE_ASSERT_FUNC(type, op, opcode, format) \
static enum ACU_TestResult acu_##type##op(const ACU_AssertParameter* parameter) { \
    return *(type*)parameter->actual opcode *(type*)parameter->expected; \
} \
static char* acu_##type##op##FormatMessage(const ACU_AssertParameter* parameter) { \
    return acu_sformatMessage(STR(actual value format not opcode to format: %s), *(const type*)parameter->actual, *(const type*)parameter->expected, parameter->message); \
} \
__EXPORT void acu_assert_##type##op(ACU_ExecuteEnv* environment, ACU_AssertParameter* parameter) { \
    parameter->assert = acu_##type##op; \
    parameter->formatFailedMessage = acu_##type##op##FormatMessage; \
    acu_assert(environment, parameter); \
}
#else
#define CREATE_ASSERT_FUNC(type, op, opCode, format) \
void acu_assert_##type##op(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter);
#endif

typedef signed char signedChar;

CREATE_ASSERT_FUNC(signedChar, Equal, == , %c)
CREATE_ASSERT_FUNC(signedChar, NotEqual, != , %c)
CREATE_ASSERT_FUNC(signedChar, Less, < , %c)
CREATE_ASSERT_FUNC(signedChar, Greater, > , %c)
CREATE_ASSERT_FUNC(signedChar, LessEqual, <= , %c)
CREATE_ASSERT_FUNC(signedChar, GreaterEqual, >= , %c)

typedef unsigned char unsignedChar;

CREATE_ASSERT_FUNC(unsignedChar, Equal, == , %c)
CREATE_ASSERT_FUNC(unsignedChar, NotEqual, != , %c)
CREATE_ASSERT_FUNC(unsignedChar, Less, < , %c)
CREATE_ASSERT_FUNC(unsignedChar, Greater, > , %c)
CREATE_ASSERT_FUNC(unsignedChar, LessEqual, <= , %c)
CREATE_ASSERT_FUNC(unsignedChar, GreaterEqual, >= , %c)

CREATE_ASSERT_FUNC(int, Equal, ==, %d)
CREATE_ASSERT_FUNC(int, NotEqual, != , %d)
CREATE_ASSERT_FUNC(int, Less, <, %d)
CREATE_ASSERT_FUNC(int, Greater, >, %d)
CREATE_ASSERT_FUNC(int, LessEqual, <=, %d)
CREATE_ASSERT_FUNC(int, GreaterEqual, >= , %d)

typedef unsigned int unsignedInt;

CREATE_ASSERT_FUNC(unsignedInt, Equal, == , %u)
CREATE_ASSERT_FUNC(unsignedInt, NotEqual, != , %u)
CREATE_ASSERT_FUNC(unsignedInt, Less, < , %u)
CREATE_ASSERT_FUNC(unsignedInt, Greater, > , %u)
CREATE_ASSERT_FUNC(unsignedInt, LessEqual, <= , %u)
CREATE_ASSERT_FUNC(unsignedInt, GreaterEqual, >= , %u)

CREATE_ASSERT_FUNC(short, Equal, == , %hd)
CREATE_ASSERT_FUNC(short, NotEqual, != , %hd)
CREATE_ASSERT_FUNC(short, Less, < , %hd)
CREATE_ASSERT_FUNC(short, Greater, > , %hd)
CREATE_ASSERT_FUNC(short, LessEqual, <= , %hd)
CREATE_ASSERT_FUNC(short, GreaterEqual, >= , %hd)

typedef unsigned short unsignedShort;

CREATE_ASSERT_FUNC(unsignedShort, Equal, == , %hu)
CREATE_ASSERT_FUNC(unsignedShort, NotEqual, != , %hu)
CREATE_ASSERT_FUNC(unsignedShort, Less, < , %hu)
CREATE_ASSERT_FUNC(unsignedShort, Greater, > , %hu)
CREATE_ASSERT_FUNC(unsignedShort, LessEqual, <= , %hu)
CREATE_ASSERT_FUNC(unsignedShort, GreaterEqual, >= , %hu)


CREATE_ASSERT_FUNC(long, Equal, == , %ld)
CREATE_ASSERT_FUNC(long, NotEqual, != , %ld)
CREATE_ASSERT_FUNC(long, Less, < , %ld)
CREATE_ASSERT_FUNC(long, Greater, > , %ld)
CREATE_ASSERT_FUNC(long, LessEqual, <= , %ld)
CREATE_ASSERT_FUNC(long, GreaterEqual, >= , %ld)

typedef unsigned long unsignedLong;

CREATE_ASSERT_FUNC(unsignedLong, Equal, ==, %lu)
CREATE_ASSERT_FUNC(unsignedLong, NotEqual, != , %lu)
CREATE_ASSERT_FUNC(unsignedLong, Less, <, %lu)
CREATE_ASSERT_FUNC(unsignedLong, Greater, >, %lu)
CREATE_ASSERT_FUNC(unsignedLong, LessEqual, <=, %lu)
CREATE_ASSERT_FUNC(unsignedLong, GreaterEqual, >= , %lu)

CREATE_ASSERT_FUNC(float, Equal, ==, %f)
CREATE_ASSERT_FUNC(float, NotEqual, != , %f)
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


#define ACU_PrepareParameter(type, actualValue, expectedValue, messageValue) \
            type __actual = (actualValue); \
            type __expected = (expectedValue); \
            ACU_AssertParameter parameter; \
            parameter.formatErrorMessage = NULL; \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.message = (messageValue); \
            parameter.fileName = __FILE__;\
            parameter.line = __LINE__;

#define ACU_assert(environment, type, op, actualValue, expectedValue, messageValue) { \
    ACU_PrepareParameter(type, actualValue, expectedValue, messageValue) \
    acu_assert_##type##op(environment, &parameter); \
};

#define ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, __assertFunc) \
    ACU_AssertParameter parameter; \
    parameter.assert = __assertFunc; \
    parameter.formatFailedMessage = __assertFunc##FailedFormatMessage; \
    parameter.formatErrorMessage = NULL; \
    parameter.actual = (actualValue); \
    parameter.expected = (expectedValue); \
    parameter.message = (messageValue); \
    parameter.fileName = __FILE__;\
    parameter.line = __LINE__;

#define ACU_assert_ptrEqual(environment, actualValue, expectedValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, acu_equalPtr) \
    acu_assert(environment, &parameter); \
};

#define ACU_assert_ptrNotEqual(environment, actualValue, expectedValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, acu_notEqualPtr) \
    acu_assert(environment, &parameter); \
};

#define __ACU_assert_str(environment, actualValue, expectedValue, messageValue, assertFunc) { \
    ACU_AssertParameter parameter; \
    parameter.assert = assertFunc; \
    parameter.actual = (actualValue); \
    parameter.expected = (expectedValue); \
    parameter.message = (messageValue); \
    parameter.formatFailedMessage = assertFunc##FailedFormatMessage; \
    parameter.formatErrorMessage = assertFunc##ErrorFormatMessage; \
    parameter.fileName = __FILE__; \
    parameter.line = __LINE__; \
    acu_assert(environment, &parameter); \
};

#define ACU_assert_strEqual(environment, actualValue, expectedValue, messageValue) {\
    __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_equalStr) \
};

#define ACU_assert_strNotEqual(environment, actualValue, expectedValue, messageValue) {\
    __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_notEqualStr) \
};

#define ACU_assert_strContains(environment, actualValue, expectedValue, messageValue) {\
    __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_containsStr) \
};

#define ACU_assert_strNotContains(environment, actualValue, expectedValue, messageValue) {\
    __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_notContainsStr) \
};

__EXPORT enum ACU_TestResult acu_equalPtr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_equalPtrFailedFormatMessage(const ACU_AssertParameter* parameter);

__EXPORT enum ACU_TestResult acu_notEqualPtr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_notEqualPtrFailedFormatMessage(const ACU_AssertParameter* parameter);

__EXPORT enum ACU_TestResult acu_equalStr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_equalStrFailedFormatMessage(const ACU_AssertParameter* parameter);
__EXPORT char* acu_equalStrErrorFormatMessage(const ACU_AssertParameter* parameter);

__EXPORT enum ACU_TestResult acu_notEqualStr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_notEqualStrFailedFormatMessage(const ACU_AssertParameter* parameter);
__EXPORT char* acu_notEqualStrErrorFormatMessage(const ACU_AssertParameter* parameter);

__EXPORT enum ACU_TestResult acu_containsStr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_containsStrFailedFormatMessage(const ACU_AssertParameter* parameter);
__EXPORT char* acu_containsStrErrorFormatMessage(const ACU_AssertParameter* parameter);

__EXPORT enum ACU_TestResult acu_notContainsStr(const ACU_AssertParameter* parameter);
__EXPORT char* acu_notContainsStrFailedFormatMessage(const ACU_AssertParameter* parameter);
__EXPORT char* acu_notContainsStrErrorFormatMessage(const ACU_AssertParameter* parameter);

#endif