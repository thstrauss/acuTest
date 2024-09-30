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

#pragma once
#ifndef _ACU_HASHTABLE_H_
#define _ACU_HASHTABLE_H_

#include "acu_cmmn.h"
#include "acu_list.h"

typedef void ACU_HashTableDestroyFunc(void* data);
typedef int ACU_HashTableHashFunc(const void* key);
typedef int ACU_HashTableMatchFunc(const void* key1, const void* key2);

typedef struct ACU_HashTable_ {
    int buckets;
    ACU_HashTableHashFunc* hash;
    ACU_HashTableMatchFunc* match;

    int size;
    ACU_List* table;
} ACU_HashTable;

__EXPORT int ACU_initHashTable(ACU_HashTable* hashTable, int buckets, ACU_HashTableHashFunc* hash, ACU_HashTableMatchFunc* match, ACU_HashTableDestroyFunc* destroy);

__EXPORT void ACU_destroyHashTable(ACU_HashTable* hashTable);

__EXPORT int ACU_insertHashTable(ACU_HashTable* hashTable, const void* data);

__EXPORT int ACU_removeHashTable(ACU_HashTable* hashTable, void** data);

__EXPORT int ACU_lookupHashTable(ACU_HashTable* hashTable, void** data);

#endif 
