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
#include <stddef.h>

typedef struct ACU_ListElement_ {
    struct ACU_ListElement_* next;
    const void* data;
} ACU_ListElement;

typedef void ACU_ListDestroyFunc(void* data);

typedef void ACU_ListVisitorFunc(const void* data, void* visitorContext);


typedef struct ACU_ListVisitor_ {
    ACU_ListVisitorFunc* visitor;
    void* context;
} ACU_ListVisitor;

typedef struct ACU_List_ {
    ACU_ListElement* head;
    ACU_ListElement* tail;
    ACU_ListDestroyFunc* destroy;
    struct ACU_AllocFuncs_* allocFuncs;
} ACU_List;

struct ACU_AllocFuncs_;

__EXPORT void acu_initList(ACU_List* list, ACU_ListDestroyFunc destroy, struct ACU_AllocFuncs_* allocFuncs);

__EXPORT ACU_ListElement* acu_listHead(const ACU_List* list);

__EXPORT ACU_ListElement* acu_listNext(ACU_ListElement* element);

/*
 * Allocates a block of the size of the ACU_List.
 * @remarks The allocated block shall be freed by the caller.
 */
#define acu_mallocList() ((ACU_List*) acu_emalloc(sizeof(ACU_List)))

__EXPORT void acu_destroyList(ACU_List* list);

__EXPORT const void* acu_appendList(ACU_List* list, const void* data);

__EXPORT const void* acu_insertHeadList(ACU_List* list, const void* data);

__EXPORT const void* acu_insertNextList(ACU_List* list, ACU_ListElement* element, const void* data);

__EXPORT void* acu_removeNextList(ACU_List* list, ACU_ListElement* element);

__EXPORT void acu_acceptList(const ACU_List* list, ACU_ListVisitor* visitor);

__EXPORT size_t acu_getListSize(const ACU_List* list);

#endif 
