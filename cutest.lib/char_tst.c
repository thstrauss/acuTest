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

#include "char_tst.h"

#include "tst_bed.h"

TESTBED(, signedChar, Equal, 'a', 'a')
TESTBED(, signedChar, NotEqual, 'a', 'b')
TESTBED(, signedChar, Less, 'a', 'b')
TESTBED(, signedChar, LessEqual, 'a', 'b')
TESTBED(Eq, signedChar, LessEqual, 'b', 'b')
TESTBED(, signedChar, Greater, 'b', 'a')
TESTBED(, signedChar, GreaterEqual, 'b', 'a')
TESTBED(Eq, signedChar, GreaterEqual, 'b', 'b')

ACU_Fixture* signedCharFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "signed char tests");

    acu_fixtureAddTestCase(fixture, "signed char Equal", testsignedCharEqual);
    acu_fixtureAddTestCase(fixture, "signed char NotEqual", testsignedCharNotEqual);
    acu_fixtureAddTestCase(fixture, "signed char Less", testsignedCharLess);
    acu_fixtureAddTestCase(fixture, "signed char LessEqual", testsignedCharLessEqual);
    acu_fixtureAddTestCase(fixture, "signed char LessEqualEq", testsignedCharLessEqualEq);
    acu_fixtureAddTestCase(fixture, "signed char Greater", testsignedCharGreater);
    acu_fixtureAddTestCase(fixture, "signed char GreaterEqualEq", testsignedCharGreaterEqualEq);
    acu_fixtureAddTestCase(fixture, "signed char GreaterEqual", testsignedCharGreaterEqual);

    return fixture;
}
