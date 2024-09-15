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

#include "shrt_tst.h"

#include "tst_bed.h"

TESTBED(, short, Equal, 0, 0)
TESTBED(, short, NotEqual, 0, 1)
TESTBED(, short, Less, 0, 1)
TESTBED(, short, LessEqual, 0, 1)
TESTBED(Eq, short, LessEqual, 1, 1)
TESTBED(, short, Greater, 1, 0)
TESTBED(, short, GreaterEqual, 1, 0)
TESTBED(Eq, short, GreaterEqual, 1, 1)

TESTBED_FAILED(, short, Equal, 0, 1)
TESTBED_FAILED(, short, NotEqual, 1, 1)
TESTBED_FAILED(, short, Less, 1, 1)
TESTBED_FAILED(, short, LessEqual, 2, 1)
TESTBED_FAILED(, short, Greater, 1, 1)
TESTBED_FAILED(, short, GreaterEqual, 1, 2)

ACU_Fixture* shortFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "short tests");

    acu_fixtureAddTestCase(fixture, "short Equal", testshortEqual);
    acu_fixtureAddTestCase(fixture, "short NotEqual", testshortNotEqual);
    acu_fixtureAddTestCase(fixture, "short Less", testshortLess);
    acu_fixtureAddTestCase(fixture, "short LessEqual", testshortLessEqual);
    acu_fixtureAddTestCase(fixture, "short LessEqualEq", testshortLessEqualEq);
    acu_fixtureAddTestCase(fixture, "short Greater", testshortGreater);
    acu_fixtureAddTestCase(fixture, "short GreaterEqualEq", testshortGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "short GreaterEqual", testshortGreaterEqual);

    acu_fixtureAddTestCase(fixture, "short Equal failed", testFailedshortEqual);
    acu_fixtureAddTestCase(fixture, "short NotEqual failed", testFailedshortNotEqual);
    acu_fixtureAddTestCase(fixture, "short Less failed", testFailedshortLess);
    acu_fixtureAddTestCase(fixture, "short LessEqual failed", testFailedshortLessEqual);
    acu_fixtureAddTestCase(fixture, "short Greater failed", testFailedshortGreater);
    acu_fixtureAddTestCase(fixture, "short GreaterEqual failed", testFailedshortGreaterEqual);

    return fixture;
}
