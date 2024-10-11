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

#include "uchr_tst.h"

#include "tst_bed.h"

TESTBED(, unsignedChar, Equal, 'a', 'a')
TESTBED(, unsignedChar, NotEqual, 'a', 'b')
TESTBED(, unsignedChar, Less, 'a', 'b')
TESTBED(, unsignedChar, LessEqual, 'a', 'b')
TESTBED(Eq, unsignedChar, LessEqual, 'b', 'b')
TESTBED(, unsignedChar, Greater, 'b', 'a')
TESTBED(, unsignedChar, GreaterEqual, 'b', 'a')
TESTBED(Eq, unsignedChar, GreaterEqual, 'b', 'b')

TESTBED_FAILED(, unsignedChar, Equal, 'a', 'b')
TESTBED_FAILED(, unsignedChar, NotEqual, 'a', 'a')
TESTBED_FAILED(, unsignedChar, Less, 'a', 'a')
TESTBED_FAILED(, unsignedChar, LessEqual, 'c', 'b')
TESTBED_FAILED(, unsignedChar, Greater, 'b', 'c')
TESTBED_FAILED(, unsignedChar, GreaterEqual, 'b', 'c')


ACU_Fixture* unsignedCharFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "unsigned char tests");

    acu_addTestCase(fixture, "unsigned char Equal", testunsignedCharEqual);
    acu_addTestCase(fixture, "unsigned char NotEqual", testunsignedCharNotEqual);
    acu_addTestCase(fixture, "unsigned char Less", testunsignedCharLess);
    acu_addTestCase(fixture, "unsigned char LessEqual", testunsignedCharLessEqual);
    acu_addTestCase(fixture, "unsigned char LessEqualEq", testunsignedCharLessEqualEq);
    acu_addTestCase(fixture, "unsigned char Greater", testunsignedCharGreater);
    acu_addTestCase(fixture, "unsigned char GreaterEqualEq", testunsignedCharGreaterEqualEq);
    acu_addTestCase(fixture, "unsigned char GreaterEqual", testunsignedCharGreaterEqual);

    acu_addTestCase(fixture, "unsigned char Equal failed", testFailedunsignedCharEqual);
    acu_addTestCase(fixture, "unsigned char NotEqual failed", testFailedunsignedCharNotEqual);
    acu_addTestCase(fixture, "unsigned char Less failed", testFailedunsignedCharLess);
    acu_addTestCase(fixture, "unsigned char LessEqual failed", testFailedunsignedCharLessEqual);
    acu_addTestCase(fixture, "unsigned char Greater failed", testFailedunsignedCharGreater);
    acu_addTestCase(fixture, "unsigned char GreaterEqual failed", testFailedunsignedCharGreaterEqual);

    return fixture;
}
