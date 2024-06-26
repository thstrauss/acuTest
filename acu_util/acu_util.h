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
#include "acu_cmmn.h"

typedef enum ACU_Level_
{
    acu_error = 1,
    acu_warning = 2,
} ACU_Level;

typedef void ACU_ErrorHandlerFunc(enum ACU_Level errorLevel, const char* errorMessage);
typedef size_t ACU_WriteHandlerFunc(const char* buffer);

/*
* Returns the program name.
*/
__EXPORT char* acu_progName(void);

/*
* Sets the program name.
*/
__EXPORT void acu_setProgName(const char* progName);

__EXPORT void acu_setErrorHandler(ACU_ErrorHandlerFunc* errorHandler);

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

__EXPORT char* acu_estrdup(const char* s);
__EXPORT void* acu_emalloc(size_t n);

__EXPORT int acu_sprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, ...);
__EXPORT int acu_vsprintf_s(char* buffer, size_t sizeOfBuffer, const char* format, va_list args);

/*
	Converts a NULL reference to the "NULL" string.
*/
#define SAFE_REF(ref) ((ref)?(ref):"NULL")

#endif
