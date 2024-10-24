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
 
#include <stddef.h>
#include <stdlib.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_rslt.h>
#include <acu_tryc.h>
 
static void ptrEqualNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv testEnvironment;
    ACU_Result resultBuf;
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_pushStack(frameStack, &frame);
    testEnvironment.exceptionFrame = &frame;

    testEnvironment.result = &resultBuf;
    resultBuf.status = ACU_TEST_PASSED;
    resultBuf.message = NULL;
    resultBuf.sourceFileName = NULL;

    if (!setjmp(testEnvironment.exceptionFrame->exceptionBuf)) {
        ACU_assert_ptrEqual(&testEnvironment, NULL, NULL, "ptrEqualNull"); \
    }
    acu_dropStack(frameStack);
    ACU_TRY
        ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_PASSED, "ptrEqualNull"); \
        ACU_assert_ptrEqual(environment, resultBuf.message, NULL, "ptrEqualNull"); \
    ACU_FINALLY
        if (resultBuf.message) {
            acu_free(resultBuf.message);
        }
    ACU_ETRY;
    UNUSED(context);
}

static void ptrEqual(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv testEnvironment;
    ACU_Result resultBuf;
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_pushStack(frameStack, &frame);
    testEnvironment.exceptionFrame = &frame;

    testEnvironment.result = &resultBuf;
    resultBuf.status = ACU_TEST_PASSED;
    resultBuf.message = NULL;
    resultBuf.sourceFileName = NULL;
    
    if (!setjmp(testEnvironment.exceptionFrame->exceptionBuf)) {
        void* ptr1 = (void*)123;
        void* ptr2 = (void*)123;

        ACU_assert_ptrEqual(&testEnvironment, ptr1, ptr2, "ptrEqual");

        UNUSED(ptr1);
        UNUSED(ptr2);
    }
    acu_dropStack(frameStack);
    ACU_TRY
        ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_PASSED, "ptrEqual"); \
        ACU_assert_ptrEqual(environment, resultBuf.message, NULL, "ptrEqual"); \
    ACU_FINALLY
        if (resultBuf.message) {
            acu_free(resultBuf.message);
        }
    ACU_ETRY;
    UNUSED(context);
}

static void ptrNotEqual(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv testEnvironment;
    ACU_Result resultBuf;
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_pushStack(frameStack, &frame);
    testEnvironment.exceptionFrame = &frame;

    testEnvironment.result = &resultBuf;
    resultBuf.status = ACU_TEST_PASSED;
    resultBuf.message = NULL;
    resultBuf.sourceFileName = NULL;
    
    if (!setjmp(testEnvironment.exceptionFrame->exceptionBuf)) {
        void* ptr1 = (void*)123;
        void* ptr2 = (void*)124;
        ACU_assert_ptrNotEqual(&testEnvironment, ptr1, ptr2, "ptrNotEqual");
        UNUSED(ptr1);
        UNUSED(ptr2);
    }
    acu_dropStack(frameStack);
    ACU_TRY
        ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_PASSED, "ptrNotEqual"); \
        ACU_assert_ptrEqual(environment, resultBuf.message, NULL, "ptrNotEqual"); \
    ACU_FINALLY
        if (resultBuf.message) {
            acu_free(resultBuf.message);
        }
    ACU_ETRY;
    UNUSED(context);
}

static void ptrNotEqualNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv testEnvironment;
    ACU_Result resultBuf;
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_pushStack(frameStack, &frame);
    testEnvironment.exceptionFrame = &frame;

    testEnvironment.result = &resultBuf;
    resultBuf.status = ACU_TEST_PASSED;
    resultBuf.message = NULL;
    resultBuf.sourceFileName = NULL;
    
    if (!setjmp(testEnvironment.exceptionFrame->exceptionBuf)) {
        void* ptr1 = (void*)123;
        void* ptr2 = NULL;
        ACU_assert_ptrNotEqual(&testEnvironment, ptr1, ptr2, "ptrNotEqualNull");
        UNUSED(ptr1);
        UNUSED(ptr2);
    }
    acu_dropStack(frameStack);
    ACU_TRY
        ACU_assert(environment, int, Equal, resultBuf.status, ACU_TEST_PASSED, "ptrNotEqualNull"); \
        ACU_assert_ptrEqual(environment, resultBuf.message, NULL, "ptrNotEqualNull"); \
    ACU_FINALLY
        if (resultBuf.message) {
            acu_free(resultBuf.message);
        }
    ACU_ETRY;
    UNUSED(context);
}  
 
ACU_Fixture* ptrFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "ptr tests");
    
    acu_addTestCase(fixture, "ptr Equal NULL", ptrEqualNull);
    acu_addTestCase(fixture, "ptr Equal", ptrEqual);
    acu_addTestCase(fixture, "ptr Not Equal", ptrNotEqual);
    acu_addTestCase(fixture, "ptr Not Equal NULL", ptrNotEqualNull);

    return fixture;
}
