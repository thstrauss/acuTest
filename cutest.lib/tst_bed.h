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
#ifndef __test_bed__
#define __test_bed__

#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <acu_asrt.h>
#include <acu_rslt.h>
#include <acu_eenv.h>
#include <acu_util.h>
#include <acu_tryc.h>

#define STR(x) #x
#define TESTBED(postfix, type, operation, actual, expected) static void test##type##operation##postfix(ACU_ExecuteEnv* environment, const void* context) { \
    ACU_ExecuteEnv testEnvironment; \
    ACU_Result resultBuf; \
    ACU_Frame frame; \
    testEnvironment.result = &resultBuf; \
    testEnvironment.exceptionFrame = &frame; \
    acuTest_resultInit(&resultBuf); \
    acu_stackPush(acu_getFrameStack(), &frame); \
    if (!setjmp(frame.exceptionBuf)) { \
        ACU_assert(&testEnvironment, type, operation, actual, expected, #type#operation#postfix); \
    } \
    acu_stackDrop(acu_getFrameStack()); \
    ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_PASSED, STR(notPassed_)#type#operation#postfix); \
    ACU_assert_ptrIsNull(environment, resultBuf.message, STR(messageIsNotNull_)#type#operation#postfix); \
    UNUSED(context); \
}

#define TESTBED_FAILED(postfix, type, operation, actual, expected) static void testFailed##type##operation##postfix(ACU_ExecuteEnv* environment, const void* context) { \
    ACU_ExecuteEnv testEnvironment; \
    ACU_Result resultBuf; \
    ACU_Frame frame; \
    testEnvironment.result = &resultBuf; \
    testEnvironment.exceptionFrame = &frame; \
    acuTest_resultInit(&resultBuf); \
    acu_stackPush(acu_getFrameStack(), &frame); \
    if (!setjmp(frame.exceptionBuf)) { \
        ACU_assert(&testEnvironment, type, operation, actual, expected, #type#operation#postfix); \
    } \
    acu_stackDrop(acu_getFrameStack()); \
    ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_FAILED, STR(passed_)#type#operation#postfix); \
    ACU_assert_ptrIsNotNull(environment, resultBuf.message, STR(messageIsNotNull_)#type#operation#postfix); \
    acu_free(resultBuf.message); \
    UNUSED(context); \
}

#endif
