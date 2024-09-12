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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "acu_rprt.h"
#include "acu_cmmn.h"
#include "acu_tcse.h"
#include "acu_fxtr.h"
#include "acu_suit.h"
#include "acu_list.h"
#include "acu_util.h"
#include "acu_tryc.h"

void acu_progress(const ACU_TestCase* testCase, void* progressContext) {
    char buffer[2];
    acu_printf_s(buffer, sizeof(buffer), "%s", testCase->result.status == ACU_TEST_FAILED ? "F" : testCase->result.status == ACU_TEST_ERROR ? "E" : ".");
    UNUSED(progressContext);
}

void acu_report(const ACU_TestCase* testCase, void* context) {
    ACU_ReportHelper* reportHelper = context;
    if (!reportHelper->suiteName || strcmp(reportHelper->suiteName, testCase->fixture->suite->name) != 0) {
        reportHelper->suiteName = testCase->fixture->suite->name;
        fprintf(stdout, "%s took %ld ms\n\r", reportHelper->suiteName, ((testCase->fixture->suite->end - testCase->fixture->suite->start)*1000)/CLK_TCK);
    }
    if (!reportHelper->fixtureName || strcmp(reportHelper->fixtureName, testCase->fixture->name) != 0) {
        reportHelper->fixtureName = testCase->fixture->name;
        fprintf(stdout, "  %s took %ld ms\n\r", reportHelper->fixtureName, ((testCase->fixture->end - testCase->fixture->start) * 1000) / CLK_TCK);
    }
    if (testCase->result.status != ACU_TEST_PASSED) {
        fprintf(stdout, "    %s: %s\n\r      %s:%d:\n\r      %s\n\r", testCase->name, testCase->result.status == ACU_TEST_PASSED ? "passed" : "failed", SAFE_REF(testCase->result.sourceFileName), testCase->result.sourceLine, SAFE_REF(testCase->result.message));
    }
}

void acu_count(const ACU_TestCase* testCase, void* context) {
    ACU_Count* count = context;
    if (!(count->last.fixtureName) || strcmp(count->last.fixtureName, testCase->fixture->name) != 0) {
        count->last.fixtureName = testCase->fixture->name;
        count->testFixtureCount++;
    }
	count->testCaseCount++;
	UNUSED(testCase);
}


void acu_countTestCases(const ACU_TestCase* testCase, void* context)
{
    (*(int*) context)++;
    UNUSED(testCase);
}

void acu_collectTestCases(const ACU_TestCase* testCase, void* context)
{
    ACU_TestCases* testCases = (ACU_TestCases*) context;
    acu_listAppend(testCases->testCases, testCase);
}

void acu_reportSummary(const ACU_TestCase* testCase, void* context)
{
    ACU_Summary* summary = (ACU_Summary*) context;
    summary->totalTestCases++;
    if (testCase->result.status == ACU_TEST_FAILED) {
        summary->failedTestCases++;
    }
}

