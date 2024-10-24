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

#include <stdlib.h>
#include <stddef.h>

#include "acu_stck.h"
#include "acu_tryc.h"
#include "acu_util.h"

static ACU_Stack* __acu_jmpBufFrames = NULL;

static ACU_Stack* __acu_initStackFrame(void) {
    ACU_Stack* frames = acu_stackMalloc();
    acu_initStack(frames, (ACU_StackDataDestroy*)NULL);
    return frames;
}

ACU_Stack* acu_getFrameStack(void)
{
    return __acu_jmpBufFrames ? __acu_jmpBufFrames : (__acu_jmpBufFrames = __acu_initStackFrame());
}

void acu_setFrameStack(ACU_Stack* jmpBufFrames)
{
    __acu_jmpBufFrames = jmpBufFrames;
}

void acu_freeFrameStack(void)
{
    if (__acu_jmpBufFrames) {
        acu_destroyStack(__acu_jmpBufFrames);
        acu_free(__acu_jmpBufFrames);
        __acu_jmpBufFrames = NULL;
    }
}

