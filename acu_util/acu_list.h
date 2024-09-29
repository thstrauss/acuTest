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

#pragma once
#ifndef _ACU_LIST_H_
#define _ACU_LIST_H_

#include "acu_cmmn.h"

typedef struct ACU_ListElement_ {
    struct ACU_ListElement_* next;
    const void* data;
} ACU_ListElement;

typedef void ACU_ListDestroyFunc(void* data);

typedef struct ACU_List_ {
    ACU_ListElement* head;
    ACU_ListElement* tail;
    int size;
    ACU_ListDestroyFunc* destroy;
} ACU_List;

__EXPORT void acu_listInit(ACU_List* list, ACU_ListDestroyFunc destroy);

__EXPORT ACU_ListElement* acu_listHead(ACU_List* list);

__EXPORT ACU_ListElement* acu_listNext(ACU_ListElement* element);

__EXPORT ACU_List* acu_listMalloc(void);

__EXPORT void acu_listDestroy(ACU_List* list);

__EXPORT int acu_listAppend(ACU_List* list, const void* data);

__EXPORT int acu_listInsertNext(ACU_List* list, ACU_ListElement* element, const void* data);

#endif 
