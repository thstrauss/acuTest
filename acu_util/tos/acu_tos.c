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
#include <stdlib.h>
#include <string.h>

#include "..\acu_util.h"

static char errorBuffer[1024];

extern ACU_ErrorHandlerFunc* acu_errorHandler;

void va_acu_printf(ACU_Level level, const char* format, va_list args) {
    int bufPos = 0;
    if (acu_progName() != NULL) {
        bufPos += acu_sprintf_s(errorBuffer, sizeof(errorBuffer), "%s: ", acu_progName());
    }

    bufPos += acu_sprintf_s(errorBuffer + bufPos, sizeof(errorBuffer) - bufPos, format, args);

    if (format[0] != '\0' && format[strlen(format) - 1] == ':') {
        bufPos += acu_sprintf_s(errorBuffer + bufPos, sizeof(errorBuffer) - bufPos, " %d %s", errno, strerror(errno));
    }
    acu_errorHandler(level, errorBuffer);
}

int acu_sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
    int result;
    va_list args;
    va_start(args, format);
    result = vsprintf(buffer, format, args);
    va_end(args);
    UNUSED(sizeOfBuffer);
    return result;
}

int acu_vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args)
{
    UNUSED(sizeOfBuffer);
    return vsprintf(buffer, format, args);
}

char* acu_estrdup(const char* s) {
    char* temp = strdup(s);
    if (!temp) {
        acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    }
    return temp;
}
