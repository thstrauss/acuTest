#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <acu_fxtr.h>
#include <acu_suit.h>
#include <acu_util.h>

#include "flt_test.h"
#include "tryc_tst.h"
#include "fxtr_tst.h"

static void progress(const ACU_TestCase* testCase) {
    fprintf(stdout, "%s", testCase->result->status == ACU_TEST_PASSED ? "." : "F");
}

static void report(const ACU_TestCase* testCase) {
    static char* fixtureName = NULL;
    static char* suiteName = NULL;
    if (suiteName == NULL || strcmp(suiteName, testCase->fixture->suite->name) != 0) {
        suiteName = testCase->fixture->suite->name;
        fprintf(stdout, "%s\n\r", suiteName);
    }
    if (fixtureName == NULL || strcmp(fixtureName, testCase->fixture->name) != 0) {
        fixtureName = testCase->fixture->name;
        fprintf(stdout, "  %s\n\r", fixtureName);
    }
    //if (testCase->result != NULL && testCase->result->status != ACU_TEST_PASSED) {
        fprintf(stdout, "    %s: %s\n\r      %s:%d:\n\r      %s\n\r", testCase->name, testCase->result->status == ACU_TEST_PASSED ? "passed" : "failed", testCase->result->file, testCase->result->line, testCase->result->message);
    //}
}

int main() {
    int returnValue;
    ACU_Suite* suite = acu_suiteMalloc();
    
    acu_suiteInit(suite, "Suite");

    acu_suiteAddFixture(suite, fixtureFixture());
    acu_suiteAddFixture(suite, floatFixture());
    acu_suiteAddFixture(suite, tryCatchFixture());

    returnValue = acu_suiteExecute(suite, progress) == ACU_TEST_PASSED ? 0 : 2;
    fprintf(stdout, "\n\r");

    acu_suiteReport(suite, report);

    acu_suiteDestroy(suite);

    return returnValue;
}