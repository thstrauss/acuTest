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

ACU_DynamicAllocator* dynamicAllocator = NULL;

static void __destroyStaticAllocators(ACU_StaticAllocator* staticAllocator) {
    acu_destroyStaticAllocator(staticAllocator);
    acu_free(staticAllocator);
}

static void __freeAllocator(ACU_StaticAllocator* staticAllocator) {
    ACU_DynamicAllocator* dynamicAllocator = (ACU_DynamicAllocator*) staticAllocator->freeContext->context;
    ACU_ListElement* element = dynamicAllocator->staticAllocators->head;
    ACU_ListElement* prev = NULL;
    while (element) {
        if (element->data == staticAllocator) {
            acu_removeNextList(dynamicAllocator->staticAllocators, prev);
            __destroyStaticAllocators(staticAllocator);
            dynamicAllocator->lastUsedStaticAllocator = (prev ? (ACU_StaticAllocator*) prev->data : NULL);
            break;
        }
        prev = element;
        element = element->next;
    }
}

extern struct ACU_AllocFuncs_* acu_defaultAllocFuncs;

void acu_initAllocator(ACU_DynamicAllocator* dynamicAllocator, size_t itemSize, size_t maxBucketElements)
{
    dynamicAllocator->staticAllocators = acu_mallocList();
    dynamicAllocator->lastUsedStaticAllocator = NULL;
    acu_initList(dynamicAllocator->staticAllocators, (ACU_ListDestroyFunc*) __destroyStaticAllocators, acu_defaultAllocFuncs);
    dynamicAllocator->itemSize = itemSize;
    dynamicAllocator->maxBucketElements = maxBucketElements;
    dynamicAllocator->freeContext = acu_emalloc(sizeof(ACU_FreeContext));
    dynamicAllocator->freeContext->context = dynamicAllocator;
    dynamicAllocator->freeContext->freeFunc = __freeAllocator;
}

void acu_destroyAllocator(ACU_DynamicAllocator* allocator)
{
    acu_destroyList(allocator->staticAllocators);
    acu_free(allocator->freeContext);
    acu_free(allocator->staticAllocators);
}

static void* __acu_allocFromOtherAllocator(ACU_DynamicAllocator* dynamicAllocator) {
    ACU_ListElement* staticAllocatorElement = dynamicAllocator->staticAllocators->head;
    while (staticAllocatorElement) {
        void* buffer = acu_allocStaticAllocator((ACU_StaticAllocator*) staticAllocatorElement->data);
        if (buffer) {
            dynamicAllocator->lastUsedStaticAllocator = (ACU_StaticAllocator*) staticAllocatorElement->data;
            return buffer;
        }
        staticAllocatorElement = staticAllocatorElement->next;
    }
    dynamicAllocator->lastUsedStaticAllocator = acu_initStaticAllocator(acu_emalloc(sizeof(ACU_StaticAllocator)), dynamicAllocator->itemSize, dynamicAllocator->maxBucketElements, dynamicAllocator->freeContext);
    acu_insertHeadList(dynamicAllocator->staticAllocators, dynamicAllocator->lastUsedStaticAllocator);
    return acu_allocStaticAllocator(dynamicAllocator->lastUsedStaticAllocator);

}

void* acu_allocAllocator(ACU_DynamicAllocator* dynamicAllocator)
{
    if (dynamicAllocator->lastUsedStaticAllocator) {
        void* buffer = acu_allocStaticAllocator(dynamicAllocator->lastUsedStaticAllocator);
        if (buffer) {
            return buffer;
        }
    }
    return __acu_allocFromOtherAllocator(dynamicAllocator);
}

static void __accumulateAllocatedElements(const void* data, void* visitorContext) {
    ACU_StaticAllocator* staticAllocator = (ACU_StaticAllocator*) data;
    (*(size_t*)visitorContext) += staticAllocator->maxElements - staticAllocator->freeElements;
}

size_t acu_getAllocatedElements(ACU_DynamicAllocator* dynamicAllocator)
{
    size_t count = 0;
    ACU_ListVisitor visitor;
    visitor.context = &count;
    visitor.visitor = __accumulateAllocatedElements;
    
    acu_acceptList(dynamicAllocator->staticAllocators, &visitor);
    return count;
}
