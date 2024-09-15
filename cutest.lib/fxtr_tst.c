/*
 * Copyright (c) 2024 Thomas Strauß
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

#include <stdio.h>
#include <stdlib.h>

#include <acu_cmmn.h>
#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>

#include "fxtr_tst.h"

static void test1(ACU_ExecuteEnv* environment, const void* context) {
    ACU_PrepareParameter(int, Equal, 1, 2, "assert");
    environment->result->sourceLine = __LINE__ + 1;
    acu_assert(environment, &parameter);
    UNUSED(context);
}

static void test2(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 1, 1, "assert2");
    ACU_assert(environment, int, NotEqual, 1, 2, "xxx");
    ACU_assert(environment, float, NotEqual, 1.0, 2.0, "yyy");
    UNUSED(context);
}

static void test3(ACU_ExecuteEnv* environment, const void* context) {
    char str[] = "abc";
    ACU_assert_strEqual(environment, str, str, "assert2");
    ACU_assert_strEqual(environment, "abc", "abc", "assert2");
    ACU_assert_strEqual(environment, (char*)context, "context", "assert context");
    ACU_assert_strNotEqual(environment, "str", "str", "assert2");
}

static void noAssertTest(ACU_ExecuteEnv* environment, const void* context) {
    UNUSED(environment);
    UNUSED(context);
}

static ACU_Fixture* failingFixture(void) {
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_fixtureAddTestCase(fixture, "test1", test1);
    acu_fixtureAddTestCase(fixture, "test2", test2);
    acu_fixtureAddTestCase(fixture, "test3", test3);
    return fixture;
}

static ACU_Fixture* passingFixture(void) {
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_fixtureAddTestCase(fixture, "test2", test2);
    return fixture;
}

static ACU_Fixture* emptyFixture(void) {
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "emptyFixture");

    return fixture;
}

static ACU_Fixture* fixtureWithTestWitNoAssert(void) {
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "emptyFixture");

    acu_fixtureAddTestCase(fixture, "no assert", noAssertTest);

    return fixture;
}

static void localProgress(const ACU_TestCase* testCase, void* progressContext) {
    char buffer[2];
    acu_printf_s(buffer, sizeof(buffer), "%s", testCase->result.status == ACU_TEST_PASSED ? "#" : "f");
    *(int*)progressContext += testCase->result.status == ACU_TEST_PASSED ? 0 : 1;
}

static void failingFixtureInSuiteTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int suiteResult;
    ACU_Fixture* localSuite = acu_fixtureMalloc();

    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    acu_fixtureInit(localSuite, "testSuite");
    acu_fixtureAddChildFixture(localSuite, failingFixture());
    suiteResult = acu_fixtureExecute(localSuite, &progress);
    acu_fixtureDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, 2, "Number of failing tests");
    ACU_assert(environment, int, Equal, suiteResult, ACU_TEST_FAILED, "suite does not return ACU_TEST_FAILED");

    UNUSED(context);
}

static void failingFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int fixtureResult;
    
    ACU_Fixture* fixture = failingFixture();

    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    fixtureResult = acu_fixtureExecute(fixture, &progress);

    acu_fixtureDestroy(fixture);
    
    ACU_assert(environment, int, Equal, result, 2, "Number of failing tests");
    ACU_assert(environment, int, Equal, fixtureResult, ACU_TEST_FAILED, "fixture does not return ACU_TEST_FAILED");

    UNUSED(context);
}

static void passingFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int suiteResult;
    ACU_Fixture* localSuite = acu_fixtureMalloc();
    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    acu_fixtureInit(localSuite, "testSuite");
    acu_fixtureAddChildFixture(localSuite, passingFixture());
    suiteResult = acu_fixtureExecute(localSuite, &progress);
    acu_fixtureDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, 0, "Number of failing tests");
    ACU_assert(environment, int, Equal, suiteResult, ACU_TEST_PASSED, "passingFixtureTest");

    UNUSED(context);
}

static void emptyFixtureInSuiteTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int suiteResult;
    ACU_Fixture* localSuite = acu_fixtureMalloc();
    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    acu_fixtureInit(localSuite, "testSuite");
    acu_fixtureAddChildFixture(localSuite, emptyFixture());
    suiteResult = acu_fixtureExecute(localSuite, &progress);
    acu_fixtureDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, 0, "Number of failing tests");
    ACU_assert(environment, int, Equal, suiteResult, ACU_TEST_PASSED, "Suite does not return ACU_TEST_PASSED");
    UNUSED(context);
}

static void emptyFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int fixtureResult;

    ACU_Fixture* fixture = emptyFixture();
    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    fixtureResult = acu_fixtureExecute(fixture, &progress);
    acu_fixtureDestroy(fixture);

    ACU_assert(environment, int, Equal, result, 0, "Number of failing tests");
    ACU_assert(environment, int, Equal, fixtureResult, ACU_TEST_PASSED, "Suite does not return ACU_TEST_PASSED");
    UNUSED(context);
}

static void noAssertFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = 0;
    int fixtureResult;

    ACU_Fixture* fixture = fixtureWithTestWitNoAssert();
    ACU_Progress progress;
    progress.progress = localProgress;
    progress.context = &result;

    fixtureResult = acu_fixtureExecute(fixture, &progress);
    acu_fixtureDestroy(fixture);

    ACU_assert(environment, int, Equal, result, 0, "Number of failing tests");
    ACU_assert(environment, int, Equal, fixtureResult, ACU_TEST_PASSED, "Suite does not return ACU_TEST_PASSED");
    UNUSED(context);
}

ACU_Fixture* fixtureFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    acu_fixtureInit(fixture, "fixture tests");

    acu_fixtureAddTestCase(fixture, "Failing Fixture", failingFixtureTest);
    acu_fixtureAddTestCase(fixture, "Failing Fixture InSuite", failingFixtureInSuiteTest);
    acu_fixtureAddTestCase(fixture, "Passing Fixture", passingFixtureTest);
    acu_fixtureAddTestCase(fixture, "Empty Fixture in Suite", emptyFixtureInSuiteTest);
    acu_fixtureAddTestCase(fixture, "Empty Fixture xxx", emptyFixtureTest);
    acu_fixtureAddTestCase(fixture, "fixtureWithTestWitNoAssert", noAssertFixtureTest);
    return fixture;
}
