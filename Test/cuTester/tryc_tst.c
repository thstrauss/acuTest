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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>

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

static void tryCatchVisitedExpandedTests(ACU_ExecuteEnv* environment, const void* context) { 
    int visited = 0;
    int catched = 0;
    do {
        jmp_buf _exception_Buf; switch (setjmp(_exception_Buf)) {
        case 0: while (1) {
            visited++;
            longjmp(_exception_Buf, (1));
            visited++;
        break; case (1):
            catched = 1;
            break;
        }
        }
    } while (0);
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

ACU_Fixture* tryCatchFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    
    acu_fixtureInit(fixture, "try catch finally");

    acu_fixtureAddTestCase(fixture, "try etry", tryETryTests);
    acu_fixtureAddTestCase(fixture, "try catch", tryCatchNotVisitedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch", tryCatchVisitedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch_ expanded", tryCatchVisitedExpandedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch finally", tryCatchFinallyVisitedTests);

    return fixture;
}
