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
    size_t i;
    ACU_AllocatorItem* allocatorItem;
    allocator->elementSize = offsetof(ACU_AllocatorItem, itemBuffer) + itemSize;
    allocator->maxElements = maxElements;
    allocator->freeElements = maxElements;
    allocator->context = context;
    allocator->handleContextFunc = handleContext;

    allocator->buffer = acu_emalloc(allocator-> elementSize * maxElements);
    allocator->next = (ACU_AllocatorItem*)allocator->buffer;
    allocatorItem = allocator->next;
    for (i = 0; i < maxElements; i++) {
        allocatorItem->allocator = allocator;
        allocatorItem->status = ACU_BUFFER_STATUS_FREE;
        ((char*)allocatorItem) += allocator->elementSize;
    }
    allocator->last = allocatorItem;
}

void acu_destroyStaticAllocator(ACU_StaticAllocator* allocator)
{
    acu_free(allocator->buffer);
}

void* acu_allocStaticAllocator(ACU_StaticAllocator* allocator)
{
    if (allocator->freeElements) {
        size_t elementSize = allocator->elementSize;
        ACU_AllocatorItem* allocatorItem = allocator->next;
        while (1) {
            if (allocatorItem >= allocator->last) {
                allocatorItem = (ACU_AllocatorItem*)allocator->buffer;
            }
            if (allocatorItem->status == ACU_BUFFER_STATUS_FREE) {
                break;
            }
            ((char*)allocatorItem) += elementSize;
        }
        allocatorItem->status = ACU_BUFFER_STATUS_OCCUPIED;
        allocator->freeElements--;
        allocator->next = (ACU_AllocatorItem*)((char*)allocatorItem + elementSize);
        return &allocatorItem->itemBuffer;
    }
    return NULL;
}

void acu_freeStaticAllocator(void* buffer)
{
    ACU_AllocatorItem* item;
    ACU_StaticAllocator* allocator;
    item = (ACU_AllocatorItem*)(((char*)buffer) - offsetof(ACU_AllocatorItem, itemBuffer));
    item->status = ACU_BUFFER_STATUS_FREE;
    allocator = item->allocator;
    allocator->freeElements++;
    allocator->next = item;
    if (allocator->freeElements == allocator->maxElements && allocator->context && allocator->handleContextFunc) {
        allocator->handleContextFunc(item->allocator);
    }
}
