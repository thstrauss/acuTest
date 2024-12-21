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

#include "pmsc_tst.h"

#include <string.h>
#include <acu_util.h>
#include <acu_strg.h>
#include <acu_perf.h>
#include <acu_tryc.h>
#include <stdio.h>

static void voidFunc(void* context) {
    int i;
    for (i = 0; i < 100; i++) {
    }
    UNUSED(context);
}

static void strLenFunc(void* context) {
    int i;
    for (i = 0; i < 100; i++) {
        size_t r = strlen("The quick brown fox jumps over the lazy dog");
        UNUSED(r);
    }
    UNUSED(context);
}

static void acu_strlenFunc(void* context) {
    int i;
    for (i = 0; i < 100; i++) {
        size_t r = acu_strlen("The quick brown fox jumps over the lazy dog");
    	UNUSED(r);
    }
    UNUSED(context);
}

static void strlenPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

#define DIVISOR 3
    printf("voidFunc\t%ld\n\r", (acu_measureLoop(voidFunc, CLK_TCK / DIVISOR, 0, NULL)) * DIVISOR);
    printf("strLenFunc\t%ld\n\r", (acu_measureLoop(strLenFunc, CLK_TCK / DIVISOR, 0, NULL)) * DIVISOR);
    printf("acu_strlenFunc\t%ld\n\r", (acu_measureLoop(acu_strlenFunc, CLK_TCK / DIVISOR, 0, NULL)) * DIVISOR);
#undef DIVISOR

    UNUSED(environment);
    UNUSED(context);
}

ACU_Fixture* miscPerformanceFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    acu_initFixture(fixture, "misc performance Tests");

    acu_addTestCase(fixture, "strlenPerformanceTest", strlenPerformanceTest);
    return fixture;
}
