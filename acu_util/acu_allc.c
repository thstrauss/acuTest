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

ACU_StaticAllocator* acu_initStaticAllocator(ACU_StaticAllocator* staticAllocator, size_t itemSize, size_t maxElements, ACU_FreeContext* freeContext)
{
    staticAllocator->maxElements = maxElements;
    staticAllocator->freeElements = maxElements;

    if (maxElements > 0) {
        ACU_AllocatorItem* allocatorItem;
        int ptrAlignment = (itemSize % sizeof(void*)) ? 1 : 0;
        size_t i;
        size_t elementSize;
        itemSize = (itemSize / sizeof(void*) + ptrAlignment) * sizeof(void*);
        staticAllocator->freeContext = freeContext;

        elementSize = offsetof(ACU_AllocatorItem, itemBuffer) + itemSize;

        staticAllocator->buffer = acu_emalloc(elementSize * maxElements);
        staticAllocator->nextItem = (ACU_AllocatorItem*)staticAllocator->buffer;
        allocatorItem = staticAllocator->nextItem;
        for (i = 0; i < maxElements; i++) {
            allocatorItem->staticAllocator = staticAllocator;
            allocatorItem->status = ACU_BUFFER_STATUS_FREE;
            allocatorItem->nextItem = (ACU_AllocatorItem*) (((char*)allocatorItem) + elementSize);
            allocatorItem = (ACU_AllocatorItem*) allocatorItem->nextItem;
        }
        ((ACU_AllocatorItem*)(staticAllocator->buffer + elementSize * (maxElements - 1)))->nextItem = staticAllocator->nextItem;
        }
    else {
        staticAllocator->buffer = NULL;
        staticAllocator->nextItem = NULL;
        staticAllocator->freeContext = NULL;
    }
    return staticAllocator;
}

void acu_destroyStaticAllocator(ACU_StaticAllocator* staticAllocator)
{
    acu_free(staticAllocator->buffer);
}

void* acu_allocStaticAllocator(ACU_StaticAllocator* staticAllocator)
{
    if (staticAllocator->freeElements) {
        ACU_AllocatorItem* allocatorItem = staticAllocator->nextItem;

        while (allocatorItem->status == ACU_BUFFER_STATUS_OCCUPIED) {
            allocatorItem = (ACU_AllocatorItem*)allocatorItem->nextItem;
            if (allocatorItem->status != ACU_BUFFER_STATUS_OCCUPIED) {
                break;
            }
            allocatorItem = (ACU_AllocatorItem*)allocatorItem->nextItem;
        }
        allocatorItem->status = ACU_BUFFER_STATUS_OCCUPIED;
        staticAllocator->freeElements--;
        staticAllocator->nextItem = (ACU_AllocatorItem*)allocatorItem->nextItem;
        return &allocatorItem->itemBuffer;
    }
    return NULL;
}

void acu_freeStaticAllocator(void* buffer)
{
    ACU_AllocatorItem* allocatorItem = (ACU_AllocatorItem*)(((char*)buffer) - offsetof(ACU_AllocatorItem, itemBuffer));
    ACU_StaticAllocator* staticAllocator = (ACU_StaticAllocator*) allocatorItem->staticAllocator;
    allocatorItem->status = ACU_BUFFER_STATUS_FREE;
    staticAllocator->nextItem = allocatorItem;
    staticAllocator->freeElements++;
    if (staticAllocator->freeContext && staticAllocator->freeElements == staticAllocator->maxElements) {
        staticAllocator->freeContext->freeFunc(staticAllocator);
    }
}
