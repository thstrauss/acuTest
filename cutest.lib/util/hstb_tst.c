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

static unsigned long acu_hashInt(const void* data) {
    return *(unsigned int*) data;
}

static int acu_matchInt(const void* key1, const void* key2) {
    return *(unsigned int*)key1 == *(unsigned int*)key2;
}

static void emptyHashTable(ACU_ExecuteEnv* environment, const void* context) {
    ACU_HashTable hashtable;

    acu_initHashTable(&hashtable, 10, acu_hashInt, acu_matchInt, (ACU_HashTableCreateDataFunc*) NULL,  (ACU_HashTableDestroyFunc*) NULL);

    ACU_assert(environment, size_t, Equal, acu_getHashTableSize(&hashtable), 0, "Not empty")

    acu_destroyHashTable(&hashtable);
    UNUSED(context);
}

static void empty2HashTable(ACU_ExecuteEnv* environment, const void* context) {
    ACU_HashTable* hashtable;

    hashtable = acu_mallocHashTable();
    acu_initHashTable(hashtable, 10, acu_hashInt, acu_matchInt, (ACU_HashTableCreateDataFunc*) NULL, (ACU_HashTableDestroyFunc*)NULL);

    ACU_assert(environment, size_t, Equal, acu_getHashTableSize(hashtable), 0, "Not empty")

    acu_destroyHashTable(hashtable);
    acu_free(hashtable);
    UNUSED(context);
}

static void visitorFunc(const void* data, void* visitorContext) {
    (*(int*) visitorContext)++;
    UNUSED(data);
}

static void fillHashTable(ACU_ExecuteEnv* environment, const void* context) {
    ACU_HashTable hashtable;
    unsigned int i;
    unsigned int values[40];
    unsigned int* lookupValue;
    int count = 0;

    ACU_HashTableVisitor visitor;

    acu_initHashTable(&hashtable, 10, acu_hashInt, acu_matchInt, (ACU_HashTableCreateDataFunc*) NULL, (ACU_HashTableDestroyFunc*) NULL);

    for (i = 0; i < 40; i++) {
        values[i] = i;
        acu_insertHashTable(&hashtable, &values[i]);
    }

    i = 15;

    lookupValue = acu_lookupHashTable(&hashtable, &i);

    ACU_assert(environment, size_t, Equal, acu_getHashTableSize(&hashtable), 40, "Not empty");

    ACU_assert_ptrEqual(environment, lookupValue, &values[15], "wrong value looked up.");

    visitor.visitor = visitorFunc;
    visitor.context = &count;
    acu_acceptHashTable(&hashtable, &visitor);

    ACU_assert(environment, int, Equal, count, 40, "Visitor not visited all elements");

    acu_destroyHashTable(&hashtable);
    UNUSED(context);
}

ACU_Fixture* hashTableTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "hash table tests");
    
    acu_addTestCase(fixture, "empty Hashtable", emptyHashTable);
    acu_addTestCase(fixture, "empty2 Hashtable", empty2HashTable);
    acu_addTestCase(fixture, "fill Hashtable", fillHashTable);

    return fixture;
}
