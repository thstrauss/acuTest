/*
 * Copyright (c) 2024 Thomas Strauss
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
  \
*
  \
* The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  \
* PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  \
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#ifndef _ACU_ASSERT_H_
#define _ACU_ASSERT_H_

#include <acu_cmmn.h>

/* Function prototype for assert functions */
typedef enum ACU_TestResult (ACU_assertFunc)(const struct ACU_AssertParameter_* parameter);
/* Function prototype for functions to format messages for specific asserts */
typedef char* (ACU_formatMessageFunc)(const struct ACU_AssertParameter_* parameter);

/* Struct holds needed function pointers. */
typedef struct ACU_Funcs_ {
    ACU_assertFunc* assert;
    ACU_formatMessageFunc* formatFailedMessage;
    ACU_formatMessageFunc* formatErrorMessage;
} ACU_Funcs;

/* All parameters to an actual assert. */
typedef struct ACU_AssertParameter_ {
    ACU_Funcs* funcs;
    const void* actual;
    const void* expected;
    const char* message;
    const char* fileName;
    int line;
} ACU_AssertParameter;

/* The assert function which takes the environment and the parameters */
__EXPORT void acu_assert(
    ACU_ExecuteEnv* environment,
    const ACU_AssertParameter* parameter);


#ifndef __ACU_EMIT_ASSERT_FUNCS__
#define CREATE_ASSERT_FUNC(type, op, opCode, format) \
__IMPORT extern ACU_Funcs acu_##type##op##Funcs;
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

/* ACU_PrepareParameter macro defines and fills parameter for the assert call. Filled at runtime. */
#define ACU_PrepareParameter(type, op, actualValue, expectedValue, messageValue) \
            const type __actual = (actualValue); \
            const type __expected = (expectedValue); \
            ACU_AssertParameter parameter; \
            parameter.funcs = &acu_##type##op##Funcs; \
            parameter.actual = &__actual; \
            parameter.expected = &__expected; \
            parameter.message = (messageValue); \
            parameter.fileName = __FILE__;\
            parameter.line = __LINE__;

/* Macro which performs the assert call.
* environment: filled from framework.
* type: the c type name
* op: one out of Equal, NotEqual, Less, Greater, LessEqual, GreaterEqual
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert(environment, type, op, actualValue, expectedValue, messageValue) { \
    ACU_PrepareParameter(type, op, actualValue, expectedValue, messageValue) \
    acu_assert(environment, &parameter); \
}

#ifndef __ACU_EMIT_ASSERT_FUNCS__
__IMPORT extern ACU_Funcs acu_equalPtrFuncs;
__IMPORT extern ACU_Funcs acu_notEqualPtrFuncs;
__IMPORT extern ACU_Funcs acu_equalStrFuncs;
__IMPORT extern ACU_Funcs acu_notEqualStrFuncs;
__IMPORT extern ACU_Funcs acu_containsStrFuncs;
__IMPORT extern ACU_Funcs acu_notContainsStrFuncs;
#endif

#define ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, __assertFunc) \
    ACU_AssertParameter parameter; \
    parameter.funcs = &##__assertFunc##Funcs; \
    parameter.actual = (actualValue); \
    parameter.expected = (expectedValue); \
    parameter.message = (messageValue); \
    parameter.fileName = __FILE__;\
    parameter.line = __LINE__;

/* Macro which performs the assert call.for ptr equal
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_ptrEqual(environment, actualValue, expectedValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, acu_equalPtr) \
    acu_assert(environment, &parameter); \
}

/* Macro which performs the assert call.for ptr is NULL
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_ptrIsNull(environment, actualValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, NULL, messageValue, acu_equalPtr) \
    acu_assert(environment, &parameter); \
}

/* Macro which performs the assert call.for ptr not equal
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_ptrNotEqual(environment, actualValue, expectedValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, expectedValue, messageValue, acu_notEqualPtr) \
    acu_assert(environment, &parameter); \
}

/* Macro which performs the assert call.for ptr is not NULL
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_ptrIsNotNull(environment, actualValue, messageValue) {\
    ACU_PtrPrepareParameter(type, actualValue, NULL, messageValue, acu_notEqualPtr) \
    acu_assert(environment, &parameter); \
}

#define __ACU_assert_str(environment, actualValue, expectedValue, messageValue, assertFunc) { \
    ACU_AssertParameter parameter; \
    parameter.funcs = &##assertFunc##Funcs; \
    parameter.actual = (actualValue); \
    parameter.expected = (expectedValue); \
    parameter.message = (messageValue); \
    parameter.fileName = __FILE__; \
    parameter.line = __LINE__; \
    acu_assert(environment, &parameter); \
}

/* Macro which performs the assert call.for string actual value is equal to expected value.
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strEqual(environment, actualValue, expectedValue, messageValue) \
   __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_equalStr)

/* Macro which performs the assert call.for string is empty
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strIsEmpty(environment, actualValue, messageValue) \
   __ACU_assert_str(environment, actualValue, "", messageValue, acu_equalStr)

/* Macro which performs the assert call.for string is not empty
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strIsNotEmpty(environment, actualValue, messageValue) \
   __ACU_assert_str(environment, actualValue, "", messageValue, acu_notEqualStr)

/* Macro which performs the assert call.for string actual value is not equal to expected value
* environment: filled from framework.
* actualValue: rvalue with value of type
* expectedValue: rvalue with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strNotEqual(environment, actualValue, expectedValue, messageValue) \
  \
   __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_notEqualStr)

/* Macro which performs the assert call.for string actual value contains expected value 
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strContains(environment, actualValue, expectedValue, messageValue) \
   __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_containsStr)

/* Macro which performs the assert call.for string actual value does not contain expected value 
* environment: filled from framework.
* actualValue: r-value with value of type
* expectedValue: r-value with value of type
* messageValue: Describes the assert.
*/
#define ACU_assert_strNotContains(environment, actualValue, expectedValue, messageValue) \
   __ACU_assert_str(environment, actualValue, expectedValue, messageValue, acu_notContainsStr)

#endif