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
#include <acu_dall.h>

#include <stdlib.h>

#include <stdio.h>
#include <time.h>

typedef struct AllocContext_ {
    char** buffer;
    ACU_StaticAllocator* allocator;
} AllocContext;

typedef struct AllocDynamicContext_ {
    char** buffer;
    ACU_DynamicAllocator* allocator;
} AllocDynamicContext;

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


static void acu_emallocFunc(void* context) {
    int i;
    AllocContext* c = context;
    for (i = 0; i < 100; i++) {
        c->buffer[i] = acu_emalloc(10);
    }
    for (i = 0; i < 100; i++) {
        acu_free(c->buffer[i]);
    }
    UNUSED(context);
}

static void allocPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    char* buffer[100];
    AllocContext c;
    c.buffer = buffer;
    c.allocator = NULL;

    printf("\n\r");

#define DIVISOR 3
    printf("allocFunc\t%ld\n\r", (acu_measureLoop(allocFunc, CLK_TCK / DIVISOR, 0, &c)) * DIVISOR);
    printf("acu_emallocFunc\t%ld\n\r", (acu_measureLoop(acu_emallocFunc, CLK_TCK / DIVISOR, 0, &c)) * DIVISOR);
#undef DIVISOR

    UNUSED(environment);
    UNUSED(context);
}

static void acu_allocStaticAllocatorFunc(void* context) {
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

static void staticAllocPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    char* buffer[100];
    ACU_StaticAllocator allocator;
    AllocContext c;
    c.buffer = buffer;
    c.allocator = &allocator;

    acu_initStaticAllocator(&allocator, 10, 100, (ACU_FreeContext*)NULL);

#define DIVISOR 3
    printf("acu_allocStaticAllocator\t%ld\n\r", (acu_measureLoop(acu_allocStaticAllocatorFunc, CLK_TCK / DIVISOR, 0, &c)) * DIVISOR);
#undef DIVISOR

    acu_destroyStaticAllocator(&allocator);

    UNUSED(environment);
    UNUSED(context);
}

static void acu_allocDynamicAllocatorFunc(void* context) {
    int i;
    AllocDynamicContext* c = (AllocDynamicContext*)context;
    for (i = 0; i < 100; i++) {
        c->buffer[i] = acu_allocAllocator(c->allocator);
    }
    for (i = 0; i < 100; i++) {
        acu_freeAllocator(c->buffer[i]);
    }
}

static void dynamicAllocPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    char* buffer[100];
    ACU_DynamicAllocator allocator;
    AllocDynamicContext c;
    c.buffer = buffer;
    c.allocator = &allocator;

    acu_initAllocator(&allocator, 10, 101);
    
#define DIVISOR 3
    printf("acu_allocDynamicAllocator\t%ld\n\r", (acu_measureLoop(acu_allocDynamicAllocatorFunc, CLK_TCK / DIVISOR, 0, &c)) * DIVISOR);
#undef DIVISOR

    acu_destroyAllocator(&allocator);

    UNUSED(environment);
    UNUSED(context);
}

static void acu_randomAllocAllocatorFunc(void* context) {
    int i;
    AllocDynamicContext* c = context;
    char** buffer1 = &(c->buffer[0]);

    for (i = 1; i < 100; i++) {
        acu_freeAllocator(buffer1[0]);
        acu_freeAllocator(buffer1[i]);
        buffer1[i] = acu_allocAllocator(c->allocator);
        buffer1[0] = acu_allocAllocator(c->allocator);
    }
    UNUSED(context);
}

static void randomAllocPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    char* buffer[100];
    ACU_DynamicAllocator allocator;
    AllocDynamicContext c;
    int i;
    c.buffer = buffer;
    c.allocator = &allocator;

    acu_initAllocator(&allocator, 10, 50);

    for (i = 0; i < 100; i++) {
        buffer[i] = acu_allocAllocator(&allocator);
    }

#define DIVISOR 3
    printf("acu_randomAllocAllocatorFunc\t%ld\n\r", (acu_measureLoop(acu_randomAllocAllocatorFunc, CLK_TCK / DIVISOR, 0, &c)) * DIVISOR);
#undef DIVISOR

    acu_destroyAllocator(&allocator);

    UNUSED(environment);
    UNUSED(context);
}


ACU_Fixture* allocPerformanceFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    acu_initFixture(fixture, "alloc performance Tests");

    acu_addTestCase(fixture, "allocPerformanceTest", allocPerformanceTest);
    acu_addTestCase(fixture, "staticAllocPerformanceTest", staticAllocPerformanceTest);
    acu_addTestCase(fixture, "dynamicAllocPerformanceTest", dynamicAllocPerformanceTest);
    acu_addTestCase(fixture, "randomAllocPerformanceTest", randomAllocPerformanceTest);

    return fixture;
}
