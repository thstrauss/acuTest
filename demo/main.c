/*
 * Copyright (c) 2024 Thomas Strauss
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_rprt.h>

#include <stddef.h>

static void simpleTestCase(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, int, Equal, 0, 42, "number not equal to 42");
    UNUSED(context);
}

ACU_Fixture* sampleFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    acu_initFixture(fixture, "sample tests");
    acu_addTestCase(fixture, "simpleTestCase", simpleTestCase);
    return fixture;
}

int main(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();
    ACU_ReportHelper reportHelper = { NULL, NULL };
    ACU_ReportVisitor report = { acu_report, NULL };
    ACU_Progress progress = { acu_progress , NULL };
    ACU_TestResult result;

    report.context = &reportHelper;

    acu_initFixture(fixture, "Sample test suite");
    acu_addChildFixture(fixture, sampleFixture());

    result = acu_executeFixture(fixture, &progress);

    acu_acceptFixture(fixture, &report);

    acu_fixtureDestroy(fixture);
    return result != ACU_TEST_PASSED ? 2 : 0;
}
