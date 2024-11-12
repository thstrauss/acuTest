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
    allocator->maxElements = maxElements;

    allocator->buffer = __acu_emalloc((itemSize + sizeof(ACU_BufferStatus))* maxElements);
    memset(allocator->buffer, 0, (itemSize + sizeof(ACU_BufferStatus)) * maxElements);
}

void ACU_destroyAllocator(ACU_Allocator* allocator)
{
    acu_free(allocator->buffer);
}

void* ACU_allocAllocator(ACU_Allocator* allocator)
{
    ACU_AllocatorItem* allocatorItem = allocator->buffer;
    ACU_AllocatorItem* allocatorEnd = allocator->buffer + (allocator->itemSize + sizeof(ACU_BufferStatus)) * allocator->maxElements;
    while (allocatorItem < allocatorEnd) {
        if (allocatorItem->status == ACU_BUFFER_STATUS_FREE) {
            allocatorItem->status = ACU_BUFFER_STATUS_OCCUPIED;
            return &allocatorItem->itemBuffer;
        }
        ((char*) allocatorItem) += (allocator->itemSize + sizeof(ACU_BufferStatus));
    }
    return NULL;
}

void ACU_freeAllocator(ACU_Allocator* allocator, void* buffer)
{
    char* buf = buffer;
    buf -= offsetof(ACU_AllocatorItem, itemBuffer);
    ((ACU_AllocatorItem*)buf)->status = ACU_BUFFER_STATUS_FREE;
}
