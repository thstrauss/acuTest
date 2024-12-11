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

#ifndef __dynamic_allocator__
#define __dynamic_allocator__

#include "acu_list.h"
#include "acu_allc.h"

typedef struct ACU_DynamicAllocator_ {
    ACU_StaticAllocator* lastUsedAllocator;
    ACU_List* staticAllocators;
    size_t itemSize;
    size_t maxBucketElements;
} ACU_DynamicAllocator;

__EXPORT void acu_initAllocator(ACU_DynamicAllocator* allocator, size_t itemSize, size_t maxBucketElements);

__EXPORT void acu_destroyAllocator(ACU_DynamicAllocator* allocator);

__EXPORT void* acu_allocAllocator(ACU_DynamicAllocator* allocator);

#define acu_freeAllocator(buffer) acu_freeStaticAllocator(buffer)

__EXPORT size_t acu_getAllocatedElements(ACU_DynamicAllocator* allocator);

#endif // !__dynamic_allocator__
