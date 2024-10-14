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

void acu_initList(ACU_List* list, ACU_ListDestroyFunc destroy) {
    list->head = NULL;
    list->tail = NULL;
    list->destroy = destroy;
}

ACU_ListElement* acu_listHead(const ACU_List* list) {
    return list->head;
}

ACU_ListElement* acu_listNext(ACU_ListElement* element) {
    return element->next;
}


#define acu_listElementMalloc() ((ACU_ListElement*) acu_emalloc(sizeof(ACU_ListElement)))

int acu_appendList(ACU_List* list, const void* data) {
    ACU_ListElement* newElement = acu_listElementMalloc();
    if (newElement) {
        ACU_ListElement* tailElement = list->tail;
        if (tailElement) {
            tailElement->next = newElement;
        }
        list->tail = newElement;

        newElement->next = NULL;
        newElement->data = data;

        if (!list->head) {
            list->head = newElement;
        }
    }
    return !newElement;
}

int acu_insertHeadList(ACU_List* list, const void* data)
{
    ACU_ListElement* newListElement = acu_listElementMalloc();
    if (newListElement) {
        newListElement->data = (void*)data;

        if (!list->head) {
            list->tail = newListElement;
        }
        newListElement->next = list->head;
        list->head = newListElement;
    }
    return !newListElement;
}

int acu_insertNextList(ACU_List* list, ACU_ListElement* element, const void* data)
{
    ACU_ListElement *newListElement = acu_listElementMalloc();
    if (newListElement) {
        newListElement->data = (void*)data;

        if (!element) {
            if (!list->head) {
                list->tail = newListElement;
            }
            newListElement->next = list->head;
            list->head = newListElement;
        }
        else {
            newListElement->next = element->next;
            element->next = newListElement;
        }
    }
    return !newListElement;
}

int acu_removeNextList(ACU_List* list, ACU_ListElement* element, void** data)
{
    if (list->head) {
        ACU_ListElement* oldListElement;
        if (!element) {
            *data = (void*) list->head->data;
            oldListElement = list->head;
            list->head = list->head->next;
            if (!list->head) {
                list->tail = NULL;
            }
        } else {
            if (!element->next) {
                return -1;
            }
            *data = (void*) element->next->data;
            oldListElement = element->next;
            element->next = element->next->next;
            if (!element->next) {
                list->tail = element;
            }
        }
        acu_free(oldListElement);
        return 0;
    }
    return -1;
}

__EXPORT void acu_acceptList(const ACU_List* list, ACU_ListVisitor* visitor)
{
    ACU_ListElement* listElement = list->head;
    while (listElement) {
        visitor->visitor(listElement->data, visitor->context);
        listElement = listElement->next;
    }
}

size_t acu_getListSize(const ACU_List* list)
{
    size_t size=0;
    ACU_ListElement* listElement = list->head;
    while (listElement) {
        size++;
        listElement = listElement->next;
    }
    return size;
}

static int acu_listRemoveHead(ACU_List* list, const void** data) {
    if (list->head) {    
        ACU_ListElement* oldElement = list->head;
        *data = oldElement->data;
        list->head = oldElement->next;
        if (!list->head) {
            list->tail = NULL;
        }
        acu_free(oldElement);
    }
    return !list->head;
}

static int acu_listDropHead(ACU_List* list) {
    if (list->head) {
        ACU_ListElement* oldElement = list->head;
        void* data = (void*) oldElement->data;
        ACU_ListDestroyFunc* destroy = list->destroy;
        if (data && destroy) {
            destroy(data);
        }
        list->head = oldElement->next;
        acu_free(oldElement);

        if (!list->head) {
            list->tail = NULL;
        }
    }
    return !list->head;
}

void acu_destroyList(ACU_List* list) {
    while (list->head) {
        acu_listDropHead(list);
    }
}
