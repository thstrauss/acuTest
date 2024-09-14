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
#include <acu_suit.h>

#include "ulng_tst.h"

#include "tst_bed.h"

TESTBED(, unsignedLong, Equal, 0, 0)
TESTBED(, unsignedLong, NotEqual, 0, 1)
TESTBED(, unsignedLong, Less, 0, 1)
TESTBED(, unsignedLong, LessEqual, 0, 1)
TESTBED(Eq, unsignedLong, LessEqual, 1, 1)
TESTBED(, unsignedLong, Greater, 1, 0)
TESTBED(, unsignedLong, GreaterEqual, 1, 0)
TESTBED(Eq, unsignedLong, GreaterEqual, 1, 1)

TESTBED_FAILED(, unsignedLong, Equal, 0, 1)
TESTBED_FAILED(, unsignedLong, NotEqual, 1, 1)
TESTBED_FAILED(, unsignedLong, Less, 1, 1)
TESTBED_FAILED(, unsignedLong, LessEqual, 2, 1)
TESTBED_FAILED(, unsignedLong, Greater, 1, 1)
TESTBED_FAILED(, unsignedLong, GreaterEqual, 1, 2)


ACU_Fixture* unsignedLongFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "unsigned long tests");

    acu_fixtureAddTestCase(fixture, "unsigned long Equal", testunsignedLongEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long NotEqual", testunsignedLongNotEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long Less", testunsignedLongLess);
    acu_fixtureAddTestCase(fixture, "unsigned long LessEqual", testunsignedLongLessEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long LessEqualEq", testunsignedLongLessEqualEq);
    acu_fixtureAddTestCase(fixture, "unsigned long Greater", testunsignedLongGreater);
    acu_fixtureAddTestCase(fixture, "unsigned long GreaterEqualEq", testunsignedLongGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "unsigned long GreaterEqual", testunsignedLongGreaterEqual);

    acu_fixtureAddTestCase(fixture, "unsigned long Equal failed", testFailedunsignedLongEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long NotEqual failed", testFailedunsignedLongNotEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long Less failed", testFailedunsignedLongLess);
    acu_fixtureAddTestCase(fixture, "unsigned long LessEqual failed", testFailedunsignedLongLessEqual);
    acu_fixtureAddTestCase(fixture, "unsigned long Greater failed", testFailedunsignedLongGreater);
    acu_fixtureAddTestCase(fixture, "unsigned long GreaterEqual failed", testFailedunsignedLongGreaterEqual);

    return fixture;
}
