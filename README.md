# acuTest

acuTest is a C89 compliant c unit test framework. The library and unit tests are compiling 
on Atari ST with PureC and on Windows with VC. 

## Demo

### Test Cases
Test cases are just C functions. They have to follow the signature: `void simpleTestCase(ACU_ExecuteEnv* environment, const void* context)`.
The parameter `environment` is used by the framework to provide some information which is needed for result collection by the assert functions.
The `context` allows to pass in fixture common data.

### Asserts

The asserts are defined as macros which allows to capture file and line number. For **simple** types the main entry
is `ACU_assert`.

### Fixtures

A Fixture's is aggregating test cases. The test cases are just added by providing a function pointer to the test case.

### Suites

A Suite's is aggregating fixtures on the next level. On suite level you typically execute the test 
cases with `result = acu_suiteExecute(suite, acu_progress, NULL)`.

```C
#include <acu_eenv.h>
#include <acu_asrt.h>
#include <acu_fxtr.h>
#include <acu_tryc.h>
#include <acu_suit.h>

static void simpleTestCase(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 0, 42, "number not equal to 42");
    UNUSED(context);
}

ACU_Fixture* listFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    acu_fixtureInit(fixture, "sample tests");
    acu_fixtureAddTestCase(fixture, "simpleTestCase", simpleTestCase);
    return fixture;
}

int main()
{
    ACU_Suite* suite = acu_suiteMalloc();
    enum ACU_TestResult result;
    acu_suiteInit(suite, "Sample  test suite");
    acu_suiteAddFixture(suite, listFixture());

    result = acu_suiteExecute(suite, acu_progress, NULL);

    acu_suiteAccept(suite, acu_report, NULL);
    acu_suiteDestroy(suite);
    return result != ACU_TEST_PASSED ? 2 : 0;
}
``` 
## Design Considerations

The Atari PureC compiler has a limitation of 4096 characters for macro expansion. To work arround this
an hybrid aproach with macro based code generation at compile time to generate the assert functions and 
macro's which are part of the runtime was taken. It acts somehow like templates in C++.

 