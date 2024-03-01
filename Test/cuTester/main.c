#include <stdio.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>

void test1(ACU_ExecuteEnv* environment, const void* context) {
    ACU_PrepareParameter(int, 1, 2, "assert", __LINE__ + 1);
    acu_assert(environment, &acu_intEqual, &acu_intEqualFormatMessage, &parameter);
    acu_assert_intEqual(environment, &parameter);
    context;
}

void test2(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 1, 1, "assert2");
    ACU_assert(environment, int, NotEqual, 1, 2, "xxx");
    ACU_assert(environment, float, NotEqual, 1.0, 2.0, "yyy");
    context;
}

void test3(ACU_ExecuteEnv* environment, const void* context) {
    char str[] = "abc";
    ACU_assert_strEqual(environment, str, str, "assert2");
    ACU_assert_strEqual(environment, "abc", "abc", "assert2");
    ACU_assert_strEqual(environment, (char*) context, "context", "assert context");
    ACU_assert_strNotEqual(environment, "str", "str", "assert2");
}

void testFixture(ACU_Suite* suite) {
    ACU_Fixture* fixture = malloc(sizeof(ACU_Fixture));

    acu_fixtureInit(fixture, "testFixture");
    acu_fixtureSetContext(fixture, "context");

    acu_fixtureAddTestCase(fixture, "test1", test1);
    acu_fixtureAddTestCase(fixture, "test2", test2);
    acu_fixtureAddTestCase(fixture, "test3", test3);

    acu_suiteAddFixture(suite, fixture);
}

int main() {
    ACU_Suite suite;
    int returnValue;

    acu_suiteInit(&suite, "Suite");

    testFixture(&suite);
    testFixture(&suite);

    acu_suiteExecute(&suite);

    returnValue = acu_suiteReport(&suite) == ACU_TEST_PASSED ? 0 : 2;

    acu_suiteDestroy(&suite);

    return returnValue;
}