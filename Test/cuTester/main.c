#include <stdio.h>

#include <acu_suit.h>
#include <acu_rprt.h>

#include "flt_test.h"
#include "tryc_tst.h"
#include "fxtr_tst.h"

int main() {
    int returnValue;
    ACU_Suite* suite = acu_suiteMalloc();
    ACU_Summary summary = { 0,0 };
    
    acu_suiteInit(suite, "Suite");

    acu_suiteAddFixture(suite, fixtureFixture());
    acu_suiteAddFixture(suite, floatFixture());
    acu_suiteAddFixture(suite, tryCatchFixture());

    returnValue = acu_suiteExecute(suite, acu_progress) == ACU_TEST_PASSED ? 0 : 2;
    fprintf(stdout, "\n\r");

    acu_suiteReport(suite, NULL, acu_report);
    acu_suiteReport(suite, &summary, acu_reportSummary);
    fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);

    acu_suiteDestroy(suite);

    return returnValue;
}