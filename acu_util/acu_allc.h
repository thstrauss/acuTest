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

#ifndef ACU_ALLOCATOR
#define ACU_ALLOCATOR

#include "acu_cmmn.h"

#include <stddef.h>

typedef enum ACU_BufferStatus_ {
    ACU_BUFFER_STATUS_FREE,
    ACU_BUFFER_STATUS_OCCUPIED,
} ACU_BufferStatus;

typedef struct ACU_AllocatorItem_ {
    ACU_BufferStatus status;
    char* itemBuffer;
} ACU_AllocatorItem;

typedef struct ACU_Allocator_ {
    char* buffer;
    size_t itemSize;
    size_t maxElements;
} ACU_Allocator;

__EXPORT void ACU_initAllocator(ACU_Allocator* allocator, size_t itemSize, size_t maxElements);

__EXPORT void ACU_destroyAllocator(ACU_Allocator* allocator);

__EXPORT void* ACU_allocAllocator(ACU_Allocator* allocator);

__EXPORT void ACU_freeAllocator(ACU_Allocator* allocator, void* buffer);

#endif // !ACU_ALLOCATOR
