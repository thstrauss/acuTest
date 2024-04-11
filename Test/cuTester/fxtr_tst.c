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

#include <stdlib.h>

#include <acu_eenv.h>
#include <acu_asrt.h>
#include <acu_tcse.h> 
#include <acu_suit.h>
#include <acu_util.h>

#include "fxtr_tst.h"

static void test1(ACU_ExecuteEnv* environment, const void* context) {
    ACU_PrepareParameter(int, 1, 2, "assert");
    environment->result->file = acu_estrdup(__FILE__);
    environment->result->line = __LINE__ + 1;
    acu_assert_intEqual(environment, &parameter);
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

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    return fixture;
}

static void progress(const ACU_TestCase* testCase) {
    printf("%s", testCase->result->status == ACU_TEST_PASSED ? "#" : "f");
}

static void failingFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = acu_suiteMalloc();

    acu_suiteInit(localSuite, "testSuite");
    acu_suiteAddFixture(localSuite, failingFixture());
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_FAILED, "assert2");
    UNUSED(context);
}

static void passingFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = acu_suiteMalloc();

    acu_suiteInit(localSuite, "testSuite");
    acu_suiteAddFixture(localSuite, passingFixture());
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_PASSED, "assert2");
    UNUSED(context);
}

static void emptyFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = acu_suiteMalloc();

    acu_suiteInit(localSuite, "testSuite");
    acu_suiteAddFixture(localSuite, emptyFixture());
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_PASSED, "assert2");
    UNUSED(context);
}

ACU_Fixture* fixtureFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    acu_fixtureInit(fixture, "fixture tests");
    acu_fixtureAddTestCase(fixture, "Failing Fixture", failingFixtureTest);
    acu_fixtureAddTestCase(fixture, "Passing Fixture", passingFixtureTest);
    acu_fixtureAddTestCase(fixture, "Empty Fixture", emptyFixtureTest);
    return fixture;
}
