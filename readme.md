# acu_Test 

acu_Test (Atari C Unit Test) is a C89 compliant c unit test framework. The library and unit tests are compiling 
on Atari ST with PureC and on Windows with VC. 

## Plug-ins

The framework allows to implement test plug-ins which are the executed by a generic runner.

### Windows

### Plug-in Implementation

Please implement a function like this with name `acu_entr.c`:

```C
ACU_Entry* acu_init() {
    ACU_Fixture* fixture = acu_fixtureMalloc();
    ACU_Entry* entry = acu_entryMalloc();
    acu_entryInit(entry, fixture);

...
    /* Please add test fixtures to suites. */
...

    return entry;
}
```

Build a dll like `example.dll`

## Start Generic Runner

```cmd
curunner example.dll
```

The runner starts and execute the given dll. Output is going to the console. The `curunner` return `0` on success and `2` when any test fails.

### Atari TOS

For plain TOS we don't have a concept of shared libraries. Something similar is done with CPX modules. 
I reimplemented the loading of CPX-modules.

#### TOS Plug-in Implementation

The PureC project file shall look like this:

```
cutscup.cup

.C [ -I..\acu_test ]
.C [ -I..\acu_util ]
.C [ -P ]
.L [ -Y ]

=

..\acu_test\plgstart.s

acu_entr.c (..\acu_test\acu_ldr.h)
char_tst.c (char_tst.h, tst_bed.h)

..\acu_util\acu_util.prj
..\acu_test\acu_test.prj

PCFLTLIB.LIB
PCSTDLIB.LIB
PCTOSLIB.LIB
PCEXTLIB.LIB
```

The of `acu_entr.c` is the same as for the windows plug-in.

The linker creates an executable with the start code `plgstart.s`. The start code is executed when 
the plug-in is loaded by `curunner.ttp`.

#### Start Generic TOS Runner

```cmd
curunner cutscup.cup
```

The runner starts and execute the given plug-in. Output is going to the console. The `curunner` return `0` on success and `2` when any test fails.

#### Start Generic GEM-Runner

There is a GEM-Runner `gemrunnr.prg` implemented. In the moment it just have a load and execute item. More to come.

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

A Fixture's can aggregate fixtures on the next level. On higher level you typically execute the test 
cases with `result = acu_fixtureExecute(suite, acu_progress, NULL)`.

```C
#include <stdlib.h>

#include <acu_eenv.h>
#include <acu_asrt.h>
#include <acu_fxtr.h>
#include <acu_tryc.h>

static void simpleTestCase(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 0, 42, "number not equal to 42");
    UNUSED(context);
}

ACU_Fixture* sampleFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    acu_fixtureInit(fixture, "sample tests");
    acu_fixtureAddTestCase(fixture, "simpleTestCase", simpleTestCase);
    return fixture;
}

int main()
{
    ACU_Fixture* fixture = acu_fixtureMalloc();
    ACU_ReportHelper reportHelper = {NULL, NULL};
    ACU_ReportVisitor report = { acu_report, NULL };
    ACU_Progress progress = { acu_progress , NULL };
    enum ACU_TestResult result;
    report.context = &reportHelper;

    acu_fixtureInit(fixture, "Sample test suite");
    acu_fixtureAddChildFixture(fixture, sampleFixture());

    result = acu_fixtureExecute(fixture, &progress);

    acu_fixtureAccept(fixture, &report);

    acu_fixtureDestroy(fixture);
    return result != ACU_TEST_PASSED ? 2 : 0;
}
``` 
## Design Considerations

The Atari PureC compiler has a limitation of 4096 characters for macro expansion. To work around this
an hybrid approach with macro based code generation at compile time to generate the assert functions and 
macro's which are part of the runtime was taken. It acts somehow like templates in C++.

 
