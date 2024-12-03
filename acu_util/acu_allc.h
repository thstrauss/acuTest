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

struct ACU_StaticAllocator_;

typedef struct ACU_AllocatorItem_ {
    ACU_BufferStatus status;
    struct ACU_StaticAllocator_* allocator;
    char* itemBuffer;
} ACU_AllocatorItem;

typedef void ACU_HandleContextFunc(struct ACU_StaticAllocator_* allocator);

typedef struct ACU_StaticAllocator_ {
    char* buffer;
    size_t itemSize;
    size_t elementSize;
    size_t maxElements;
    size_t occupiedElements;
    ACU_AllocatorItem* last;
    void* context;
    ACU_HandleContextFunc* handleContextFunc;
} ACU_StaticAllocator;

__EXPORT void acu_initStaticAllocator(ACU_StaticAllocator* allocator, size_t itemSize, size_t maxElements, ACU_HandleContextFunc* handleContext, void* context);

__EXPORT void acu_destroyStaticAllocator(ACU_StaticAllocator* allocator);

__EXPORT void* acu_allocStaticAllocator(ACU_StaticAllocator* allocator);

__EXPORT void acu_freeStaticAllocator(void* buffer);

#endif // !ACU_ALLOCATOR
