#include <stdio.h>
#include "..\..\Source\cu_test.h"
#include "..\..\Source\cu_assrt.h"
#include "..\..\Source\cu_cmmn.h"

void test1(CU_ExecuteEnv* environment, const void* context) {
    CU_PrepareParameter(int, 1, 2, "assert", __LINE__ + 1);
    cu_assert(environment, &cu_equalInt, &cu_equalIntFormatMessage, &parameter);
}

void test2(CU_ExecuteEnv* environment, const void* context) {
    CU_assertEqualInt(environment, 1, 1, "assert2");
    CU_assertNotEqualInt(environment, 1, 2, "xxx");
}

void test3(CU_ExecuteEnv* environment, const void* context) {
    char str[] = "abc";
    CU_assertEqualStr(environment, str, str, "assert2");
    CU_assertEqualStr(environment, "abc", "abc", "assert2");
    CU_assertEqualStr(environment, (char*) context, "context", "assert context");
    CU_assertNotEqualStr(environment, "str", "abc", "assert2");
}

int main() {
    CU_Fixture fixture;
    int returnValue;

    cu_fixtureInit(&fixture, "testFixture");
    cu_fixtureSetContext(&fixture, "context");

    cu_fixtureAddTestCase(&fixture, "test1", test1);
    cu_fixtureAddTestCase(&fixture, "test2", test2);
    cu_fixtureAddTestCase(&fixture, "test3", test3);

    cu_fixtureExecute(&fixture);

    returnValue = cu_fixturReport(&fixture) == CU_TEST_PASSED ? 0 : 2;

    cu_fixtureDestroy(&fixture);

    return returnValue;
}