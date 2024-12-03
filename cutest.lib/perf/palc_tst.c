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

#include "palc_tst.h"

#include <acu_tryc.h>

#include <acu_perf.h>

#include <acu_allc.h>

#include <stdlib.h>

#include <stdio.h>
#include <time.h>

static void voidFunc(void* context) {
    int i;
    for (i = 0; i < 100; i++) {
    }
    for (i = 0; i < 100; i++) {
    }
    UNUSED(context);
}

typedef struct AllocContext_ {
    char** buffer;
    ACU_StaticAllocator* allocator;
} AllocContext;

static void allocFunc(void* context) {
    int i;
    AllocContext* c = context;
    for (i = 0; i < 100; i++) {
        c->buffer[i] = malloc(10);
    }
    for (i = 0; i < 100; i++) {
        free(c->buffer[i]);
    }
    UNUSED(context);
}

static void acu_allocFunc(void* context) {
    int i;
    AllocContext* c = context;
    for (i = 0; i < 100; i++) {
        c->buffer[i] = acu_allocStaticAllocator(c->allocator);
    }
    for (i = 0; i < 100; i++) {
        acu_freeStaticAllocator(c->buffer[i]);
    }
    UNUSED(context);
}

static void allocPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    char* buffer[100];
    ACU_StaticAllocator allocator;
    AllocContext c;
    c.buffer = &buffer[0];
    c.allocator = &allocator;

    acu_initStaticAllocator(&allocator, 10, 100, (ACU_HandleContextFunc*) NULL, NULL);

#define DIVISOR 3
    printf("voidFunc\t%ld\n\r", (acu_measureLoop(voidFunc, CLK_TCK / DIVISOR, &c)) * DIVISOR);
    printf("allocFunc\t%ld\n\r", (acu_measureLoop(allocFunc, CLK_TCK / DIVISOR, &c)) * DIVISOR);
    printf("acu_allocFunc\t%ld\n\r", (acu_measureLoop(acu_allocFunc, CLK_TCK / DIVISOR, &c)) * DIVISOR);
#undef DIVISOR

    acu_destroyStaticAllocator(&allocator);

    UNUSED(environment);
    UNUSED(context);
}

ACU_Fixture* allocPerformanceFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    acu_initFixture(fixture, "alloc performance Tests");

    acu_addTestCase(fixture, "allocPerformanceTest", allocPerformanceTest);

    return fixture;
}
