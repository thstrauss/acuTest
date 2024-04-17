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
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

#include <stdio.h>

#include <acu_cmmn.h>
#include <acu_fxtr.h>
#include <acu_list.h>

typedef struct ACU_Suite_ {
    char* name;
    ACU_List* testFixtures;
} ACU_Suite;

typedef void ACU_SuiteDestroyFunc(ACU_Suite* suite);
typedef int ACU_SuiteExecuteFunc(const ACU_Suite* suite, ACU_ProgressFunc progress);
typedef void* ACU_SuiteReportFunc(const ACU_Suite* suite, void* context, ACU_ReportFunc report);

__EXPORT void acu_suiteAddFixture(ACU_Suite* suite, ACU_Fixture* fixture);

__EXPORT void acu_suiteInit(ACU_Suite* suite, const char* name);

__EXPORT int acu_suiteExecute(const ACU_Suite* suite, ACU_ProgressFunc progress);

__EXPORT void* acu_suiteReport(const ACU_Suite* suite, void* context, ACU_ReportFunc report);

__EXPORT ACU_Suite* acu_suiteMalloc(void);

__EXPORT void acu_suiteDestroy(ACU_Suite* suite);


#endif