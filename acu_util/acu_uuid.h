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
#ifndef _ACU_UUID_H_
#define _ACU_UUID_H_

#include "acu_cmmn.h"

#ifdef __TOS__
typedef unsigned long uint32_t;
#else
#include <stdint.h>
#endif

typedef union ACU_UUID_ {
    uint32_t longs[4];
    unsigned char bytes[16];
} ACU_UUID;

__EXPORT void acu_initUuid(ACU_UUID* uuid);
__EXPORT int acu_compareUuid(const ACU_UUID* uuid1, const ACU_UUID* uuid2);

__EXPORT void acu_formatUuid(char* buffer,const ACU_UUID* uuid);


#endif
