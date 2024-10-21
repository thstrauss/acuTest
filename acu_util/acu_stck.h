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
#ifndef _ACU_STACK_H_
#define _ACU_STACK_H_

#include "acu_cmmn.h"
#include <stddef.h>

typedef struct ACU_StackElement_ {
    struct ACU_StackElement_* next;
    void* data;
} ACU_StackElement;

typedef void ACU_StackDataDestroy(void* data);

typedef struct ACU_Stack_ {
    ACU_StackElement* head;
    ACU_StackDataDestroy* destroy;
} ACU_Stack;

__EXPORT size_t acu_getStackSize(const ACU_Stack* stack);

__EXPORT void acu_initStack(ACU_Stack* stack, ACU_StackDataDestroy destroy);

__EXPORT void* acu_peekStack(const ACU_Stack* stack);

#define acu_stackMalloc() ((ACU_Stack*) acu_emalloc(sizeof(ACU_Stack)));

__EXPORT void acu_destroyStack(ACU_Stack* stack);

__EXPORT int acu_pushStack(ACU_Stack* stack, void* data);
__EXPORT void acu_pushStackElement(ACU_Stack* stack, ACU_StackElement* element);

__EXPORT void* acu_popStack(ACU_Stack* stack);
__EXPORT int acu_dropStack(ACU_Stack* stack);
__EXPORT void acu_dropStackElement(ACU_Stack* stack);

#endif 
