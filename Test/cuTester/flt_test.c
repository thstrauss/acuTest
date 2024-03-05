#include <string.h>
#include <setjmp.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>

#include "flt_test.h"

static void floatTests(ACU_ExecuteEnv* environment, const void* context) {

    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    void* null = NULL;

    memset(testEnvironment, 0, sizeof(ACU_ExecuteEnv));
    testEnvironment->result = resultBuf;

    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;

    if (!setjmp(testEnvironment->assertBuf)) {
        ACU_assert(testEnvironment, float, Equal, 0.0, 0.0, "");
    }

    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "Float Equal");
        ACU_assert_strEqual(environment, testEnvironment->result->message, "", "Float Equal");
    FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;

    UNUSED(context);
}

void floatFixture(ACU_Suite* suite)
{
    ACU_Fixture* fixture = acu_emalloc(sizeof(ACU_Fixture));
    acu_suiteAddFixture(suite, fixture);
    acu_fixtureInit(fixture, "float tests");

    acu_fixtureAddTestCase(fixture, "float", floatTests);
}
