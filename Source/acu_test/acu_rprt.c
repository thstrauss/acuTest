
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <acu_rprt.h>
#include <acu_cmmn.h>
#include <acu_tcse.h>
#include <acu_fxtr.h>
#include <acu_suit.h>

void acu_progress(const ACU_TestCase* testCase) {
    fprintf(stdout, "%s", testCase->result->status == ACU_TEST_PASSED ? "." : "F");
}

void acu_report(const ACU_TestCase* testCase) {
    static const char* fixtureName = NULL;
    static const char* suiteName = NULL;
    if (suiteName == NULL || strcmp(suiteName, testCase->fixture->suite->name) != 0) {
        suiteName = testCase->fixture->suite->name;
        fprintf(stdout, "%s\n\r", suiteName);
    }
    if (fixtureName == NULL || strcmp(fixtureName, testCase->fixture->name) != 0) {
        fixtureName = testCase->fixture->name;
        fprintf(stdout, "  %s\n\r", fixtureName);
    }
    if (testCase->result != NULL && testCase->result->status != ACU_TEST_PASSED) {
        fprintf(stdout, "    %s: %s\n\r      %s:%d:\n\r      %s\n\r", testCase->name, testCase->result->status == ACU_TEST_PASSED ? "passed" : "failed", testCase->result->file, testCase->result->line, testCase->result->message);
    }
}

