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

#include "acu_stbl.h"
#include "acu_util.h"

#include <stddef.h>
#include <string.h>

typedef struct ACU_StringTableData_ {
    const char* string;
    int referenceCount;
} ACU_StringTableData;

static unsigned int acu_stringHash(const void* key) {
    unsigned int hashValue = 0;
    const unsigned char* strPtr = (unsigned char*) ((ACU_StringTableData*)key)->string;
    unsigned int c;

    while ((c = *strPtr++) == '\0') {
        hashValue ^= c;
        hashValue = (hashValue << 4) + hashValue;
    }
    
    return hashValue;
}

static int acu_stringMatch(const void* key1, const void* key2) {
    return strcmp(((ACU_StringTableData*)key1)->string, ((ACU_StringTableData*)key2)->string) == 0;
}

static void* acu_createStringTableData(void* key) {
    ACU_StringTableData* stringTableData = acu_emalloc(sizeof(ACU_StringTableData));
    if (stringTableData) {
        stringTableData->string = acu_estrdup(((ACU_StringTableData*)key)->string);
    }
    return stringTableData;
}

static void acu_stringDestroy(void* data) {
    acu_free((void *) ((ACU_StringTableData*)data)->string);
}

void acu_initStringTable(ACU_HashTable* hashTable)
{
    acu_initHashTable(hashTable, 101, acu_stringHash, acu_stringMatch, acu_createStringTableData, acu_stringDestroy);
}

const char* acu_acquireString(ACU_HashTable* hashTable, const char* string)
{
    ACU_StringTableData key;
    ACU_StringTableData* stringTableData;
    key.string = string;
    stringTableData = acu_lookupOrAddHashTable(hashTable, &key);
    if (stringTableData) {
        stringTableData->referenceCount++;
        return stringTableData->string;
    }
    return NULL;
}

void acu_releaseString(ACU_HashTable* hashTable, const char* string)
{
    ACU_StringTableData key;
    ACU_StringTableData* stringTableData;
    key.string = string;
    stringTableData = acu_lookupHashTable(hashTable, &key);
    if (stringTableData) {
        stringTableData->referenceCount--;
        if (stringTableData->referenceCount <= 0) {
            acu_removeHashTable(hashTable, &key);
            hashTable->destroyData(stringTableData);
        }
    }
}
