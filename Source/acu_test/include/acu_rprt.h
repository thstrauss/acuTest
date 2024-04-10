#pragma once
#ifndef _ACU_RPRT_H_
#define _ACU_RPRT_H_

#include <acu_tcse.h>

typedef void ACU_ProgressFunc(const ACU_TestCase* testCase);
typedef void* ACU_ReportFunc(const ACU_TestCase* testCase, void* context);

void acu_progress(const ACU_TestCase* testCase);
void* acu_report(const ACU_TestCase* testCase, void* context);

typedef struct ACU_Summary_ {
    int totalTestCases;
    int failedTestCases;
} ACU_Summary;

void* acu_reportSummary(const ACU_TestCase* testCase, void* context);

#endif