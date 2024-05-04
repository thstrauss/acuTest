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

typedef struct ACU_StackElement_ {
    void* data;
    struct ACU_StackElement_* next;
} ACU_StackElement;

typedef void ACU_Destroy(void* data);

typedef struct ACU_Stack_ {
    int size;

    ACU_Destroy* destroy;

    ACU_StackElement* head;
} ACU_Stack;

int acu_stackSize(const ACU_Stack* stack);

__EXPORT void acu_stackInit(ACU_Stack* stack, ACU_Destroy destroy);

void* acu_stackPeek(const ACU_Stack* stack);

__EXPORT ACU_Stack* acu_stackMalloc(void);

__EXPORT void acu_stackDestroy(ACU_Stack* stack);

__EXPORT int acu_stackPush(ACU_Stack* stack, void* data);

__EXPORT int acu_stackPop(ACU_Stack* stack, void** data);

#endif 
