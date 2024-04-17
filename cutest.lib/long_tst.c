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
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>

#include "long_tst.h"

#include "tst_bed.h"

TESTBED(, long, Equal, 0, 0)
TESTBED(, long, NotEqual, 0, 1)
TESTBED(, long, Less, 0, 1)
TESTBED(, long, LessEqual, 0, 1)
TESTBED(Eq, long, LessEqual, 1, 1)
TESTBED(, long, Greater, 1, 0)
TESTBED(, long, GreaterEqual, 1, 0)
TESTBED(Eq, long, GreaterEqual, 1, 1)

ACU_Fixture* longFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "long tests");

    acu_fixtureAddTestCase(fixture, "long Equal", testlongEqual);
    acu_fixtureAddTestCase(fixture, "long NotEqual", testlongNotEqual);
    acu_fixtureAddTestCase(fixture, "long Less", testlongLess);
    acu_fixtureAddTestCase(fixture, "long LessEqual", testlongLessEqual);
    acu_fixtureAddTestCase(fixture, "long LessEqualEq", testlongLessEqualEq);
    acu_fixtureAddTestCase(fixture, "long Greater", testlongGreater);
    acu_fixtureAddTestCase(fixture, "long GreaterEqualEq", testlongGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "long GreaterEqual", testlongGreaterEqual);

    return fixture;
}
