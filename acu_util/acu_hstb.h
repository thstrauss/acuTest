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

#include <stddef.h>

typedef void ACU_HashTableDestroyFunc(void* data);
typedef unsigned int ACU_HashTableHashFunc(const void* key);
typedef int ACU_HashTableMatchFunc(const void* key1, const void* key2);

typedef void ACU_HashTableVisitorFunc(const void* data, void* visitorContext);

typedef struct ACU_HashTableVisitor_ {
    ACU_HashTableVisitorFunc* visitor;
    void* context;
} ACU_HashTableVisitor;

typedef struct ACU_HashTable_ {
    ACU_List* table;
    unsigned int buckets;
    ACU_HashTableHashFunc* hash;
    ACU_HashTableMatchFunc* match;
    size_t size;
} ACU_HashTable;

__EXPORT ACU_HashTable* acu_mallocHashTable(void);

__EXPORT int acu_initHashTable(ACU_HashTable* hashTable, unsigned int buckets, ACU_HashTableHashFunc* hash, ACU_HashTableMatchFunc* match, ACU_HashTableDestroyFunc* destroy);

__EXPORT void acu_destroyHashTable(ACU_HashTable* hashTable);

__EXPORT int acu_insertHashTable(ACU_HashTable* hashTable, const void* data);

__EXPORT int acu_removeHashTable(ACU_HashTable* hashTable, void** data);

__EXPORT int acu_lookupHashTable(ACU_HashTable* hashTable, void** data);

__EXPORT void acu_acceptHashTable(const ACU_HashTable* list, ACU_HashTableVisitor* visitor);


#endif 
