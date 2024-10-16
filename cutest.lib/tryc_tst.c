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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_rslt.h>
#include <acu_stck.h>
#include <acu_tryc.h>

#include <tryctch.h>

#include "tryc_tst.h"

static void tryETryTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    TRY
        visited = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block not visited");
    UNUSED(context);
}

static void tryCatchNotVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    TRY
        visited = 1;
    CATCH(1)
        catched = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block not visited");
    ACU_assert(environment, int, Equal, catched, 0, "catch not visited");
    UNUSED(context);
}

static void tryCatchVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    TRY
        visited++;
        THROW(1);
        visited++;
    CATCH(1)
        catched = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    UNUSED(context);
}

static void tryCatchFinallyVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    int finally = 0;
    TRY
        THROW(1);
        visited = 1;
    CATCH(1)
        catched = 1;
    FINALLY
        finally = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 0, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    ACU_assert(environment, int, Equal, finally, 1, "finally not visited");
    UNUSED(context);
}

static void visitFinallyTest(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int finally = 0;
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result resultBuf;

    ACU_Frame frame;
    frame.exception = 0;

    acu_pushStack(acu_getFrameStack(), &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = &resultBuf;
    acu_initResult(&resultBuf);

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_TRY
            visited = 1;
        ACU_assert(testEnvironment, int, Equal, 0, 1, "visitFinallyTest");
        visited = 2;
        ACU_FINALLY
            finally = 1;
        ACU_ETRY;
    }
    acu_dropStack(acu_getFrameStack());
    ACU_assert(environment, int, Equal, frame.exception, ACU_EXCEPTION_ABORTED, "failed");
    ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_FAILED, "failed");
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, finally, 1, "finally not visited"); 
    acu_free(testEnvironment);
    acu_destroyResult(&resultBuf);

    UNUSED(context);
}

static void visitFinallyAfterThrowTest(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int finally = 0;
    int catched = 0;
    ACU_TRY
        visited = 1;;
        ACU_THROW(1);
        visited = 2;
    ACU_CATCH(1)
        catched = 1;
    ACU_FINALLY
        finally = 1;
    ACU_ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    ACU_assert(environment, int, Equal, finally, 1, "finally not visited");
    UNUSED(context);
}

static void visitOverlappingThrowTest(ACU_ExecuteEnv* environment, const void* context) {
    volatile int visited = 0;
    volatile int finally = 0;
    volatile int outerFinally = 0;
    volatile int catched = 0;
    ACU_TRY_CTX(outer)
        ACU_TRY
            visited = 1;;
        ACU_THROW_CTX(outer, 12);
        visited = 2;
        ACU_FINALLY
            finally = 1;
        ACU_ETRY;
    ACU_CATCH_CTX(outer, 12)
        catched = 1;
    ACU_FINALLY_CTX(outer)
        outerFinally = 1;
    ACU_ETRY_CTX(outer);
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    ACU_assert(environment, int, Equal, finally, 1, "finally not visited");
    ACU_assert(environment, int, Equal, outerFinally, 1, "outerFinally not visited");
    UNUSED(context);
}

ACU_Fixture* tryCatchFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    
    acu_initFixture(fixture, "try catch finally");

    acu_addTestCase(fixture, "try etry", tryETryTests);
    acu_addTestCase(fixture, "try catch", tryCatchNotVisitedTests);
    acu_addTestCase(fixture, "try throw catch", tryCatchVisitedTests);
    acu_addTestCase(fixture, "try throw catch finally", tryCatchFinallyVisitedTests);

    acu_addTestCase(fixture, "visitFinallyTest", visitFinallyTest);
    acu_addTestCase(fixture, "visitFinallyAfterThrowTest", visitFinallyAfterThrowTest);
    acu_addTestCase(fixture, "visitOverlappingThrowTest", visitOverlappingThrowTest); 

    return fixture;
}
