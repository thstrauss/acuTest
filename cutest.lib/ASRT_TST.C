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

#include "asrt_tst.h"

#include <acu_tryc.h>
#include <acu_asrt.h>
#include <acu_util.h>

#include <stdio.h>

static void formatIntEqualFailedMessageTest(ACU_ExecuteEnv* environment, const void* context) {
    char* buffer;
    __ACU_PrepareParameter(int, Equal, 1, 2, "testMessage", parameter1);
    buffer = parameter1.funcs->formatFailedMessage(&parameter1);
    ACU_assert_strEqual(environment, buffer, "actual value 1 not == to 2: testMessage", "failed");
    acu_free(buffer);
    UNUSED(context);
}

static void formatFloatEqualFailedMessageTest(ACU_ExecuteEnv* environment, const void* context) {
    char* buffer;
    char expected[256];
    __ACU_PrepareParameter(float, Equal, 123456789.f, 2.f, "testMessage", parameter1);
    buffer = parameter1.funcs->formatFailedMessage(&parameter1);
    acu_sprintf_s(expected, sizeof expected, "actual value %le not == to %le: testMessage", parameter1.values.actual.floatType, parameter1.values.expected.floatType);
    ACU_assert_strEqual(environment, buffer, expected, "failed");
    acu_free(buffer);
    UNUSED(context);
}

static void formatDoubleEqualFailedMessageTest(ACU_ExecuteEnv* environment, const void* context) {
    char* buffer;
    char expected[256];
    __ACU_PrepareParameter(double, Equal, 123456789., 2., "testMessage", parameter1);
    buffer = parameter1.funcs->formatFailedMessage(&parameter1);
    acu_sprintf_s(expected, sizeof expected, "actual value %le not == to %le: testMessage", parameter1.values.actual.doubleType, parameter1.values.expected.doubleType);
    ACU_assert_strEqual(environment, buffer, expected, "failed");
    acu_free(buffer);
    UNUSED(context);

}

ACU_Fixture* assertFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "assert tests");

    acu_addTestCase(fixture, "formatIntEqualFailedMessageTest", formatIntEqualFailedMessageTest);
    acu_addTestCase(fixture, "formatFloatEqualFailedMessageTest", formatFloatEqualFailedMessageTest);
    acu_addTestCase(fixture, "formatDoubleEqualFailedMessageTest", formatDoubleEqualFailedMessageTest);

    return fixture;
}
