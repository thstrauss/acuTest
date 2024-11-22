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
    ACU_StaticAllocator allocator;
    void* alloc;

    acu_initStaticAllocator(&allocator, sizeof(int), 10);

    alloc = acu_allocStaticAllocator(&allocator);
    ACU_assert_ptrIsNotNull(environment, alloc, "ptr is null");
    acu_freeStaticAllocator(alloc);

    acu_destroyStaticAllocator(&allocator);

    UNUSED(context);
}

static void emptyDynamicAllocator(ACU_ExecuteEnv* environment, const void* context) {
    ACU_DynamicAllocator allocator;

    acu_initAllocator(&allocator, sizeof(int), 10);

    acu_destroyAllocator(&allocator);

    UNUSED(context);
    UNUSED(environment);
}

static void allocatorReturnsNullWhenAllElementsAllocated(ACU_ExecuteEnv* environment, const void* context) {
    ACU_StaticAllocator allocator;
    void* alloc1, * alloc2, * alloc3;

    acu_initStaticAllocator(&allocator, sizeof(int), 2);

    alloc1 = acu_allocStaticAllocator(&allocator);
    alloc2 = acu_allocStaticAllocator(&allocator);
    alloc3 = acu_allocStaticAllocator(&allocator);

    ACU_assert_ptrIsNotNull(environment, alloc1, "ptr is null");
    ACU_assert_ptrIsNotNull(environment, alloc2, "ptr is null");
    ACU_assert_ptrIsNull(environment, alloc3, "ptr is not null");

    acu_freeStaticAllocator(alloc1);
    acu_freeStaticAllocator(alloc2);

    acu_destroyStaticAllocator(&allocator);

    UNUSED(context);
}

static void dynamicAllocReturnPointerToMemory(ACU_ExecuteEnv* environment, const void* context) {
    ACU_DynamicAllocator allocator;
    void* buffer;

    acu_initAllocator(&allocator, sizeof(int), 10);

    buffer = acu_allocAllocator(&allocator);
    ACU_assert_ptrIsNotNull(environment, buffer, "ptr is null");
    acu_freeAllocator(buffer);

    acu_destroyAllocator(&allocator);

    UNUSED(context);
}

ACU_Fixture* allocatorTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "allocator Tests");

    acu_addTestCase(fixture, "staticAllocReturnPointerToMemory", staticAllocReturnPointerToMemory);
    acu_addTestCase(fixture, "allocatorReturnsNullWhenAllElementsAllocated", allocatorReturnsNullWhenAllElementsAllocated);
    acu_addTestCase(fixture, "emptyDynamicAllocator", emptyDynamicAllocator);
    acu_addTestCase(fixture, "dynamicAllocReturnPointerToMemory", dynamicAllocReturnPointerToMemory);

    return fixture;
}
