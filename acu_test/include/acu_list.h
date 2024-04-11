/*
 * Copyright (c) 2024 Thomas Strau�
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

#include <stdlib.h>

typedef struct ACU_ListElement_ {
    void* data;
    struct ACU_ListElement_* next;
} ACU_ListElement;

typedef void ACU_Destroy(void* data);

typedef struct ACU_List_ {
    int size;

    ACU_Destroy* destroy;

    ACU_ListElement* head;
    ACU_ListElement* tail;
} ACU_List;

void acu_listInit(ACU_List* list, ACU_Destroy destroy);

ACU_ListElement* acu_listHead(ACU_List* list);

ACU_ListElement* acu_listNext(ACU_ListElement* element);

ACU_List* acu_listMalloc(void);

void acu_listDestroy(ACU_List* list);

int acu_listAppend(ACU_List* list, void* data);

#endif 