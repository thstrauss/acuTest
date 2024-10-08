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
#include <stdio.h>
#include <string.h>

#include "..\acu_util.h"

static char errorBuffer[1024];

extern ACU_ErrorHandlerFunc* __acu_errorHandler;
extern int __acu_allocCount;
extern int __enabledTrackMemory;

void va_acu_printf(ACU_Level level, const char* format, va_list args) {
    int bufPos = 0;
    if (acu_progName() != NULL) {
        bufPos += acu_sprintf_s(errorBuffer, sizeof(errorBuffer), "%s: ", acu_progName());
    }

    bufPos += acu_sprintf_s(errorBuffer + bufPos, sizeof(errorBuffer) - bufPos, format, args);

    if (format[0] != '\0' && format[strlen(format) - 1] == ':') {
        char buffer[50];
        strerror_s(buffer, 50, errno);
        bufPos += acu_sprintf_s(errorBuffer + bufPos, sizeof(errorBuffer) - bufPos, " %d %s", errno, buffer);
    }
    __acu_errorHandler(level, errorBuffer);
}

int acu_sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
    int result;
    va_list args;
    va_start(args, format);
    result = vsprintf_s(buffer, sizeOfBuffer, format, args);
    va_end(args);
    return result;
}

int acu_vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args)
{
    return vsprintf_s(buffer, sizeOfBuffer, format, args);
}


char* __acu_estrdup(const char* s, const char* fileName, int line) {
    char* temp = _strdup(s);
    if (temp) {
        if (__enabledTrackMemory) {
            __addMallocToAllocTable(temp, strlen(temp), "%s", fileName, line);
        }
        __acu_allocCount++;
        return temp;
    }
    acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    return NULL;
}

size_t acu_ellipsisString(char* buffer, size_t bufferSize, const char* s, size_t width)
{
    static char ellipsesChar = '.';
    const int ellipsesLength = 3;

    size_t length = strlen(s);
    if (length <= width) {
        strncpy_s(buffer, bufferSize, s, width);
        width = length;
    } else if (width < ellipsesLength) {
        char* bufferPtr = buffer;
        int i;
        for (i = 0; i < width; i++) {
            *(bufferPtr++) = ellipsesChar;
        }
        length = width;
    } else {
        size_t remainderEnd = (width - ellipsesLength) / 2;
        size_t remainderStart = width - remainderEnd - ellipsesLength;
        char* bufferPtr = buffer;

        strncpy_s(bufferPtr, remainderStart + 1, s, remainderStart);
        bufferPtr += remainderStart;
        *(bufferPtr++) = ellipsesChar;
        *(bufferPtr++) = ellipsesChar;
        *(bufferPtr++) = ellipsesChar;
        strncpy_s(bufferPtr, remainderEnd + 1, s + (length - remainderEnd), remainderEnd);
        length = width;
    }
    buffer[width] = '\0';
    return length;
}