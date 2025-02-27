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

#include "../acu_strg.h"

#include "../acu_util.h"
#include "../acu_stbl.h"

#include <stddef.h>
#include <string.h>

char* __acu_estrdup(const char* s) {
    char* temp = _strdup(s);
    if (temp) {
        return temp;
    }
    acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    return NULL;
}

extern ACU_HashTable* __allocTable; 
extern ACU_HashTable* __stringTable;


const char* __strdupToAllocTable(const char* s, const char* fileName, int line) {
    const char* buffer = __acu_estrdup(s);
    if (__acuMemoryTrackingEnabled) {
        Block key;
        Block* block;
        key.buffer = buffer;
        __acuMemoryTrackingEnabled = 0;
        block = (Block*)acu_lookupOrAddHashTable(__allocTable, &key);
        block->fileName = acu_acquireString(__stringTable, fileName);
        __acuMemoryTrackingEnabled = 1;
        block->size = acu_strlen(buffer);
        block->line = line;
    }
    return buffer;
}

static size_t acu_strlen(const char* s) {
    const char* sPtr = s;
    unsigned long  magic = 0x7F7F7F7FL;

    unsigned long  l = *((const unsigned long*)sPtr);
    while (!~(((l & magic) + magic) | l | magic)) {
        sPtr += sizeof(unsigned long);
        l = *((const unsigned long*)sPtr);
        if (~(((l & magic) + magic) | l | magic)) {
            break;
        }
        sPtr += sizeof(unsigned long);
        l = *((const unsigned long*)sPtr);
        if (~(((l & magic) + magic) | l | magic)) {
            break;
        }
        sPtr += sizeof(unsigned long);
        l = *((const unsigned long*)sPtr);
    }
    while (*sPtr) {
        if (!*(++sPtr)) break;
        if (!*(++sPtr)) break;
        sPtr++;
    }
    return sPtr - s;
}

__EXPORT char* acu_strncpy(char* strDest, const char* strSource, size_t count)
{
    strncpy_s(strDest, count+1, strSource, count);
    return strDest;
}
