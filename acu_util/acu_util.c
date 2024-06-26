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

static char* programName = NULL;

char* acu_progName(void) {
    return programName;
}

char* acu_estrdup(const char* s) {
#ifdef __TOS__
    char* temp = strdup(s);
#else
    char* temp = _strdup(s);
#endif
    if (!temp) {
        acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    }
    return temp;
}

void acu_setProgName(const char* progName) {
    if (programName) {
        free(programName);
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

static ACU_ErrorHandlerFunc* acu_errorHandler = defaultErrorHandler;

void acu_setErrorHandler(ACU_ErrorHandlerFunc* errorHandler)
{
    if (errorHandler) {
        acu_errorHandler = errorHandler;
    } else {
        acu_errorHandler = defaultErrorHandler;
    }
}

static size_t defaultWriteHandler(const char* buffer) {
    return fwrite(buffer, sizeof(char), strlen(buffer), stdout);
}

static ACU_WriteHandlerFunc* acu_writeHandler = defaultWriteHandler;

__EXPORT void acu_setWriteHandler(ACU_WriteHandlerFunc* writeHandler)
{
    if (writeHandler) {
        acu_writeHandler = writeHandler;
    } else {
        acu_writeHandler = defaultWriteHandler;
    }
}

static char errorBuffer[1024];

static void va_acu_printf(ACU_Level level, const char* format, va_list args) {
    int bufPos = 0;
    if (acu_progName() != NULL) {
        bufPos += acu_sprintf_s(errorBuffer, sizeof(errorBuffer), "%s: ", acu_progName());
    }

    bufPos += acu_sprintf_s(errorBuffer+bufPos, sizeof(errorBuffer) - bufPos, format, args);

    if (format[0] != '\0' && format[strlen(format) - 1] == ':') {
#ifdef __TOS__
        bufPos += acu_sprintf_s(errorBuffer+bufPos, sizeof(errorBuffer) - bufPos, " %d %s", errno, strerror(errno));
#else 
        char buffer[50];
        strerror_s(buffer, 50, errno);
        bufPos += acu_sprintf_s(errorBuffer + bufPos, sizeof(errorBuffer) - bufPos, " %d %s", errno, buffer);
#endif
    }
    acu_errorHandler(level, errorBuffer);
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

void* acu_emalloc(size_t size) {
    void* p;
    p = malloc(size);
    if (!p) {
        acu_eprintf("acu_emalloc of %u bytes failed:", size);
    }
    return p;
}

int acu_sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...)
{
    int result;
    va_list args;
    va_start(args, format);
#ifdef __TOS__
    result = vsprintf(buffer, format, args);
    UNUSED(sizeOfBuffer);
#else
    result = vsprintf_s(buffer, sizeOfBuffer, format, args);
#endif
    va_end(args);
    return result;
}

int acu_vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args)
{
#ifdef __TOS__
    UNUSED(sizeOfBuffer);
    return vsprintf(buffer, format, args);
#else
    return vsprintf_s(buffer, sizeOfBuffer, format, args);
#endif
}


