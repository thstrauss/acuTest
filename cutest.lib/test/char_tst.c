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

TESTBED_FAILED(, signedChar, Equal, 'a', 'b')
TESTBED_FAILED(, signedChar, NotEqual, 'a', 'a')
TESTBED_FAILED(, signedChar, Less, 'a', 'a')
TESTBED_FAILED(, signedChar, LessEqual, 'c', 'b')
TESTBED_FAILED(, signedChar, Greater, 'b', 'c')
TESTBED_FAILED(, signedChar, GreaterEqual, 'b', 'c')

ACU_Fixture* signedCharFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "signed char tests");

    acu_addTestCase(fixture, "signed char Equal", testsignedCharEqual);
    acu_addTestCase(fixture, "signed char NotEqual", testsignedCharNotEqual);
    acu_addTestCase(fixture, "signed char Less", testsignedCharLess);
    acu_addTestCase(fixture, "signed char LessEqual", testsignedCharLessEqual);
    acu_addTestCase(fixture, "signed char LessEqualEq", testsignedCharLessEqualEq);
    acu_addTestCase(fixture, "signed char Greater", testsignedCharGreater);
    acu_addTestCase(fixture, "signed char GreaterEqualEq", testsignedCharGreaterEqualEq);
    acu_addTestCase(fixture, "signed char GreaterEqual", testsignedCharGreaterEqual);

    acu_addTestCase(fixture, "signed char Equal failed", testFailedsignedCharEqual);
    acu_addTestCase(fixture, "signed char NotEqual failed", testFailedsignedCharNotEqual);
    acu_addTestCase(fixture, "signed char Less failed", testFailedsignedCharLess);
    acu_addTestCase(fixture, "signed char LessEqual failed", testFailedsignedCharLessEqual);
    acu_addTestCase(fixture, "signed char Greater failed", testFailedsignedCharGreater);
    acu_addTestCase(fixture, "signed char GreaterEqual failed", testFailedsignedCharGreaterEqual);

    return fixture;
}
