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
/* Copyright (C) 2009-2015 Francesco Nidito
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _ACU_TRY_THROW_CATCH_H_
#define _ACU_TRY_THROW_CATCH_H_

#include <setjmp.h>
#include "acu_stck.h"

typedef enum ACU_Exception_ {
    ACU_EXCEPTION_ABORTED = 3,
    ACU_EXCEPTION_DEFAULT = 0xFFFF,
} ACU_Exception;

typedef struct ACU_Frame_ {
    jmp_buf exceptionBuf;
    int exception;
} ACU_Frame;

typedef void (ACU_setFrameStackFunc)(ACU_Stack* jmpBufFrames);

__EXPORT ACU_Stack* acu_getFrameStack(void);
void acu_setFrameStack(ACU_Stack* jmpBufFrames);

 /* For the full documentation and explanation of the code below, please refer to
  * http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
  */

#define ACU_TRY_CTX(CONTEXT) do { \
    ACU_Frame _##CONTEXT##_Frame; \
    _##CONTEXT##_Frame.exception = 0; \
    acu_stackPush(acu_getFrameStack(), &_##CONTEXT##_Frame); \
    switch(setjmp(_##CONTEXT##_Frame.exceptionBuf) ) { \
         case 0: while(1) {

#define ACU_CATCH_CTX(CONTEXT, x) break; case (x): 

#define ACU_FINALLY_CTX(CONTEXT) break; } default: 

#define ACU_ETRY_CTX(CONTEXT) \
    { \
        ACU_Frame* f; \
        acu_stackDrop(acu_getFrameStack()); \
        f = acu_stackPeek(acu_getFrameStack()); \
        if (f && f->exception != 0) { \
            longjmp(f->exceptionBuf, f->exception != 0 ? f->exception : ACU_EXCEPTION_DEFAULT); \
        } \
    } \
    break; } } while(0);

#define ACU_THROW_CTX(CONTEXT, x) { \
    ACU_Frame* f = acu_stackPeek(acu_getFrameStack()); \
    _##CONTEXT##_Frame.exception=(x); \
    longjmp(f->exceptionBuf, f->exception != 0 ? f->exception : ACU_EXCEPTION_DEFAULT);}

#define ACU_TRY ACU_TRY_CTX(exception)
#define ACU_THROW(x) ACU_THROW_CTX(exception, x)
#define ACU_CATCH(x) ACU_CATCH_CTX(exception, x)
#define ACU_FINALLY ACU_FINALLY_CTX(exception)
#define ACU_ETRY ACU_ETRY_CTX(exception)

#endif /*!_ACU_TRY_THROW_CATCH_H_*/