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

#pragma once
#ifndef _ACU_UTILS_H_
#define _ACU_UTILS_H_

#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include "acu_cmmn.h"
#include "acu_hstb.h"

typedef enum ACU_Level_
{
    acu_error = 1,
    acu_warning = 2,
} ACU_Level;

/* Defines the error handler call back. */
typedef void ACU_ErrorHandlerFunc(enum ACU_Level errorLevel, const char* errorMessage);
typedef size_t ACU_WriteHandlerFunc(const char* buffer);
typedef void ACU_setWriteHandlerFunc(ACU_WriteHandlerFunc* writeHandler);

/*
* Returns the program name.
*/
__EXPORT char* acu_progName(void);

/*
* Sets the program name.
*/
__EXPORT void acu_setProgName(const char* progName);

__EXPORT void acu_setErrorHandler(ACU_ErrorHandlerFunc* errorHandler);

__EXPORT ACU_WriteHandlerFunc* acu_getWriteHandler(void);
__EXPORT void acu_setWriteHandler(ACU_WriteHandlerFunc* writeHandler);

/*
    Prints an error message to stderr and terminates the program. 
    The arguments are according to stdio.h printf().
*/
__EXPORT void acu_eprintf(const char* format, ...);

/*
    Prints a warning message to stderr. 
    The arguments are according to stdio.h printf().
*/
__EXPORT void acu_wprintf(const char* format, ...);

__EXPORT int acu_printf_s(char* buffer, size_t bufferSize, const char* format, ...);

#define acu_estrdup(s) (__allocTable ?(char*)__addMallocToAllocTable(__acu_estrdup((s)), strlen(s), "%s", __FILE__, __LINE__):__acu_estrdup((s))) 
__EXPORT char* __acu_estrdup(const char* s);

__EXPORT void acu_enabledTrackMemory(int enabled);
__EXPORT void acu_reportTrackMemory(void);

#define acu_emalloc(n) (__allocTable ? __addMallocToAllocTable(__acu_emalloc((n)), (n), "%p", __FILE__, __LINE__) :__acu_emalloc((n)))

__EXPORT void* __acu_emalloc(size_t n);

__EXPORT void acu_free(void* buf);

__EXPORT int acu_sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...);
__EXPORT int acu_vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args);

__EXPORT size_t acu_ellipsisString(char* buffer, size_t bufferSize, const char* s, size_t width);

__EXPORT void* __addMallocToAllocTable(void* p, size_t size, const char* format, const char* fileName, int line);

#ifdef __TOS__
extern ACU_HashTable* __allocTable;
#else
__EXPORT ACU_HashTable* __allocTable;
#endif

/*
	Converts a NULL reference to the "NULL" string.
*/
#define SAFE_REF(ref) ((ref)?(ref):"NULL")

#endif
