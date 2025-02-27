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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include "allc_tst.h"

#include <acu_allc.h>
#include <acu_dall.h>

static void staticAllocReturnPointerToMemory(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator staticAllocator;
    void* alloc;

    acu_initStaticAllocator(&staticAllocator, sizeof(int), 10, (ACU_FreeContext*) NULL);

    alloc = acu_allocStaticAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, alloc, "ptr is null");
    acu_freeStaticAllocator(alloc);

    acu_destroyStaticAllocator(&staticAllocator);

    UNUSED(context);
}

static void staticAllocWithZeroLength(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator staticAllocator;
    void* alloc;

    acu_initStaticAllocator(&staticAllocator, sizeof(int), 0, (ACU_FreeContext*)NULL);

    alloc = acu_allocStaticAllocator(&staticAllocator);
    ACU_assert_ptrIsNull(environment, alloc, "ptr is not null");

    acu_destroyStaticAllocator(&staticAllocator);

    UNUSED(context);
}

static void staticAllocReturnPointerToMemory1(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator staticAllocator;
    void* alloc;

    acu_initStaticAllocator(&staticAllocator, sizeof(int), 1, (ACU_FreeContext*)NULL);

    alloc = acu_allocStaticAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, alloc, "ptr is null");
    ACU_assert_ptrIsNull(environment, acu_allocStaticAllocator(&staticAllocator), "ptr is not null");
    acu_freeStaticAllocator(alloc);

    acu_destroyStaticAllocator(&staticAllocator);

    UNUSED(context);
}

static void staticAllocReturnPointerToMemory2(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator staticAllocator;
    void* alloc1;
    void* alloc2;

    acu_initStaticAllocator(&staticAllocator, sizeof(int), 2, (ACU_FreeContext*)NULL);

    alloc1 = acu_allocStaticAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, alloc1, "ptr is null");
    alloc2 = acu_allocStaticAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, alloc2, "ptr is null");
    ACU_assert_ptrIsNull(environment, acu_allocStaticAllocator(&staticAllocator), "ptr is not null");
    acu_freeStaticAllocator(alloc1);
    acu_freeStaticAllocator(alloc2);

    acu_destroyStaticAllocator(&staticAllocator);

    UNUSED(context);
}

static void emptyDynamicAllocator(ACU_ExecuteEnv* environment, const void* context) {
    ACU_DynamicAllocator staticAllocator;

    acu_initAllocator(&staticAllocator, sizeof(int), 10);

    ACU_assert(environment, size_t, Equal, acu_getAllocatedElements(&staticAllocator), 0, "Not 0");

    acu_destroyAllocator(&staticAllocator);

    UNUSED(context);
    UNUSED(environment);
}

static void allocatorReturnsNullWhenAllElementsAllocated(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator staticAllocator;
    void* alloc1, * alloc2, * alloc3;

    acu_initStaticAllocator(&staticAllocator, sizeof(int), 2, (ACU_FreeContext*)NULL);

    alloc1 = acu_allocStaticAllocator(&staticAllocator);
    alloc2 = acu_allocStaticAllocator(&staticAllocator);
    alloc3 = acu_allocStaticAllocator(&staticAllocator);

    ACU_assert_ptrIsNotNull(environment, alloc1, "ptr is null");
    ACU_assert_ptrIsNotNull(environment, alloc2, "ptr is null");
    ACU_assert_ptrIsNull(environment, alloc3, "ptr is not null");

    acu_freeStaticAllocator(alloc1);
    acu_freeStaticAllocator(alloc2);

    acu_destroyStaticAllocator(&staticAllocator);

    UNUSED(context);
}

static void dynamicAllocReturnPointerToMemory(ACU_ExecuteEnv* environment, const void* context) {
    ACU_DynamicAllocator staticAllocator;
    void* buffer1;
    void* buffer2;

    acu_initAllocator(&staticAllocator, sizeof(int), 1);

    buffer1 = acu_allocAllocator(&staticAllocator);
    buffer2 = acu_allocAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, buffer1, "ptr is null");
    ACU_assert_ptrIsNotNull(environment, buffer2, "ptr is null");
    ACU_assert(environment, size_t, Equal, acu_getAllocatedElements(&staticAllocator), 2, "Not 2");
    acu_freeAllocator(buffer1);
    acu_freeAllocator(buffer2);
    ACU_assert(environment, size_t, Equal, acu_getAllocatedElements(&staticAllocator), 0, "Not 0");

    buffer1 = acu_allocAllocator(&staticAllocator);
    ACU_assert_ptrIsNotNull(environment, buffer1, "ptr is null");

    acu_destroyAllocator(&staticAllocator);

    UNUSED(context);
}

static void dynamicAllocWithZeroLength(ACU_ExecuteEnv* environment, const void* context) {
    ACU_DynamicAllocator staticAllocator;
    void* buffer;

    acu_initAllocator(&staticAllocator, sizeof(int), 0);

    buffer = acu_allocAllocator(&staticAllocator);
    ACU_assert_ptrIsNull(environment, buffer, "ptr is null");
    ACU_assert(environment, size_t, Equal, acu_getAllocatedElements(&staticAllocator), 0, "Not 0");

    acu_destroyAllocator(&staticAllocator);

    UNUSED(context);
}

ACU_Fixture* allocatorTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "allocator Tests");

    acu_addTestCase(fixture, "staticAllocReturnPointerToMemory", staticAllocReturnPointerToMemory);
    acu_addTestCase(fixture, "staticAllocWithZeroLength", staticAllocWithZeroLength);
    acu_addTestCase(fixture, "staticAllocReturnPointerToMemory1", staticAllocReturnPointerToMemory1);
    acu_addTestCase(fixture, "staticAllocReturnPointerToMemory2", staticAllocReturnPointerToMemory2);
    acu_addTestCase(fixture, "allocatorReturnsNullWhenAllElementsAllocated", allocatorReturnsNullWhenAllElementsAllocated);
    acu_addTestCase(fixture, "emptyDynamicAllocator", emptyDynamicAllocator);
    acu_addTestCase(fixture, "dynamicAllocReturnPointerToMemory", dynamicAllocReturnPointerToMemory);
    acu_addTestCase(fixture, "dynamicAllocWithZeroLength", dynamicAllocWithZeroLength);

    return fixture;
}

