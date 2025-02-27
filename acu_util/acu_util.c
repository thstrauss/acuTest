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

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "acu_util.h"
#include "acu_hstb.h"
#include "acu_stbl.h"
#include "acu_strg.h"

extern void va_acu_printf(ACU_Level level, const char* format, va_list args);

static char* programName = NULL;
ACU_HashTable* __allocTable = NULL;
ACU_HashTable* __stringTable = NULL;
int __acuMemoryTrackingEnabled = 0;
static unsigned int __shift = 3;

char* acu_progName(void) {
    return programName;
}

void acu_setProgName(const char* progName) {
    if (programName) {
        acu_free(programName);
    }
    if (progName) {
        programName = (__acuMemoryTrackingEnabled ? (char*)__strdupToAllocTable(progName, "C:\\thomas\\Atari\\Drive\\K\\cutest.t\\acu_util\\acu_util.c", 51) : __acu_estrdup((progName)));
    }
    else {
        programName = NULL;
    }
}

static void defaultErrorHandler(enum ACU_Level level, const char* message) {
    fflush(stdout);
    fprintf(stderr, "%s\r\n", message);
    if (level == acu_error) {
        exit(2);
    }
}

ACU_ErrorHandlerFunc* __acu_errorHandler = defaultErrorHandler;

void acu_setErrorHandler(ACU_ErrorHandlerFunc* errorHandler)
{
    if (errorHandler) {
        __acu_errorHandler = errorHandler;
    } else {
        __acu_errorHandler = defaultErrorHandler;
    }
}

ACU_HashTable* acu_getAllocTable(void)
{
    return __allocTable;
}

void acu_setAllocTable(ACU_HashTable* allocTable)
{
    __allocTable = allocTable;
}

static size_t defaultWriteHandler(const char* buffer) {
    return fwrite(buffer, sizeof(char), acu_strlen(buffer), stdout);
}

static ACU_WriteHandlerFunc* acu_writeHandler = defaultWriteHandler;

ACU_WriteHandlerFunc* acu_getWriteHandler(void)
{
    return acu_writeHandler;
}

__EXPORT void acu_setWriteHandler(ACU_WriteHandlerFunc* writeHandler)
{
    if (writeHandler) {
        acu_writeHandler = writeHandler;
    } else {
        acu_writeHandler = defaultWriteHandler;
    }
}

void acu_eprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_acu_printf(acu_error, format, args);
    va_end(args);
}

void acu_wprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_acu_printf(acu_warning, format, args);
    va_end(args);
}

int acu_printf_s(char* buffer, size_t bufferSize, const char* format, ...)
{
    int result;
    va_list args;

    va_start(args, format);
    result = acu_vsprintf_s(buffer, bufferSize, format, args);
    va_end(args);

    acu_writeHandler(buffer);

    return result;
}

static unsigned long acu_hashPtr(const void* key) {
    const Block* block = key;
    return (unsigned long) ((size_t) block->buffer) >> __shift;
}

static unsigned long acu_hashPtr2(const void* key) {
    const Block* block = key;
    return (unsigned long)((size_t)block->buffer) >> 2;
}

static unsigned long acu_hashPtr3(const void* key) {
    const Block* block = key;
    return (unsigned long)((size_t)block->buffer) >> 3;
}

static int acu_matchPtr(const void* key1, const void* key2) {
    return (((size_t) ((Block*) key1)->buffer) - ((size_t) ((Block*)key2)->buffer)) == 0;
}

static void __freeAllocTable(void) {
    if (__stringTable) {
        acu_destroyHashTable(__stringTable);
        free(__stringTable);
        __stringTable = NULL;
    }
    if (__allocTable) {
        acu_destroyHashTable(__allocTable);
        free(__allocTable);
        __allocTable = NULL;
    }
}

static void* acu_createBlock(const void* key) {
    Block* block = (Block*)malloc(sizeof(Block));
    if (block) {
        block->buffer = ((Block*)key)->buffer;
    }
    return block;
}


