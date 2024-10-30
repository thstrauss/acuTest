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
#ifndef _ACU_TEST_CASE_H_
#define _ACU_TEST_CASE_H_

#include "acu_eenv.h"
#include "acu_rslt.h"

struct ACU_Fixture_;
struct ACU_Progress_;

typedef void ACU_TestFunc(ACU_ExecuteEnv* environment, const void* context);

/* Defines the individual test cases.*/
typedef struct ACU_TestCase_ {
    ACU_TestFunc* testFunc;
    char* name;
    const struct ACU_Fixture_* fixture;
    ACU_Result result;
    const void* context;
    const struct ACU_Progress_ * progress;
} ACU_TestCase;

#endif
