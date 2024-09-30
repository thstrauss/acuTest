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
#include "hstb_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>
#include <acu_hstb.h>

static int hash(const void* data) {
    return *(int*) data;
}

static int match(const void* key1, const void* key2) {
    return *(int*)key1 == *(int*)key2;
}

static void emptyHashTable(ACU_ExecuteEnv* environment, const void* context) {
    ACU_HashTable hashtable;

    ACU_initHashTable(&hashtable, 10, hash, match, (ACU_HashTableDestroyFunc*) NULL);

    ACU_assert(environment, int, Equal, hashtable.size, 0, "Not empty")

    ACU_destroyHashTable(&hashtable);
}

static void fillHashTable(ACU_ExecuteEnv* environment, const void* context) {
    ACU_HashTable hashtable;
    int i;
    int values[40];
    int* lookupValue;

    ACU_initHashTable(&hashtable, 10, hash, match, (ACU_HashTableDestroyFunc*) NULL);

    for (i = 0; i < 40; i++) {
        values[i] = i;
        ACU_insertHashTable(&hashtable, &values[i]);
    }

    i = 15;
    lookupValue = &i;

    ACU_lookupHashTable(&hashtable, (void**) &lookupValue);

    ACU_assert(environment, int, Equal, hashtable.size, 40, "Not empty");

    ACU_assert_ptrEqual(environment, lookupValue, &values[15], "wrong value looked up.");

    ACU_destroyHashTable(&hashtable);
}

ACU_Fixture* hashTableFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "hash table tests");
    
    acu_fixtureAddTestCase(fixture, "empty Hashtable", emptyHashTable);
    acu_fixtureAddTestCase(fixture, "fill Hashtable", fillHashTable);

    return fixture;
}
