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
#include <acu_tryc.h>
#include <acu_list.h>

#include <acu_mtch.h>
#include "mtch_tst.h"

static void matchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("123", "0012300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void matchAnyTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1.3", "0012300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void plusMatchAnyTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1.+3", "0012567300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void matchAnyManyTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1....3", "0012222300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void starMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("12*3", "01222300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void starMatchNoTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("12*3", "01300");
    ACU_assert(environment, int, Equal, result, 1, "Does Match");
    UNUSED(context);
}

static void starMatchFailsTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("12*3", "0001300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void plusMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("12+3", "00012222300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void plusMatchFailTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("12+3", "0001300");
    ACU_assert(environment, int, Equal, result, 0, "Does Match");
    UNUSED(context);
}

static void queryMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("012?3", "012300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void queryMatchWrongTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("012?3", "01300");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void notStartMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("^123", "0012300");
    ACU_assert(environment, int, Equal, result, 0, "Does Match");
    UNUSED(context);
}

static void startMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("^123", "1230000");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void endMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("123$", "000123");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void escapeMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("\\$", "000$123");
    ACU_assert(environment, int, Equal, result, 1, "Does not Match");
    UNUSED(context);
}

static void noEndMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("123$", "1230000");
    ACU_assert(environment, int, Equal, result, 0, "Does Match");
    UNUSED(context);
}

static void noMatchTest(ACU_ExecuteEnv* environment, const void* context) {
    int result = acu_match("1234", "0012300");
    ACU_assert(environment, int, Equal, result, 0, "Does match");
    UNUSED(context);
}

ACU_Fixture* matchTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "match Tests");
    acu_addTestCase(fixture, "Match test", matchTest);
    acu_addTestCase(fixture, "matchAnyTest", matchAnyTest);
    acu_addTestCase(fixture, "plusMatchAnyTest", plusMatchAnyTest);
    acu_addTestCase(fixture, "matchAnyManyTest", matchAnyManyTest);
    acu_addTestCase(fixture, "Star Match test", starMatchTest);
    acu_addTestCase(fixture, "plusMatchTest", plusMatchTest);
    acu_addTestCase(fixture, "plusMatchFailTest", plusMatchFailTest);
    acu_addTestCase(fixture, "queryMatchTest", queryMatchTest);
    acu_addTestCase(fixture, "queryMatchWrongTest", queryMatchWrongTest);
    acu_addTestCase(fixture, "starMatchNoTest", starMatchNoTest);
    acu_addTestCase(fixture, "Star Match fails test", starMatchFailsTest);
    acu_addTestCase(fixture, "Not Start Match test", notStartMatchTest);
    acu_addTestCase(fixture, "Start Match test", startMatchTest);
    acu_addTestCase(fixture, "No Match test", noMatchTest);
    acu_addTestCase(fixture, "End Match test", endMatchTest);
    acu_addTestCase(fixture, "escapeMatchTest", escapeMatchTest);
    acu_addTestCase(fixture, "No End Match test", noEndMatchTest);

    return fixture;
}