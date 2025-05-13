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

#include "rslt_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_stck.h>
#include <acu_tryc.h>

#include <acu_rslt.h>

static void resultFailedTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TestResult result = acuTest_calcResult(ACU_TEST_PASSED, ACU_TEST_FAILED);
    ACU_assert(environment, int, Equal, result, ACU_TEST_FAILED, "Failed");
    UNUSED(context);
}

static void resultPassedInitialFailedTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TestResult result = acuTest_calcResult(ACU_TEST_FAILED, ACU_TEST_PASSED);
    ACU_assert(environment, int, Equal, result, ACU_TEST_FAILED, "Failed");
    UNUSED(context);
}

static void resultErrorTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TestResult result = acuTest_calcResult(ACU_TEST_PASSED, ACU_TEST_ERROR);
    ACU_assert(environment, int, Equal, result, ACU_TEST_ERROR, "Error");
    UNUSED(context);
}

static void resultPassedInitialErrorTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TestResult result = acuTest_calcResult(ACU_TEST_ERROR, ACU_TEST_PASSED);
    ACU_assert(environment, int, Equal, result, ACU_TEST_ERROR, "Error");
    UNUSED(context);
}

static void resultFailedInitialErrorTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TestResult result = acuTest_calcResult(ACU_TEST_FAILED, ACU_TEST_ERROR);
    ACU_assert(environment, int, Equal, result, ACU_TEST_ERROR, "Error");
    UNUSED(context);
}



ACU_Fixture* resultFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "result tests");

    acu_addTestCase(fixture, "result aggregation Test", resultFailedTest);
    acu_addTestCase(fixture, "result aggregation Test", resultErrorTest);
    acu_addTestCase(fixture, "result aggregation Test", resultPassedInitialFailedTest);
    acu_addTestCase(fixture, "result aggregation Test", resultPassedInitialErrorTest);
    acu_addTestCase(fixture, "result aggregation Test", resultFailedInitialErrorTest);


    return fixture;
}