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

#include "plst_tst.h"

#include <acu_tryc.h>

#include <acu_perf.h>

#include <acu_list.h>

#include <stdlib.h>

#include <stdio.h>
#include <time.h>

#define DISABLE_CACHE 0

static void addRemoveListFunc(void* context) {
    ACU_List* list = context;
    int i;

    for (i = 0; i < 100; i++) {
        acu_insertHeadList(list, NULL);
    }
    for (i = 0; i < 100; i++) {
        acu_removeNextList(list, NULL);
    }
}

static void listPerformanceTest(ACU_ExecuteEnv* environment, const void* context) {

    ACU_List list;

    acu_initList(&list, (ACU_ListDestroyFunc*) NULL, NULL);

#define DIVISOR 3
    printf("addRemoveListFunc\t%ld\n\r", (acu_measureLoop(addRemoveListFunc, CLK_TCK / DIVISOR, DISABLE_CACHE, &list)) * DIVISOR);
#undef DIVISOR

    acu_destroyList(&list);
    
    UNUSED(environment);
    UNUSED(context);
}

ACU_Fixture* listPerformanceFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    acu_initFixture(fixture, "list performance Tests");

    acu_addTestCase(fixture, "listPerformanceTest", listPerformanceTest);


    return fixture;
}
