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
#include "../acu_stbl.h"


#include <stddef.h>
#include <string.h>

size_t acu_ellipsisString(char* buffer, size_t bufferSize, const char* s, size_t width)
{
#define ellipsesChar '.'
#define ellipsesLength 3

    size_t length = acu_strlen(s);
    if (length <= width) {
        strncpy(buffer, s, length);
        width = length;
    }
    else if (width < ellipsesLength) {
        char* bufferPtr = buffer;
        int i;
        for (i = 0; i < width; i++) {
            *(bufferPtr++) = ellipsesChar;
        }
        length = width;
    }
    else {
        size_t remainderEnd = (width - ellipsesLength) >> 1;
        size_t remainderStart = width - remainderEnd - ellipsesLength;
        char* bufferPtr = buffer;

        strncpy(bufferPtr, s, remainderStart);
        bufferPtr += remainderStart;
        *(bufferPtr++) = ellipsesChar;
        *(bufferPtr++) = ellipsesChar;
        *(bufferPtr++) = ellipsesChar;
        strncpy(bufferPtr, s + (length - remainderEnd), remainderEnd);
        length = width;
    }
    buffer[width] = '\0';
    UNUSED(bufferSize);
    return length;
}

char* __acu_estrdup(const char* s) {
    size_t size = acu_strlen(s) + 1;
    char* temp = malloc(size);
    if (temp) {
        const char* end = temp + (size / sizeof(unsigned long)) * sizeof(unsigned long);
        char* t = temp;
        while (t < end) {
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
        }
        end = temp + size;
        while (t < end) {
            *(t++) = *(s++);
        }


        return temp;
    }
    acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    return NULL;
}

extern int __acuMemoryTrackingEnabled;
extern ACU_HashTable* __allocTable;
extern ACU_HashTable* __stringTable;

char* __strdupToAllocTable(const char* s, const char* fileName, int line) {
    size_t size = acu_strlen(s) + 1;
    char* temp = malloc(size);
    if (temp) {
        const char* end = temp + (size / sizeof(unsigned long)) * sizeof(unsigned long);
        char* t = temp;
        while (t < end) {
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
            if (t >= end) {
                break;
            }
            *((unsigned long*)t) = *((unsigned long*)s);
            t += (size_t)sizeof(unsigned long);
            s += (size_t)sizeof(unsigned long);
        }
        end = temp + size;
        while (t < end) {
            *(t++) = *(s++);
        }

        if (__acuMemoryTrackingEnabled) {
            Block key;
            Block* block;
            key.p = temp;
            __acuMemoryTrackingEnabled = 0;
            block = (Block*)acu_lookupOrAddHashTable(__allocTable, &key);
            block->fileName = acu_acquireString(__stringTable, fileName);
            __acuMemoryTrackingEnabled = 1;
            block->size = size;
            block->line = line;
        }
        return temp;
    }
    acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    return NULL;
}