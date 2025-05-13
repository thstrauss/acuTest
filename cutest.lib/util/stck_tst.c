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

#include "stck_tst.h"

#include <acu_eenv.h>
#include <acu_asrt.h>
#include <acu_rprt.h>

#include <acu_stck.h>

static void emptyStackTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_Stack stack;
    acu_initStack(&stack, (ACU_StackDataDestroy*) NULL);

    ACU_assert(environment, size_t, Equal, acu_getStackSize(&stack), 0, "stack size not 0");

    UNUSED(environment);
    UNUSED(context);
}

static void stackTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_Stack stack;
    int i;
    int values[10];

    acu_initStack(&stack, (ACU_StackDataDestroy*) NULL);

    for (i = 0; i < 10; i++) {
        values[i] = i;
        acu_pushStack(&stack, &values[i]);
    }

    ACU_assert(environment, size_t, Equal, acu_getStackSize(&stack), 10, "stack size not 10");

    ACU_assert(environment, int, Equal, *(int*) acu_peekStack(&stack), 9, "not expected value on stack")

    ACU_assert(environment, int, Equal, *(int*)acu_popStack(&stack), 9, "not expected value on stack")

    ACU_assert(environment, int, Equal, *(int*)acu_peekStack(&stack), 8, "not expected value on stack")

    acu_dropStack(&stack);

    ACU_assert(environment, int, Equal, *(int*)acu_peekStack(&stack), 7, "not expected value on stack")

    acu_destroyStack(&stack);

    UNUSED(environment);
    UNUSED(context);
}

__EXPORT ACU_Fixture* stackFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "stack tests");

    acu_addTestCase(fixture, "emptyStackTest", emptyStackTest);
    acu_addTestCase(fixture, "stackTest", stackTest);

    return fixture;
}
