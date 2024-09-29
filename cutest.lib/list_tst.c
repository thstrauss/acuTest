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

#include <stdlib.h>
#include <stddef.h>
#include "list_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>
#include <acu_list.h>

static void emptyList(ACU_ExecuteEnv* environment, const void* context) {
    ACU_List* list = acu_listMalloc();
    acu_listInit(list, (ACU_ListDestroyFunc*) NULL);
    ACU_TRY
        ACU_assert_ptrIsNull(environment, acu_listHead(list), "head is not null.");
    ACU_FINALLY
        acu_listDestroy(list);
        acu_free(list);
    ACU_ETRY
    UNUSED(context);
}

static void listInsert(ACU_ExecuteEnv* environment, const void* context) {
    int data1 = 1;
    int data2 = 2;
    ACU_List* list = acu_listMalloc();
    acu_listInit(list, (ACU_ListDestroyFunc*)NULL);
    acu_listInsertNext(list, NULL, &data1);
    acu_listInsertNext(list, acu_listHead(list), &data2);
    ACU_TRY
        ACU_assert_ptrIsNotNull(environment, acu_listHead(list), "head is null.");
        ACU_assert(environment, int, Equal, *(int*)(acu_listHead(list)->data), data1, "");
        ACU_assert(environment, int, Equal, *(int*)(acu_listHead(list)->next->data), data2, "");
    ACU_FINALLY
        acu_listDestroy(list);
        acu_free(list);
    ACU_ETRY
    UNUSED(context);
}

ACU_Fixture* sampleFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "list tests");

    acu_fixtureAddTestCase(fixture, "emptyList", emptyList);
    acu_fixtureAddTestCase(fixture, "listInsert", listInsert);

    return fixture;
}