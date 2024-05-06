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
#ifndef _ACU_TEST_RESULT_H_
#define _ACU_TEST_RESULT_H_

#include <time.h>
#include "acu_cmmn.h"

typedef struct ACU_Result_ {
    enum ACU_TestResult status;
    char* message;
    char* file;
    int line;
    clock_t start;
    clock_t end;
} ACU_Result;

__EXPORT ACU_Result* acuTest_resultMalloc(void);
__EXPORT void acuTest_resultInit(ACU_Result* result);
__EXPORT void acuTest_resultDestroy(ACU_Result* result);
__EXPORT enum ACU_TestResult acuTest_calcResult(enum ACU_TestResult aggregatedresult, enum ACU_TestResult result);

#endif
