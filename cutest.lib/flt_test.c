/*
 * Copyright (c) 2024 Thomas Strauß
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

#include "flt_test.h"
#include <math.h>
#include <float.h>

#ifdef __TOS__
#define INF FLT_MAX * 2.0f
#define NAN INF/INF
#endif

#include "tst_bed.h"

TESTBED(,float, Equal, 0.0f, 0.0f)
TESTBED(Nan, float, NotEqual, 0.0f, NAN)
TESTBED(,float, NotEqual, 0.0f, 1.0f)
TESTBED(,float, Less, 0.0f, 1.0f)
TESTBED(,float, LessEqual, 0.0f, 1.0f)
TESTBED(Eq, float, LessEqual, 1.0f, 1.0f)
TESTBED(,float, Greater, 1.0f, 0.0f)
TESTBED(,float, GreaterEqual, 1.0f, 0.0f)
TESTBED(Eq, float, GreaterEqual, 1.0f, 1.0f)

TESTBED_FAILED(, float, Equal, 0.0f, 1.0f)
TESTBED_FAILED(, float, NotEqual, 1.0f, 1.0f)
TESTBED_FAILED(, float, Less, 1.0f, 1.0f)
TESTBED_FAILED(, float, LessEqual, 2.0f, 1.0f)
TESTBED_FAILED(, float, Greater, 1.0f, 1.0f)
TESTBED_FAILED(, float, GreaterEqual, 1.0f, 2.0f)

TESTBED(, double, Equal, 0.0, 0.0)
TESTBED(, double, NotEqual, 0.0, 1.0)
TESTBED(Nan, double, NotEqual, 0.0f, NAN)
TESTBED(, double, Less, 0.0, 1.0)
TESTBED(, double, LessEqual, 0.0, 1.0)
TESTBED(Eq, double, LessEqual, 1.0, 1.0)
TESTBED(, double, Greater, 1.0, 0.0)
TESTBED(, double, GreaterEqual, 1.0, 0.0)
TESTBED(Eq, double, GreaterEqual, 1.0, 1.0)

TESTBED_FAILED(, double, Equal, 0.0, 1.0)
TESTBED_FAILED(, double, NotEqual, 1.0, 1.0)
TESTBED_FAILED(, double, Less, 1.0, 1.0)
TESTBED_FAILED(, double, LessEqual, 2.0, 1.0)
TESTBED_FAILED(, double, Greater, 1.0, 1.0)
TESTBED_FAILED(, double, GreaterEqual, 1.0, 2.0)

ACU_Fixture* floatFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "float/double tests");

    acu_fixtureAddTestCase(fixture, "float Equal", testfloatEqual);
    acu_fixtureAddTestCase(fixture, "float NotEqual NAN", testfloatNotEqualNan);
    acu_fixtureAddTestCase(fixture, "float NotEqual", testfloatNotEqual);
    acu_fixtureAddTestCase(fixture, "float Less", testfloatLess);
    acu_fixtureAddTestCase(fixture, "float LessEqual", testfloatLessEqual);
    acu_fixtureAddTestCase(fixture, "float LessEqualEq", testfloatLessEqualEq);
    acu_fixtureAddTestCase(fixture, "float Greater", testfloatGreater);
    acu_fixtureAddTestCase(fixture, "float GreaterEqualEq", testfloatGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "float GreaterEqual", testfloatGreaterEqual);

    acu_fixtureAddTestCase(fixture, "float Equal failed", testFailedfloatEqual);
    acu_fixtureAddTestCase(fixture, "float NotEqual failed", testFailedfloatNotEqual);
    acu_fixtureAddTestCase(fixture, "float Less failed", testFailedfloatLess);
    acu_fixtureAddTestCase(fixture, "float LessEqual failed", testFailedfloatLessEqual);
    acu_fixtureAddTestCase(fixture, "float Greater failed", testFailedfloatGreater);
    acu_fixtureAddTestCase(fixture, "float GreaterEqual failed", testFailedfloatGreaterEqual);

    acu_fixtureAddTestCase(fixture, "double Equal", testdoubleEqual);
    acu_fixtureAddTestCase(fixture, "double NotEqual NAN", testdoubleNotEqualNan);
    acu_fixtureAddTestCase(fixture, "double NotEqual", testdoubleNotEqual);
    acu_fixtureAddTestCase(fixture, "double Less", testdoubleLess);
    acu_fixtureAddTestCase(fixture, "double LessEqual", testdoubleLessEqual);
    acu_fixtureAddTestCase(fixture, "double LessEqualEq", testdoubleLessEqualEq);
    acu_fixtureAddTestCase(fixture, "double Greater", testdoubleGreater);
    acu_fixtureAddTestCase(fixture, "double GreaterEqualEq", testdoubleGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "double GreaterEqual", testdoubleGreaterEqual);

    acu_fixtureAddTestCase(fixture, "double Equal failed", testFaileddoubleEqual);
    acu_fixtureAddTestCase(fixture, "double NotEqual failed", testFaileddoubleNotEqual);
    acu_fixtureAddTestCase(fixture, "double Less failed", testFaileddoubleLess);
    acu_fixtureAddTestCase(fixture, "double LessEqual failed", testFaileddoubleLessEqual);
    acu_fixtureAddTestCase(fixture, "double Greater failed", testFaileddoubleGreater);
    acu_fixtureAddTestCase(fixture, "double GreaterEqual failed", testFaileddoubleGreaterEqual);

    return fixture;
}
