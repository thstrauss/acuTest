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

#include <stdlib.h>
#include <setjmp.h>
#include <tryctch.h>
#include <acu_asrt.h>
#include <acu_rslt.h>
#include <acu_eenv.h>
#include <acu_util.h>

#define TESTBED(postfix, type, operation, actual, expetced) static void test##type##operation##postfix(ACU_ExecuteEnv* environment, const void* context) { \
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv)); \
    ACU_Result* resultBuf = acuTest_resultMalloc(); \
    testEnvironment->result = resultBuf; \
    acuTest_resultInit(resultBuf); \
    if (!setjmp(testEnvironment->assertBuf)) { \
        ACU_assert(testEnvironment, type, operation, actual, expetced, #type#operation#postfix); \
    } \
    TRY \
    ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, #type#operation#postfix); \
    ACU_assert_strEqual(environment, testEnvironment->result->message, "", #type#operation#postfix); \
    FINALLY \
    acuTest_resultDestroy(resultBuf); \
    free(testEnvironment); \
    ETRY; \
    UNUSED(context); \
}\

#endif
