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

#ifndef __acu_string__
#define __acu_string__

#include "acu_cmmn.h"
#include "acu_util.h"

#include <stddef.h> 

__EXPORT char* __acu_estrdup(const char* s);

__EXPORT size_t acu_strlen(const char* s);

#define acu_estrdup(s) (acu_isMemoryTrackingEnabled() ? (char*)__strdupToAllocTable(s, __FILE__, __LINE__):__acu_estrdup((s))) 

__EXPORT size_t acu_ellipsisString(char* buffer, size_t bufferSize, const char* s, size_t width);

__EXPORT const char* __strdupToAllocTable(const char* s, const char* fileName, int line);

#endif