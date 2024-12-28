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


#include <stdlib.h>
#include "rprt_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_stck.h>
#include <acu_tryc.h>

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

static void collectTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ReportVisitor collect = {acu_collectTestCases, NULL};
    ACU_TestCases tests;

    ACU_List list;
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initList(&list, (ACU_ListDestroyFunc*) NULL, NULL);

    tests.testCases = &list;
    collect.context = &tests;


    acu_initFixture(fixture, "testFixture");
    acu_setFixtureContext(fixture, "context");

    acu_addTestCase(fixture, "test1", test1);
    acu_addTestCase(fixture, "test2", test2);
    acu_addTestCase(fixture, "test3", test3);

    acu_acceptFixture(fixture, &collect);

    ACU_assert(environment, size_t, Equal, acu_getListSize(&list), 3, "Wrong number of tests");

    acu_destroyList(&list);
    acu_fixtureDestroy(fixture);
    
    UNUSED(context);
}

ACU_Fixture* reportFixture(void) {
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "report tests");
    acu_addTestCase(fixture, "collect", collectTest);
    return fixture;
}