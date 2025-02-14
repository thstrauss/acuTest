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
#ifndef __ACU_VERSION__
#define __ACU_VERSION__

#include "acu_cmmn.h"

/*
 * The version of the ACU.
 */
typedef struct ACU_Version_ {
    unsigned char version[4];
} ACU_Version;

typedef ACU_Version* (ACU_getVersionFunc)(void);

/*
 * Returns the version of the ACU.
 */
ACU_Version* acu_getVersion(void);

/*
 * Compares two versions.
 */ 
__EXPORT int acu_compareVersion(const ACU_Version* version1, const ACU_Version* version2);

/*
 * Formats the version.
 */
__EXPORT char* acu_formatVersion(char* buffer, size_t bufferSize, const ACU_Version* version);

#endif
