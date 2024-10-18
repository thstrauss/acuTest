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

static unsigned int acu_stringHash(const void* key) {
    unsigned int hashValue = 2003;
    const unsigned char* strPtr = (unsigned char*) key;
    unsigned int c;

    while ((c = *strPtr++) == '\0') {
        hashValue ^= c;
        hashValue = (hashValue << 4) + hashValue;
    }
    
    return hashValue;
}

static int acu_stringMatch(const void* key1, const void* key2) {
    return strcmp(key1, key2) == 0;
}

static void* acu_createStringTableData(const void* key) {
    return __acu_estrdup(key);
}

static void acu_stringDestroy(void* data) {
    acu_free(data);
}

void acu_initStringTable(ACU_HashTable* hashTable)
{
    acu_initHashTable(hashTable, 101, acu_stringHash, acu_stringMatch, acu_createStringTableData, acu_stringDestroy);
}

const char* acu_acquireString(ACU_HashTable* hashTable, const char* string)
{
    char * stringTableData = acu_lookupOrAddHashTable(hashTable, string);
    if (stringTableData) {
        return stringTableData;
    }
    return NULL;
}

