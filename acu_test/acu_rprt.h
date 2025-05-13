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
#ifndef __ACU_RPRT_H__
#define __ACU_RPRT_H__

#include <acu_list.h>
#include "acu_cmmn.h"

struct ACU_TestCase_;
 /*
  * The ACU_TestCase is the test case that is executed.
  */
typedef int ACU_MatchFunc(const struct ACU_TestCase_* testCase, void* matchContext);
/*
 * The ACU_TestCase is the test case that is executed.
 */
typedef void ACU_ProgressFunc(const struct ACU_TestCase_* testCase, void* progressContext);
/*
 * The ACU_TestCase is the test case that is executed.
 */
typedef void ACU_ReportVisitorFunc(const struct ACU_TestCase_* testCase, void* context);

/*
 * The ACU_TestCase is the test case that is executed.
 */
__EXPORT void acu_progress(const struct ACU_TestCase_* testCase, void* progressContext);

/*
 * The ACU_TestCase is the test case that is executed.
 */
__EXPORT void acu_report(const struct ACU_TestCase_* testCase, void* context);

/*
 * The ACU_TestCase is the test case that is executed.
 */
__EXPORT void acu_countTestCases(const struct ACU_TestCase_* testCase, void* context);

/*
 * The ACU_TestCase is the test case that is executed.
 */
__EXPORT void acu_collectTestCases(const struct ACU_TestCase_* testCase, void* context);

/*
 * The ACU_TestCase is the test case that is executed.
 */
__EXPORT void acu_count(const struct ACU_TestCase_* testCase, void* context);

typedef struct ACU_Progress_ {
    ACU_ProgressFunc* progress;
    void* context;
} ACU_Progress;

typedef struct ACU_ReportVisitor_ {
    ACU_ReportVisitorFunc* visitor;
    void* context;
} ACU_ReportVisitor;

typedef struct ACU_Summary_ {
    int totalTestCases;
    int failedTestCases;
} ACU_Summary;

typedef struct ACU_ReportHelper_ {
    const char* fixtureName;
    const char* suiteName;
} ACU_ReportHelper;

typedef struct ACU_Count_ {
    int testCaseCount;
    int testFixtureCount;
    ACU_ReportHelper last;
} ACU_Count;

typedef struct ACU_TestCases_ {
    ACU_List* testCases;
} ACU_TestCases;

/*
 * Reports the progress of the test case.
 */
__EXPORT void acu_performProgress(const ACU_Progress* progress, const struct ACU_TestCase_* testCase);

/*
 * Reports the progress of the test case.
 */ 
__EXPORT void acu_reportSummary(const struct ACU_TestCase_* testCase, void* context);

#endif