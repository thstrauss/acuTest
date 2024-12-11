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

static void __destroyStaticAllocators(ACU_StaticAllocator* allocator) {
    acu_destroyStaticAllocator(allocator);
    acu_free(allocator);
}

void acu_initAllocator(ACU_DynamicAllocator* allocator, size_t itemSize, size_t maxBucketElements)
{
    allocator->staticAllocators = acu_mallocList();
    allocator->lastUsedAllocator = NULL;
    acu_initList(allocator->staticAllocators, (ACU_ListDestroyFunc*) __destroyStaticAllocators);
    allocator->itemSize = itemSize;
    allocator->maxBucketElements = maxBucketElements;
}

void acu_destroyAllocator(ACU_DynamicAllocator* allocator)
{
    acu_destroyList(allocator->staticAllocators);
    acu_free(allocator->staticAllocators);
}

static void __handleContext(ACU_StaticAllocator* staticAllocator) {
    ACU_DynamicAllocator* allocator = (ACU_DynamicAllocator*)staticAllocator->context;
    ACU_ListElement* element = allocator->staticAllocators->head;
    ACU_ListElement* prev = NULL;
    while (element) {
        if (element->data == staticAllocator) {
            acu_removeNextList(allocator->staticAllocators, prev);
            __destroyStaticAllocators(staticAllocator);
            allocator->lastUsedAllocator = NULL;
            break;
        }
        prev = element;
        element = element->next;
    }
}

void* acu_allocAllocator(ACU_DynamicAllocator* allocator)
{
    ACU_ListElement* staticAllocatorElement;
    ACU_StaticAllocator* staticAllocator = allocator->lastUsedAllocator;
    if (staticAllocator) {
        void* buffer = acu_allocStaticAllocator(staticAllocator);
        if (buffer) {
            return buffer;
        }
    }
    staticAllocatorElement = allocator->staticAllocators->head;
    while (staticAllocatorElement) {
        void* buffer;
        staticAllocator = (ACU_StaticAllocator*)staticAllocatorElement->data;
        buffer = acu_allocStaticAllocator(staticAllocator);
        if (buffer) {
            allocator->lastUsedAllocator = staticAllocator;
            return buffer;
        }
        staticAllocatorElement = staticAllocatorElement->next;
    }
    staticAllocator = acu_emalloc(sizeof(ACU_StaticAllocator));
    acu_initStaticAllocator(staticAllocator, allocator->itemSize, allocator->maxBucketElements, __handleContext, allocator);
    acu_insertHeadList(allocator->staticAllocators, staticAllocator);
    allocator->lastUsedAllocator = staticAllocator;
    return acu_allocStaticAllocator(staticAllocator);
}

static void __accumulateAllocatedElements(const void* data, void* visitorContext) {
    (*(size_t*)visitorContext) += ((ACU_StaticAllocator*)data)->maxElements - ((ACU_StaticAllocator*) data)->freeElements;
}

size_t acu_getAllocatedElements(ACU_DynamicAllocator* allocator)
{
    size_t count = 0;
    ACU_ListVisitor visitor;
    visitor.context = &count;
    visitor.visitor = __accumulateAllocatedElements;
    
    acu_acceptList(allocator->staticAllocators, &visitor);
    return count;
}
