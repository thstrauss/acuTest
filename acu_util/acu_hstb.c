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

int ACU_initHashTable(ACU_HashTable* hashTable, size_t buckets, ACU_HashTableHashFunc* hash, ACU_HashTableMatchFunc* match, ACU_HashTableDestroyFunc* destroy)
{
    size_t i;
    hashTable->table = (ACU_List*) acu_emalloc(buckets * sizeof(ACU_List));
    if (!hashTable->table) {
        return -1;
    }
    hashTable->buckets = buckets;
    for (i = 0; i < buckets; i++) {
        acu_listInit(&hashTable->table[i], destroy);
    }
    hashTable->hash = hash;
    hashTable->match = match;
    hashTable->size = 0;
    return 0;
}

void ACU_destroyHashTable(ACU_HashTable* hashTable)
{
    size_t i;

    for (i = 0; i < hashTable->buckets; i++) {
        acu_listDestroy(&hashTable->table[i]);
    }
    acu_free(hashTable->table);
}

int ACU_insertHashTable(ACU_HashTable* hashTable, const void* data)
{
    void* temp;
    size_t bucket;
    int retval;

    temp = (void*) data;
    if (!ACU_lookupHashTable(hashTable, &temp)) {
        return 1;
    }

    bucket = hashTable->hash(data) % hashTable->buckets;

    if ((retval = acu_listInsertNext(&hashTable->table[bucket], NULL, data)) == 0) {
        hashTable->size++;
    }

    return retval;
}

int ACU_removeHashTable(ACU_HashTable* hashTable, void** data)
{
    ACU_ListElement* element, *prev;
    size_t bucket;

    bucket = hashTable->hash(*data) % hashTable->buckets;
    prev = NULL;
    for (element = acu_listHead(&hashTable->table[bucket]); element != NULL; element = element->next) {
        if (hashTable->match(*data, element->data)) {
            if (!acu_listRemoveNext(&hashTable->table[bucket], prev, data)) {
                hashTable->size--;
                return 0;
            }
        }
        prev = element;
    }
    return -1;
}

int ACU_lookupHashTable(ACU_HashTable* hashTable, void** data)
{
    ACU_ListElement* element;
    size_t bucket;

    bucket = hashTable->hash(*data) % hashTable->buckets;

    for (element = acu_listHead(&hashTable->table[bucket]); element != NULL; element = element->next) {
        if (hashTable->match(*data, element->data)) {
            *data = (void*) element->data;
            return 0;
        }
    }
    return -1;
}
