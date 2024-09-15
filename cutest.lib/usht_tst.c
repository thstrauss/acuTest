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

#include "usht_tst.h"

#include "tst_bed.h"

TESTBED(, unsignedShort, Equal, 0, 0)
TESTBED(, unsignedShort, NotEqual, 0, 1)
TESTBED(, unsignedShort, Less, 0, 1)
TESTBED(, unsignedShort, LessEqual, 0, 1)
TESTBED(Eq, unsignedShort, LessEqual, 1, 1)
TESTBED(, unsignedShort, Greater, 1, 0)
TESTBED(, unsignedShort, GreaterEqual, 1, 0)
TESTBED(Eq, unsignedShort, GreaterEqual, 1, 1)

TESTBED_FAILED(, unsignedShort, Equal, 0, 1)
TESTBED_FAILED(, unsignedShort, NotEqual, 1, 1)
TESTBED_FAILED(, unsignedShort, Less, 1, 1)
TESTBED_FAILED(, unsignedShort, LessEqual, 2, 1)
TESTBED_FAILED(, unsignedShort, Greater, 1, 1)
TESTBED_FAILED(, unsignedShort, GreaterEqual, 1, 2)

ACU_Fixture* unsignedShortFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "unsigned short tests");

    acu_fixtureAddTestCase(fixture, "unsigned short Equal", testunsignedShortEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short NotEqual", testunsignedShortNotEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short Less", testunsignedShortLess);
    acu_fixtureAddTestCase(fixture, "unsigned short LessEqual", testunsignedShortLessEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short LessEqualEq", testunsignedShortLessEqualEq);
    acu_fixtureAddTestCase(fixture, "unsigned short Greater", testunsignedShortGreater);
    acu_fixtureAddTestCase(fixture, "unsigned short GreaterEqualEq", testunsignedShortGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "unsigned short GreaterEqual", testunsignedShortGreaterEqual);

    acu_fixtureAddTestCase(fixture, "unsigned short Equal failed", testFailedunsignedShortEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short NotEqual failed", testFailedunsignedShortNotEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short Less failed", testFailedunsignedShortLess);
    acu_fixtureAddTestCase(fixture, "unsigned short LessEqual failed", testFailedunsignedShortLessEqual);
    acu_fixtureAddTestCase(fixture, "unsigned short Greater failed", testFailedunsignedShortGreater);
    acu_fixtureAddTestCase(fixture, "unsigned short GreaterEqual failed", testFailedunsignedShortGreaterEqual);

    return fixture;
}
