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

#include "uint_tst.h"

#include "tst_bed.h"

TESTBED(, unsignedInt, Equal, 0, 0)
TESTBED(, unsignedInt, NotEqual, 0, 1)
TESTBED(, unsignedInt, Less, 0, 1)
TESTBED(, unsignedInt, LessEqual, 0, 1)
TESTBED(Eq, unsignedInt, LessEqual, 1, 1)
TESTBED(, unsignedInt, Greater, 1, 0)
TESTBED(, unsignedInt, GreaterEqual, 1, 0)
TESTBED(Eq, unsignedInt, GreaterEqual, 1, 1)

TESTBED_FAILED(, unsignedInt, Equal, 0, 1)
TESTBED_FAILED(, unsignedInt, NotEqual, 1, 1)
TESTBED_FAILED(, unsignedInt, Less, 1, 1)
TESTBED_FAILED(, unsignedInt, LessEqual, 2, 1)
TESTBED_FAILED(, unsignedInt, Greater, 1, 1)
TESTBED_FAILED(, unsignedInt, GreaterEqual, 1, 2)

ACU_Fixture* unsignedIntFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "unsigned int tests");

    acu_addTestCase(fixture, "unsigned int Equal", testunsignedIntEqual);
    acu_addTestCase(fixture, "unsigned int NotEqual", testunsignedIntNotEqual);
    acu_addTestCase(fixture, "unsigned int Less", testunsignedIntLess);
    acu_addTestCase(fixture, "unsigned int LessEqual", testunsignedIntLessEqual);
    acu_addTestCase(fixture, "unsigned int LessEqualEq", testunsignedIntLessEqualEq);
    acu_addTestCase(fixture, "unsigned int Greater", testunsignedIntGreater);
    acu_addTestCase(fixture, "unsigned int GreaterEqualEq", testunsignedIntGreaterEqualEq);
    acu_addTestCase(fixture, "unsigned int GreaterEqual", testunsignedIntGreaterEqual);

    acu_addTestCase(fixture, "unsigned int Equal failed", testFailedunsignedIntEqual);
    acu_addTestCase(fixture, "unsigned int NotEqual failed", testFailedunsignedIntNotEqual);
    acu_addTestCase(fixture, "unsigned int Less failed", testFailedunsignedIntLess);
    acu_addTestCase(fixture, "unsigned int LessEqual failed", testFailedunsignedIntLessEqual);
    acu_addTestCase(fixture, "unsigned int Greater failed", testFailedunsignedIntGreater);
    acu_addTestCase(fixture, "unsigned int GreaterEqual failed", testFailedunsignedIntGreaterEqual);

    return fixture;
}
