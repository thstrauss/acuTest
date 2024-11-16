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

void ACU_initAllocator(ACU_Allocator* allocator, size_t itemSize, size_t maxElements)
{
    allocator->itemSize = itemSize;
    allocator->elementSize = offsetof(ACU_AllocatorItem, itemBuffer) + itemSize;
    allocator->maxElements = maxElements;
    allocator->occupiedElements = 0;

    allocator->buffer = __acu_emalloc(allocator-> elementSize * maxElements);
    memset(allocator->buffer, 0, allocator->elementSize * maxElements);
}

void ACU_destroyAllocator(ACU_Allocator* allocator)
{
    acu_free(allocator->buffer);
}

void* ACU_allocAllocator(ACU_Allocator* allocator)
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

void ACU_freeAllocator(void* buffer)
{
    char* buf = buffer;
    buf -= offsetof(ACU_AllocatorItem, itemBuffer);
    ((ACU_AllocatorItem*)buf)->status = ACU_BUFFER_STATUS_FREE;
    ((ACU_AllocatorItem*)buf)->allocator->occupiedElements--;
}
