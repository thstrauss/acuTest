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

#include "int_tst.h"

#include "tst_bed.h"

TESTBED(, int, Equal, 0, 0)
TESTBED(, int, NotEqual, 0, 1)
TESTBED(, int, Less, 0, 1)
TESTBED(, int, LessEqual, 0, 1)
TESTBED(Eq, int, LessEqual, 1, 1)
TESTBED(, int, Greater, 1, 0)
TESTBED(, int, GreaterEqual, 1, 0)
TESTBED(Eq, int, GreaterEqual, 1, 1)

TESTBED_FAILED(, int, Equal, 0, 1)
TESTBED_FAILED(, int, NotEqual, 1, 1)
TESTBED_FAILED(, int, Less, 1, 1)
TESTBED_FAILED(, int, LessEqual, 2, 1)
TESTBED_FAILED(, int, Greater, 1, 1)
TESTBED_FAILED(, int, GreaterEqual, 1, 2)

ACU_Fixture* intFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "int tests");

    acu_fixtureAddTestCase(fixture, "int Equal", testintEqual);
    acu_fixtureAddTestCase(fixture, "int NotEqual", testintNotEqual);
    acu_fixtureAddTestCase(fixture, "int Less", testintLess);
    acu_fixtureAddTestCase(fixture, "int LessEqual", testintLessEqual);
    acu_fixtureAddTestCase(fixture, "int LessEqualEq", testintLessEqualEq);
    acu_fixtureAddTestCase(fixture, "int Greater", testintGreater);
    acu_fixtureAddTestCase(fixture, "int GreaterEqualEq", testintGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "int GreaterEqual", testintGreaterEqual);

    acu_fixtureAddTestCase(fixture, "int Equal failed", testFailedintEqual);
    acu_fixtureAddTestCase(fixture, "int NotEqual failed", testFailedintNotEqual);
    acu_fixtureAddTestCase(fixture, "int Less failed", testFailedintLess);
    acu_fixtureAddTestCase(fixture, "int LessEqual failed", testFailedintLessEqual);
    acu_fixtureAddTestCase(fixture, "int Greater failed", testFailedintGreater);
    acu_fixtureAddTestCase(fixture, "int GreaterEqual failed", testFailedintGreaterEqual);

    return fixture;
}
