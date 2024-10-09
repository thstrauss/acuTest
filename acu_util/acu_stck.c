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

#include "acu_stck.h"
#include "acu_util.h"

size_t acu_stackSize(const ACU_Stack* stack)
{
    return stack->size;
}

void acu_stackInit(ACU_Stack* stack, ACU_StackDataDestroy destroy) {
    stack->size = 0;
    stack->destroy = destroy;
    stack->head = NULL;
}

void* acu_stackPeek(const ACU_Stack* stack)
{
    return (!stack->size) ? NULL : stack->head->data;
}

ACU_Stack* acu_stackMalloc(void)
{
    return acu_emalloc(sizeof(ACU_Stack));
}

void acu_stackDestroy(ACU_Stack* stack)
{
    while (stack->size) {
        acu_stackDrop(stack);
    }
}

int acu_stackPush(ACU_Stack* stack, void* data)
{
    ACU_StackElement* newElement = acu_emalloc(sizeof(ACU_StackElement));
    if (newElement) {
        newElement->data = data;

        newElement->next = stack->head;
        stack->head = newElement;

        stack->size++;
        return 0;
    }
    return -1;
}

int acu_stackPop(ACU_Stack* stack, void** data)
{
    ACU_StackElement* oldElement;

    if (!stack->size) {
        if (data) {
            *data = NULL;
        }
        return -1;
    }

    oldElement = stack->head;
    if (data) {
        *data = oldElement->data;
    }

    stack->head = stack->head->next;

    acu_free(oldElement);

    stack->size--;

    return 0;
}

__EXPORT int acu_stackDrop(ACU_Stack* stack)
{
    if (stack->size) {
        ACU_StackElement* oldElement = stack->head;
        stack->head = oldElement->next;

        if (stack->destroy && oldElement->data) {
            stack->destroy(oldElement->data);
        }
        acu_free(oldElement);
        stack->size--;

        return 0;
    }
    return -1;
}