static int nlz(unsigned long x) {
    int n;

    if (x == 0) return(32);
    n = 0;
    if (x <= 0x0000FFFFL) { n = n + 16; x = x << 16; }
    if (x <= 0x00FFFFFFL) { n = n + 8; x = x << 8; }
    if (x <= 0x0FFFFFFFL) { n = n + 4; x = x << 4; }
    if (x <= 0x3FFFFFFFL) { n = n + 2; x = x << 2; }
    if (x <= 0x7FFFFFFFL) { n = n + 1; }
    return n;
}

void acu_enabledTrackMemory(int enabled)
{
    if (enabled) {
        ACU_HashTableHashFunc* hashFunc = acu_hashPtr;
        
        __freeAllocTable();
        __shift = 31-nlz(sizeof(void*));
        
        if (__shift == 2) {
            hashFunc = acu_hashPtr2;
        }
        else if (__shift == 3) {
            hashFunc = acu_hashPtr3;
        }
        __allocTable = malloc(sizeof(ACU_HashTable));
        acu_initHashTable(__allocTable, 2003, hashFunc, acu_matchPtr, acu_createBlock, free);

        __stringTable = malloc(sizeof(ACU_HashTable));
        acu_initStringTable(__stringTable);
    }
    else {
        __freeAllocTable();
    }
    __acuMemoryTrackingEnabled = enabled;
}

static void report(const void* data, void* visitorContext) {
    const Block* block = data;
    char buffer1[512];
    acu_printf_s(buffer1, sizeof buffer1, "%s:%d instance = %ld size = %ld: %p\n\r", block->fileName, block->line, block->instance, block->size, block->buffer);
    UNUSED(visitorContext);
}

__EXPORT void acu_reportTrackMemory(void)
{
    if (acu_getAllocTable()) {
        ACU_HashTableVisitor visitor;
        visitor.visitor = report;
        visitor.context = NULL;
        acu_acceptHashTable(acu_getAllocTable(), &visitor);
    }
}

static size_t instanceCount = 0;

void* __mallocToAllocTable(size_t size, const char* fileName, int line) {
    void* buffer = malloc(size);
    if (buffer) {
        if (__acuMemoryTrackingEnabled) {
            Block key;
            Block* block;
            key.buffer = buffer;
            __acuMemoryTrackingEnabled = 0;
            block = (Block*)acu_lookupOrAddHashTable(__allocTable, &key);
            block->fileName = acu_acquireString(__stringTable, fileName);
            __acuMemoryTrackingEnabled = 1;
            block->size = size;
            block->line = line;
            block->instance = instanceCount++;
        }
        return buffer;
    }
    acu_eprintf("acu_emalloc of %u bytes failed:", size);
    return NULL;
}

void* __acu_emalloc(size_t size) {
    const void* buffer = malloc(size);
    if (buffer) {
        return buffer;
    }
    acu_eprintf("acu_emalloc of %u bytes failed:", size);
    return NULL;
}

void __acu_free(void* block)
{
    void* out;
    Block key;
    key.buffer = block;
    out = acu_removeHashTable(__allocTable, &key);
    if (out) {
        free(out);
    }
    free(block);
}

unsigned long isqrt(unsigned long x) {
    unsigned long x1;
    unsigned long s, g0, g1;

    if (x <= 1) return x;
    s = 1;
    x1 = x - 1L;
    if (x1 > 65535L) { s = s + 8; x1 = x1 >> 16; }
    if (x1 > 255L) { s = s + 4; x1 = x1 >> 8; }
    if (x1 > 15L) { s = s + 2; x1 = x1 >> 4; }
    if (x1 > 3L) { s = s + 1; }

    g0 = 1 << s;
    g1 = (g0 + (x >> s)) >> 1;

    while (g1 < g0) {
        g0 = g1;
        g1 = (g0 + (x / g0)) >> 1;
    }
    return g0;
}

unsigned long acu_prime(unsigned long n) {
    unsigned long i, j; 
    if (!(n & 1)) {
        n--;
    }

    for (i = n; i >= 2; i -= 2) {
        unsigned long sqrtI;
        if (i % 2 == 0) {
            continue;
        }
        sqrtI = isqrt(i);
        for (j = 3; j <= sqrtI; j += 2) {
            if (i % j == 0) {
                break;
            }
        }
        if (j > sqrtI) {
            return i;
        }
    }
    return 2;
}

