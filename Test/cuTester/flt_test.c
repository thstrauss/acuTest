#include <string.h>
#include <setjmp.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>

#include "flt_test.h"
#include <math.h>
#include <float.h>

#ifdef __TOS__
#define INF FLT_MAX * 2.0f
#define NAN INF/INF
#endif


#define TESTBED(postfix, type, operation, actual, expetced) static void test##type##operation##postfix(ACU_ExecuteEnv* environment, const void* context) { \
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv)); \
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result)); \
    testEnvironment->result = resultBuf; \
    resultBuf->status = ACU_TEST_PASSED; \
    resultBuf->message = NULL; \
    resultBuf->file = NULL; \
    if (!setjmp(testEnvironment->assertBuf)) { \
        ACU_assert(testEnvironment, type, operation, actual, expetced, #type#operation#postfix); \
    } \
    TRY \
    ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, #type#operation#postfix); \
    ACU_assert_strEqual(environment, testEnvironment->result->message, "", #type#operation#postfix); \
    FINALLY \
    if (resultBuf->message != NULL) { \
        free(resultBuf->message); \
    } \
    free(resultBuf); \
    free(testEnvironment); \
    ETRY; \
    UNUSED(context); \
}\

TESTBED(,float, Equal, 0.0f, 0.0f)
TESTBED(Nan, float, NotEqual, 0.0f, NAN)
TESTBED(,float, NotEqual, 0.0f, 1.0f)
TESTBED(,float, Less, 0.0f, 1.0f)
TESTBED(,float, LessEqual, 0.0f, 1.0f)
TESTBED(Eq, float, LessEqual, 1.0f, 1.0f)
TESTBED(,float, Greater, 1.0f, 0.0f)
TESTBED(,float, GreaterEqual, 1.0f, 0.0f)
TESTBED(Eq, float, GreaterEqual, 1.0f, 1.0f)

TESTBED(, double, Equal, 0.0, 0.0)
TESTBED(, double, NotEqual, 0.0, 1.0)
TESTBED(Nan, double, NotEqual, 0.0f, NAN)
TESTBED(, double, Less, 0.0, 1.0)
TESTBED(, double, LessEqual, 0.0, 1.0)
TESTBED(Eq, double, LessEqual, 1.0, 1.0)
TESTBED(, double, Greater, 1.0, 0.0)
TESTBED(, double, GreaterEqual, 1.0, 0.0)
TESTBED(Eq, double, GreaterEqual, 1.0, 1.0)

static void doubleNotEqual(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, double, NotEqual, 0.0, 1.0, "");
    UNUSED(context);
}

ACU_Fixture* floatFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "float tests");

    acu_fixtureAddTestCase(fixture, "float Equal", testfloatEqual);
    acu_fixtureAddTestCase(fixture, "float NotEqual NAN", testfloatNotEqualNan);
    acu_fixtureAddTestCase(fixture, "float NotEqual", testfloatNotEqual);
    acu_fixtureAddTestCase(fixture, "float Less", testfloatLess);
    acu_fixtureAddTestCase(fixture, "float LessEqual", testfloatLessEqual);
    acu_fixtureAddTestCase(fixture, "float LessEqualEq", testfloatLessEqualEq);
    acu_fixtureAddTestCase(fixture, "float Greater", testfloatGreater);
    acu_fixtureAddTestCase(fixture, "float GreaterEqualEq", testfloatGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "float GreaterEqual", testfloatGreaterEqual);

    acu_fixtureAddTestCase(fixture, "double Equal", testdoubleEqual);
    acu_fixtureAddTestCase(fixture, "double NotEqual NAN", testdoubleNotEqualNan);
    acu_fixtureAddTestCase(fixture, "double NotEqual", testdoubleNotEqual);
    acu_fixtureAddTestCase(fixture, "double Less", testdoubleLess);
    acu_fixtureAddTestCase(fixture, "double LessEqual", testdoubleLessEqual);
    acu_fixtureAddTestCase(fixture, "double LessEqualEq", testdoubleLessEqualEq);
    acu_fixtureAddTestCase(fixture, "double Greater", testdoubleGreater);
    acu_fixtureAddTestCase(fixture, "double GreaterEqualEq", testdoubleGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "double GreaterEqual", testdoubleGreaterEqual);

    return fixture;
}
