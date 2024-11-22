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

#include "acu_allc.h"
#include "acu_util.h"

#include <stdio.h>

void acu_initStaticAllocator(ACU_StaticAllocator* allocator, size_t itemSize, size_t maxElements, ACU_HandleContextFunc* handleContext, void* context)
{
    allocator->itemSize = itemSize;
    allocator->elementSize = offsetof(ACU_AllocatorItem, itemBuffer) + itemSize;
    allocator->maxElements = maxElements;
    allocator->occupiedElements = 0;
    allocator->context = context;
    allocator->handleContextFunc = handleContext;

    allocator->buffer = acu_emalloc(allocator-> elementSize * maxElements);
    memset(allocator->buffer, 0, allocator->elementSize * maxElements);
}

void acu_destroyStaticAllocator(ACU_StaticAllocator* allocator)
{
    acu_free(allocator->buffer);
}

void* acu_allocStaticAllocator(ACU_StaticAllocator* allocator)
{
    ACU_AllocatorItem* allocatorItem = (ACU_AllocatorItem*) allocator->buffer;
    if (allocator->occupiedElements < allocator->maxElements) {
        while (1) {
            if (allocatorItem->status == ACU_BUFFER_STATUS_FREE) {
                allocatorItem->status = ACU_BUFFER_STATUS_OCCUPIED;
                allocatorItem->allocator = allocator;
                allocator->occupiedElements++;
                return &allocatorItem->itemBuffer;
            }
            ((char*) allocatorItem) += allocator->elementSize;
        }
    }
    return NULL;
}

void acu_freeStaticAllocator(void* buffer)
{
    char* buf = buffer;
    ACU_AllocatorItem* item;
    ACU_StaticAllocator* allocator;
    buf -= offsetof(ACU_AllocatorItem, itemBuffer);
    item = (ACU_AllocatorItem*)buf;
    item->status = ACU_BUFFER_STATUS_FREE;
    allocator = item->allocator;
    allocator->occupiedElements--;
    if (allocator->occupiedElements == 0 && allocator->context && allocator->handleContextFunc) {
        allocator->handleContextFunc(item->allocator);
    }
}
