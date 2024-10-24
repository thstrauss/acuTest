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

 #include "acu_hstb.h"
 #include "acu_util.h"

ACU_HashTable* acu_mallocHashTable(void)
{
    return acu_emalloc(sizeof(ACU_HashTable));
}

int acu_initHashTable(ACU_HashTable* hashTable, unsigned int buckets, ACU_HashTableHashFunc* hash, ACU_HashTableMatchFunc* match,ACU_HashTableCreateDataFunc* create, ACU_HashTableDestroyFunc* destroy)
{
    ACU_List* bucketList;
    ACU_List* bucketListEnd;

    hashTable->table = (ACU_List*) acu_emalloc(buckets * sizeof(ACU_List));
    if (!hashTable->table) {
        return -1;
    }
    hashTable->hash = hash;
    hashTable->match = match;
    hashTable->createData = create;
    hashTable->destroyData = destroy;
    hashTable->buckets = buckets;

    bucketList = hashTable->table;
    bucketListEnd = hashTable->table + hashTable->buckets;

    for (; bucketList < bucketListEnd; bucketList++) {
        acu_initList(bucketList, (ACU_ListDestroyFunc*) destroy);
    }
    return 0;
}

void acu_destroyHashTable(ACU_HashTable* hashTable)
{
    ACU_List* bucketList = hashTable->table;
    ACU_List* bucketListEnd = bucketList + hashTable->buckets;

    for (; bucketList < bucketListEnd; bucketList++) {
        acu_destroyList(bucketList);
    }
    acu_free(hashTable->table);
}

static ACU_List* acu_lookupBucketList(ACU_HashTable* hashTable,const void* data)
{
    ACU_ListElement* element;
    ACU_List* bucketList;
    
    bucketList = hashTable->table + hashTable->hash(data) % hashTable->buckets;
    for (element = bucketList->head; element; element = element->next) {
        if (hashTable->match(data, element->data)) {
            return NULL;
        }
    }
    return bucketList;
}

const void* acu_insertHashTable(ACU_HashTable* hashTable, const void* data)
{
    return acu_insertHeadList(acu_lookupBucketList(hashTable, data), data);
}

void* acu_removeHashTable(ACU_HashTable* hashTable, const void* key)
{
    ACU_ListElement* element, *prev = NULL;
    ACU_List* bucketList;

    bucketList = hashTable->table + hashTable->hash(key) % hashTable->buckets;
    for (element = bucketList->head; element; element = element->next) {
        if (hashTable->match(key, element->data)) {
            return acu_removeNextList(bucketList, prev);
        }
        prev = element;
    }
    return NULL;
}

void* acu_lookupHashTable(ACU_HashTable* hashTable, const void* key)
{
    ACU_ListElement* element;

    for (element = (hashTable->table + hashTable->hash(key) % hashTable->buckets)->head; element; element = element->next) {
        if (hashTable->match(key, element->data)) {
            return (void*) element->data;
        }
    }
    return NULL;
}

const void* acu_lookupOrAddHashTable(ACU_HashTable* hashTable, const void* key)
{
    ACU_ListElement* element;
    ACU_List* bucketList;

    bucketList = hashTable->table + hashTable->hash(key) % hashTable->buckets;
    element = bucketList->head;
    while(element) {
        if (hashTable->match(key, element->data)) {
            return (void*)element->data;
        }
        element = element->next;
    }
    return acu_insertHeadList(bucketList, hashTable->createData(key));
}

void acu_acceptHashTable(const ACU_HashTable* hashTable, ACU_HashTableVisitor* visitor)
{
    ACU_ListVisitor listVisitor;
    ACU_List* bucketList = hashTable->table;
    ACU_List* bucketListEnd = bucketList + hashTable->buckets;

    listVisitor.visitor = (ACU_ListVisitorFunc*) visitor->visitor;
    listVisitor.context = visitor->context;

    for (; bucketList < bucketListEnd; bucketList++) {
        if (bucketList->head) {
            acu_acceptList(bucketList, &listVisitor);
        }
    }
}

size_t acu_getHashTableSize(const ACU_HashTable* hashTable)
{
    size_t size = 0;
    ACU_List* bucketList;
    ACU_List* bucketListEnd = hashTable->table + hashTable->buckets;

    for (bucketList = hashTable->table; bucketList < bucketListEnd; bucketList++) {
        if (bucketList->head) {
            size += acu_getListSize(bucketList);
        }
    }
    return size;
}

size_t acu_getUsedBucketCount(const ACU_HashTable* hashTable)
{
    size_t size = 0;
    ACU_List* bucketList;
    ACU_List* bucketListEnd = hashTable->table + hashTable->buckets;

    for (bucketList = hashTable->table; bucketList < bucketListEnd; bucketList++) {
        if (bucketList->head) {
            size ++;
        }
    }
    return size;
}
