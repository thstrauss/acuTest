#include <acu_eenv.h>
#include <acu_asrt.h>
#include <acu_tcse.h> 
#include "fxtr_tst.h"

static void test1(ACU_ExecuteEnv* environment, const void* context) {
    ACU_PrepareParameter(int, 1, 2, "assert");
    environment->result->file = acu_estrdup(__FILE__);
    environment->result->line = __LINE__ + 1;
    acu_assert_intEqual(environment, &parameter);
    UNUSED(context);
}

static void test2(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 1, 1, "assert2");
    ACU_assert(environment, int, NotEqual, 1, 2, "xxx");
    ACU_assert(environment, float, NotEqual, 1.0, 2.0, "yyy");
    UNUSED(context);
}

static void test3(ACU_ExecuteEnv* environment, const void* context) {
    char str[] = "abc";
    ACU_assert_strEqual(environment, str, str, "assert2");
    ACU_assert_strEqual(environment, "abc", "abc", "assert2");
    ACU_assert_strEqual(environment, (char*)context, "context", "assert context");
    ACU_assert_strNotEqual(environment, "str", "str", "assert2");
}

static void failingFixture(ACU_Suite* suite) {
    ACU_Fixture* fixture = (ACU_Fixture *) acu_emalloc(sizeof(ACU_Fixture));

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_fixtureAddTestCase(fixture, "test1", test1);
    acu_fixtureAddTestCase(fixture, "test2", test2);
    acu_fixtureAddTestCase(fixture, "test3", test3);
    acu_suiteAddFixture(suite, fixture);
}

static void passingFixture(ACU_Suite* suite) {
    ACU_Fixture* fixture = (ACU_Fixture*)acu_emalloc(sizeof(ACU_Fixture));

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_fixtureAddTestCase(fixture, "test2", test2);
    acu_suiteAddFixture(suite, fixture);
}

static void emptyFixture(ACU_Suite* suite) {
    ACU_Fixture* fixture = (ACU_Fixture*)acu_emalloc(sizeof(ACU_Fixture));

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_suiteAddFixture(suite, fixture);
}

static void progress(const ACU_TestCase* testCase) {
    printf("%s", testCase->result->status == ACU_TEST_PASSED ? "#" : "f");
}

static void failingFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = (ACU_Suite*) acu_emalloc(sizeof(ACU_Suite));

    acu_suiteInit(localSuite, "testSuite");
    failingFixture(localSuite);
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_FAILED, "assert2");
}

static void passíngFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = (ACU_Suite*)acu_emalloc(sizeof(ACU_Suite));

    acu_suiteInit(localSuite, "testSuite");
    passingFixture(localSuite);
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_PASSED, "assert2");
}

static void emptyFixtureTest(ACU_ExecuteEnv* environment, const void* context)
{
    int result = ACU_TEST_PASSED;
    ACU_Suite* localSuite = (ACU_Suite*)acu_emalloc(sizeof(ACU_Suite));

    acu_suiteInit(localSuite, "testSuite");
    emptyFixture(localSuite);
    result = acu_suiteExecute(localSuite, progress);
    acu_suiteDestroy(localSuite);

    ACU_assert(environment, int, Equal, result, ACU_TEST_PASSED, "assert2");
}

void fixtureFixture(ACU_Suite* suite)
{
    ACU_Fixture* fixture = acu_emalloc(sizeof(ACU_Fixture));
    acu_suiteAddFixture(suite, fixture);
    acu_fixtureInit(fixture, "fixture tests");
    acu_fixtureAddTestCase(fixture, "Failing Fixture", failingFixtureTest);
    acu_fixtureAddTestCase(fixture, "Passing Fixture", passíngFixtureTest);
    acu_fixtureAddTestCase(fixture, "Empty Fixture", emptyFixtureTest);
}