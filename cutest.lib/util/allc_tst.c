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

static void xxx(ACU_ExecuteEnv* environment, const void* context) {
    ACU_Allocator allocator;
    void* alloc;

    ACU_initAllocator(&allocator, sizeof(ACU_Allocator), 10);

    alloc = ACU_allocAllocator(&allocator);
    ACU_freeAllocator(&allocator, alloc);

    ACU_destroyAllocator(&allocator);

    UNUSED(context);
}

static void xxx2(ACU_ExecuteEnv* environment, const void* context) {
    ACU_Allocator allocator;
    void *alloc1, *alloc2;

    ACU_initAllocator(&allocator, sizeof(int), 2);

    alloc1 = ACU_allocAllocator(&allocator);
    alloc2 = ACU_allocAllocator(&allocator);
    ACU_freeAllocator(&allocator, alloc1);
    ACU_freeAllocator(&allocator, alloc2);

    ACU_destroyAllocator(&allocator);

    UNUSED(context);
}

ACU_Fixture* allocatorTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "allocator Tests");

    acu_addTestCase(fixture, "xxx", xxx);
    acu_addTestCase(fixture, "xxx2", xxx2);

    return fixture;
}
