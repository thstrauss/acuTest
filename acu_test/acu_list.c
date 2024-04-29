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

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "acu_list.h"
#include "acu_util.h"

void acu_listInit(ACU_List* list, ACU_Destroy destroy) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

ACU_ListElement* acu_listHead(ACU_List* list) {
    return list->head;
}

ACU_ListElement* acu_listNext(ACU_ListElement* element) {
    return element->next;
}

ACU_List* acu_listMalloc(void)
{
    return (ACU_List*) acu_emalloc(sizeof(ACU_List));
}

static int acu_listSize(ACU_List* list) {
    return list->size;
}

static ACU_ListElement* acu_listElementMalloc(void) {
    return (ACU_ListElement*)acu_emalloc(sizeof(ACU_ListElement));
}

int acu_listAppend(ACU_List* list, void* data) {
    ACU_ListElement* newElement;
    ACU_ListElement* tailElement = list->tail;

    if ((newElement = acu_listElementMalloc()) == NULL) {
        return -1;
    }
    newElement->next = NULL; 
    newElement->data = data;

    if (acu_listSize(list) == 0) {
        list->head = newElement;
    }
    
    if (tailElement) {
        tailElement->next = newElement;
    }
    list->tail = newElement;

    list->size++;

    return 0;
}

static int acu_listRemoveHead(ACU_List* list, void** data) {
    ACU_ListElement* oldElement;
    if (acu_listSize(list) <= 0) {
        return -1;
    }
    
    *data = list->head->data;
    oldElement = list->head;
    list->head = list->head->next;
    if (acu_listSize(list) == 1) {
        list->tail = NULL;
    }

    free(oldElement);

    list->size--;
    
    return 0;
}

void acu_listDestroy(ACU_List* list) {
    void* data = NULL;

    while (acu_listSize(list) > 0) {
        if (acu_listRemoveHead(list, (void**)&data) == 0 && list->destroy) {
            list->destroy(data);
        }
    }
}
