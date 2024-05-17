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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_suit.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include <acu_mtch.h>
#include "mtch_tst.h"

static void matchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("123", "0012300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void starMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1*3", "00____300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void notStartMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("^123", "0012300");
    ACU_assert(environment, int, Equal, result, 0, "Does not Match");
    UNUSED(context);
}

static void startMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("^123", "1230000");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void noMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1234", "0012300");
    ACU_assert(environment, int, Equal, result, 0, "Does match");
    UNUSED(context);
}

ACU_Fixture* matchFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "match Tests");
    acu_fixtureAddTestCase(fixture, "Match test", matchTest);
    acu_fixtureAddTestCase(fixture, "Star Match test", starMatchTest);
    acu_fixtureAddTestCase(fixture, "Not Start Match test", notStartMatchTest);
    acu_fixtureAddTestCase(fixture, "Start Match test", startMatchTest);
    acu_fixtureAddTestCase(fixture, "No Match test", noMatchTest);

    return fixture;
}