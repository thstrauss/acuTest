#include <stdio.h>
#include "..\..\Source\cu_test.h"
#include "..\..\Source\cu_assrt.h"
#include "..\..\Source\cu_cmmn.h"

void test1(CU_ExecuteEnv* environment, const void* context) {
    cuTest_PrepareParameter(int, 1, 2, "assert", __LINE__ + 1);
    cuTest_assert(environment, &cuTest_equalInt, &cuTest_equalIntFormatMessage, &parameter);
}

void test2(CU_ExecuteEnv* environment, const void* context) {
    cuTest_assertEqualInt(environment, 1, 1, "assert2");
    cuTest_assertNotEqualInt(environment, 1, 2, "xxx");
}

void test3(CU_ExecuteEnv* environment, const void* context) {
    char str[] = "abc";
    cuTest_assertEqualStr(environment, str, str, "assert2");
    cuTest_assertEqualStr(environment, "abc", "abc", "assert2");
    cuTest_assertEqualStr(environment, (char*) context, "context", "assert context");
    cuTest_assertNotEqualStr(environment, "str", "abc", "assert2");
}

int main() {
    CU_Fixture fixture;
    int returnValue;

    cuTest_init(&fixture, "testFixture");
    cuTest_addContext(&fixture, "context");

    cuTest_addTestCase(&fixture, "test1", test1);
    cuTest_addTestCase(&fixture, "test2", test2);
    cuTest_addTestCase(&fixture, "test3", test3);

    cuTest_execute(&fixture);

    returnValue = cuTest_report(&fixture) == CU_TEST_PASSED ? 0 : 2;

    cuTest_destroy(&fixture);

    return returnValue;
}