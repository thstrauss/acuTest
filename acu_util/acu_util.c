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
#include <math.h>

#include "acu_util.h"
#include "acu_hstb.h"

extern void va_acu_printf(ACU_Level level, const char* format, va_list args);

static char* programName = NULL;
static ACU_HashTable* __allocTable = NULL;
static unsigned int __shift = 3;

char* acu_progName(void) {
    return programName;
}

void acu_setProgName(const char* progName) {
    if (programName) {
        acu_free(programName);
    }
    if (progName) {
        programName = acu_estrdup(progName);
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

__EXPORT ACU_HashTable* acu_getAllocTable(void)
{
    return __allocTable;
}

void acu_setAllocTable(ACU_HashTable* allocTable)
{
    __allocTable = allocTable;
}

static size_t defaultWriteHandler(const char* buffer) {
    return fwrite(buffer, sizeof(char), strlen(buffer), stdout);
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

typedef struct Block_ {
    void* p;
    size_t size;
    const char* fileName;
    int line;
} Block;

static unsigned int hash(const void* key) {
    const Block* block = key;
    return (unsigned int) ((size_t) block->p) >> __shift;
}

static int match(const void* key1, const void* key2) {
    const Block* block1 = key1;
    const Block* block2 = key2;
    return block1->p == block2->p;
}

static void destroy(void* data) {
    Block* block = data;
    free(block->fileName);
    free(data);
}

static void __freeAllocTable(void) {
    if (__allocTable) {
        acu_destroyHashTable(__allocTable);
        free(__allocTable);
        __allocTable = NULL;
    }
}

void acu_enabledTrackMemory(int enabled)
{
    if (enabled) {
    	ACU_HashTable* allocTable;
        __freeAllocTable();
        allocTable = malloc(sizeof(ACU_HashTable));
        __shift = (int) (log(sizeof(void*)+1.0) / log(2.0));
        allocTable = malloc(sizeof(ACU_HashTable));
        acu_initHashTable(allocTable, 2003, hash, match, destroy);
        __allocTable = allocTable;
    }
    else {
        __freeAllocTable();
    }
}

static void report(const void* data, void* visitorContext) {
    const Block* block = data;
    char buffer1[512];
    acu_printf_s(buffer1, sizeof buffer1, "%s:%d size = %ld: %p\n\r", block->fileName, block->line, block->size, block->p);
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

void* __addMallocToAllocTable(void* p, size_t size, const char* fileName, int line) {
    if (__allocTable) {
        Block* block = (Block*)malloc(sizeof(Block));
        if (block) {
            ACU_HashTable* allocTable;
            block->p = p;
            block->size = size;
            block->line = line;
            allocTable = __allocTable;
            __allocTable = NULL;
            block->fileName = __acu_estrdup(fileName);
            acu_insertHashTable(allocTable, block);
            __allocTable = allocTable;
        }
    }
    return p;
}

void* __acu_emalloc(size_t size) {
    void* p = malloc(size);
    if (p) {
        return p;
    }
    acu_eprintf("acu_emalloc of %u bytes failed:", size);
    return NULL;
}

__EXPORT void acu_free(void* block)
{
    if (__allocTable) {
        ACU_HashTable* allocTable;
        Block key;
        Block* out = &key;
        key.p = block;
        allocTable = __allocTable;
        __allocTable = NULL;
        if (acu_removeHashTable(allocTable, (void*) &out) == 0) {
            free(out);
        }
        __allocTable = allocTable;
    }
    free(block);
}

long acu_prime(long n) {
    long i, j; 
    if (!(n & 1)) {
        n--;
    }

    for (i = n; i >= 2; i -= 2) {
        double sqrtI;
        if (i % 2 == 0) {
            continue;
        }
        sqrtI = sqrt(i);
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

