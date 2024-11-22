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

#include "acu_dall.h"
#include "acu_util.h"

#include <stddef.h>

void __destroyStaticAllocators(ACU_StaticAllocator* allocator) {
    acu_destroyStaticAllocator(allocator);
    acu_free(allocator);
}

void acu_initAllocator(ACU_DynamicAllocator* allocator, size_t itemSize, size_t maxElements)
{
    allocator->staticAllocators = acu_emalloc(sizeof(ACU_List));
    acu_initList(allocator->staticAllocators, (ACU_ListDestroyFunc*) __destroyStaticAllocators);
    allocator->itemSize = itemSize;
    allocator->maxElements = maxElements;
}

void acu_destroyAllocator(ACU_DynamicAllocator* allocator)
{
    acu_destroyList(allocator->staticAllocators);
    acu_free(allocator->staticAllocators);
}

void* acu_allocAllocator(ACU_DynamicAllocator* allocator)
{
    ACU_ListElement* staticAllocatorElement = allocator->staticAllocators->head;
    ACU_StaticAllocator* staticAllocator;
    while (staticAllocatorElement) {
        staticAllocator = (ACU_StaticAllocator*)staticAllocatorElement->data;
        if (staticAllocator->occupiedElements < staticAllocator->maxElements) {
            void* buffer = acu_allocStaticAllocator((ACU_StaticAllocator*)staticAllocatorElement->data);
            if (buffer) {
                return buffer;
            }
        }
        staticAllocatorElement = staticAllocatorElement->next;
    }
    staticAllocator = acu_emalloc(sizeof(ACU_StaticAllocator));
    acu_initStaticAllocator(staticAllocator, allocator->itemSize, allocator->maxElements);
    acu_insertHeadList(allocator->staticAllocators, staticAllocator);
    return acu_allocStaticAllocator(staticAllocator);
}

void acu_freeAllocator(void* buffer)
{
    acu_freeStaticAllocator(buffer);
}
