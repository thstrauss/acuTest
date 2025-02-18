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

void acu_initStaticAllocator(ACU_StaticAllocator* allocator, size_t itemSize, size_t maxElements, ACU_FreeContext* freeContext)
{

    allocator->maxElements = maxElements;
    allocator->freeElements = maxElements;

    if (maxElements > 0) {
        ACU_AllocatorItem* allocatorItem;
        int ptrAlignment = (itemSize % sizeof(void*)) ? 1 : 0;
        size_t i;
        size_t elementSize;
        itemSize = (itemSize / sizeof(void*) + ptrAlignment) * sizeof(void*);
        allocator->freeContext = freeContext;

        elementSize = offsetof(ACU_AllocatorItem, itemBuffer) + itemSize;

        allocator->buffer = acu_emalloc(elementSize * maxElements);
        allocator->nextItem = (ACU_AllocatorItem*)allocator->buffer;
        allocatorItem = allocator->nextItem;
        for (i = 0; i < maxElements; i++) {
            allocatorItem->allocator = allocator;
            allocatorItem->status = ACU_BUFFER_STATUS_FREE;
            allocatorItem->nextItem = (ACU_AllocatorItem*) (((char*)allocatorItem) + elementSize);
            allocatorItem = (ACU_AllocatorItem*) allocatorItem->nextItem;
        }
        ((ACU_AllocatorItem*)(allocator->buffer + elementSize * (maxElements - 1)))->nextItem = allocator->nextItem;
        }
    else {
        allocator->buffer = NULL;
        allocator->nextItem = NULL;
        allocator->freeContext = NULL;
    }
}

void acu_destroyStaticAllocator(ACU_StaticAllocator* allocator)
{
    acu_free(allocator->buffer);
}

void* acu_allocStaticAllocator(ACU_StaticAllocator* allocator)
{
    if (allocator && allocator->freeElements > 0) {
        ACU_AllocatorItem* allocatorItem = allocator->nextItem;

        while (allocatorItem->status == ACU_BUFFER_STATUS_OCCUPIED) {
            allocatorItem = (ACU_AllocatorItem*)allocatorItem->nextItem;
        }
        allocatorItem->status = ACU_BUFFER_STATUS_OCCUPIED;
        allocator->freeElements--;
        allocator->nextItem = (ACU_AllocatorItem*)allocatorItem->nextItem;
        return &allocatorItem->itemBuffer;
    }
    return NULL;
}

void acu_freeStaticAllocator(void* buffer)
{
    ACU_AllocatorItem* item = (ACU_AllocatorItem*)(((char*)buffer) - offsetof(ACU_AllocatorItem, itemBuffer));
    ACU_StaticAllocator* allocator = (ACU_StaticAllocator*) item->allocator;
    item->status = ACU_BUFFER_STATUS_FREE;
    allocator->nextItem = item;
    allocator->freeElements++;
    if (allocator->freeContext && allocator->freeElements == allocator->maxElements) {
        allocator->freeContext->freeFunc(allocator);
    }
}
