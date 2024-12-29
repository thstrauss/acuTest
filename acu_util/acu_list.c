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

#include "acu_list.h"
#include "acu_util.h"

static void __acu_defaultListDestroy(void* data) {
    UNUSED(data);
}

static ACU_AllocFuncs __acu_defaultAllocFuncs = {
    malloc,
    free 
};

void acu_initList(ACU_List* list, ACU_ListDestroyFunc destroy, struct ACU_AllocFuncs_* allocFuncs) {
    list->head = NULL;
    list->tail = NULL;
    if (destroy) {
        list->destroy = destroy;
    }
    else {
        list->destroy = __acu_defaultListDestroy;
    }
    if (allocFuncs) {
        list->allocFuncs = allocFuncs;
    }
    else {
        list->allocFuncs = &__acu_defaultAllocFuncs;
    }
}

ACU_ListElement* acu_listHead(const ACU_List* list) {
    return list->head;
}

ACU_ListElement* acu_listNext(ACU_ListElement* element) {
    return element->next;
}

#define ACU__LISTELEMENTMALLOC() ((ACU_ListElement*) acu_emalloc(sizeof(ACU_ListElement)))

const void* acu_appendList(ACU_List* list, const void* data) {
    ACU_ListElement* newElement = ACU__LISTELEMENTMALLOC();
    if (newElement) {
    	ACU_ListElement* tailElement;
        newElement->next = NULL;
        newElement->data = data;
        if (!list->head) {
            list->head = newElement;
        }

        tailElement = list->tail;
        if (tailElement) {
            tailElement->next = newElement;
        }
        list->tail = newElement;

        return data;
    }
    return NULL;
}

const void* acu_insertHeadList(ACU_List* list, const void* data)
{
    ACU_ListElement* newListElement = ACU__LISTELEMENTMALLOC();
    if (newListElement) {
        newListElement->data = (void*)data;

        if (!list->head) {
            list->tail = newListElement;
        }
        newListElement->next = list->head;
        list->head = newListElement;
        return data;
    }
    return NULL;
}

const void* acu_insertNextList(ACU_List* list, ACU_ListElement* element, const void* data)
{
    ACU_ListElement *newListElement = ACU__LISTELEMENTMALLOC();
    if (newListElement) {

        if (element) {
            newListElement->next = element->next;
            element->next = newListElement;
        }
        else {
            if (!list->head) {
                list->tail = newListElement;
            }
            newListElement->next = list->head;
            list->head = newListElement;
        }
        newListElement->data = (void*)data;
        return data;
    }
    return NULL;
}

void* acu_removeNextList(ACU_List* list, ACU_ListElement* element)
{
    void* data;
    ACU_ListElement* oldListElement;
    if (element) {
        if (!element->next) {
            return NULL;
        }
        oldListElement = element->next;
        data = (void*)oldListElement->data;
        element->next = oldListElement->next;
        if (!element->next) {
            list->tail = element;
        }
    } else {
        oldListElement = list->head;
        data = (void*)oldListElement->data;
        list->head = oldListElement->next;
        if (!list->head) {
            list->tail = NULL;
        }
    }
    acu_free(oldListElement);
    return data;
}

void acu_acceptList(const ACU_List* list, ACU_ListVisitor* visitor)
{
	void* context = visitor->context;
    ACU_ListVisitorFunc* visitorFunc = visitor->visitor;
	ACU_ListElement* listElement = list->head;
    while (listElement) {
        visitorFunc(listElement->data, context);
        listElement = listElement->next;
    }
}

static void acu_countVisitor(const void* data, void* visitorContext) {
    (*(size_t*) visitorContext)++;
    UNUSED(data);
}

size_t acu_getListSize(const ACU_List* list)
{
    size_t size=0;
    ACU_ListVisitor visitor;
    visitor.visitor = acu_countVisitor;
    visitor.context = &size;
    acu_acceptList(list, &visitor);
    return size;
}

void acu_destroyList(ACU_List* list) {
    while (list->head) {
        const void* data;
        ACU_ListElement* oldElement = list->head;
        data = oldElement->data;
        if (data) {
            list->destroy((void*) data);
        }
        list->head = oldElement->next;
        acu_free(oldElement);
    }
    list->tail = NULL;
}
