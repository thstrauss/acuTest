/*
 * Copyright (c) 2025 Thomas Strauss
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

#include "misc_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_perf.h>
#include <acu_strg.h>

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static float func2(float f1, ...) {
    va_list list;
    float f2;
    float f3;
    char* s;
    size_t len;
    va_start(list, f1);

    f2 = (float) va_arg(list, double);
    f3 = (float)va_arg(list, double);
    s = (char*)va_arg(list, char*);

    len = strlen(s);

    va_end(list);

    UNUSED(len);
    UNUSED(f3);
    UNUSED(f1);
    UNUSED(s);

    return f2;
}

static void floatInVaArgsTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, float, Equal, func2(1.0, 42.f, 43.f, "123"), 42.f, "float argument not handled correctly.");
    UNUSED(context);
}

void* buffer;

static void orphanedAlloc(ACU_ExecuteEnv* environment, const void* context) {
    buffer = acu_emalloc(1);
    UNUSED(environment);
    UNUSED(context);
}

static int zbyter(unsigned long x) {
    unsigned long y;

    y = (x & 0x7F7F7F7FL) + 0x7F7F7F7FL;
    y = ~(y | x | 0x7F7F7F7FL);

    if (y == 0) {
        return 4;
    } 
    else if (y & 0x0000FFFFL) {
        return (int) ((y >> 7) & 1) ^ 1;
    }
    else {
        return (int) ((y >> 23) & 1) ^ 3;
    }
}


static int zbytel(unsigned long x) {
    unsigned long y;
    y = (x & 0x7F7F7F7FL) + 0x7F7F7F7FL;
    y = ~(y | x | 0x7F7F7F7FL);
    
    if (y == 0) return 4; 
    else if (y > 0x0000FFFFL) 
        return (int) (y >> 31) ^ 1; 
    else 
        return (int) (y >> 15) ^ 3;
}

static void strlenTest(ACU_ExecuteEnv* environment, const void* context) {

    char s[] = "";

    ACU_assert(environment, size_t, Equal, acu_strlen(s), 0, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("1"), 1, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("12"), 2, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("123"), 3, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("1234"), 4, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("12345"), 5, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("123456"), 6, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("1234567"), 7, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("12345678"), 8, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("123456789"), 9, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("1234567890"), 10, "other than zero")
    ACU_assert(environment, size_t, Equal, acu_strlen("The quick brown fox jumps over the lazy dog"), 43, "other than zero")

    UNUSED(environment);
    UNUSED(context);
}



ACU_Fixture* miscTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "mixed bag of tests");

    acu_addTestCase(fixture, "float In VaArgs are implicit converted to double", floatInVaArgsTest);
    acu_addTestCase(fixture, "orphanedAlloc", orphanedAlloc);
    acu_addTestCase(fixture, "strlenTest", strlenTest);
    return fixture;
}
