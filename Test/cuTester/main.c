#include <stdio.h>
#include <stdlib.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>

#include "flt_test.h"
#include "tryc_tst.h"
#include "fxtr_tst.h"

static void progress(const ACU_TestCase* testCase) {
    printf("%s", testCase->result->status == ACU_TEST_PASSED ? "." : "F");
}

int main() {
    ACU_Suite suite;
    int returnValue;

    acu_suiteInit(&suite, "Suite");

    fixtureFixture(&suite);
    floatFixture(&suite);
    tryCatchFixture(&suite);

    returnValue = acu_suiteExecute(&suite, progress) == ACU_TEST_PASSED ? 0 : 2;
    fprintf(stdout, "\n\r");

    acu_suiteReport(stdout, &suite);

    acu_suiteDestroy(&suite);

    return returnValue;
}