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

#include "sizt_tst.h"

#include "tst_bed.h"

TESTBED(, size_t, Equal, 0, 0)
TESTBED(, size_t, NotEqual, 0, 1)
TESTBED(, size_t, Less, 0, 1)
TESTBED(, size_t, LessEqual, 0, 1)
TESTBED(Eq, size_t, LessEqual, 1, 1)
TESTBED(, size_t, Greater, 1, 0)
TESTBED(, size_t, GreaterEqual, 1, 0)
TESTBED(Eq, size_t, GreaterEqual, 1, 1)

TESTBED_FAILED(, size_t, Equal, 0, 1)
TESTBED_FAILED(, size_t, NotEqual, 1, 1)
TESTBED_FAILED(, size_t, Less, 1, 1)
TESTBED_FAILED(, size_t, LessEqual, 2, 1)
TESTBED_FAILED(, size_t, Greater, 1, 1)
TESTBED_FAILED(, size_t, GreaterEqual, 1, 2)

ACU_Fixture* size_tFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "size_t tests");

    acu_addTestCase(fixture, "size_t Equal", testsize_tEqual);
    acu_addTestCase(fixture, "size_t NotEqual", testsize_tNotEqual);
    acu_addTestCase(fixture, "size_t Less", testsize_tLess);
    acu_addTestCase(fixture, "size_t LessEqual", testsize_tLessEqual);
    acu_addTestCase(fixture, "size_t LessEqualEq", testsize_tLessEqualEq);
    acu_addTestCase(fixture, "size_t Greater", testsize_tGreater);
    acu_addTestCase(fixture, "size_t GreaterEqualEq", testsize_tGreaterEqualEq);
    acu_addTestCase(fixture, "size_t GreaterEqual", testsize_tGreaterEqual);
    acu_addTestCase(fixture, "size_t Equal failed", testFailedsize_tEqual);
    acu_addTestCase(fixture, "size_t NotEqual failed", testFailedsize_tNotEqual);
    acu_addTestCase(fixture, "size_t Less failed", testFailedsize_tLess);
    acu_addTestCase(fixture, "size_t LessEqual failed", testFailedsize_tLessEqual);
    acu_addTestCase(fixture, "size_t Greater failed", testFailedsize_tGreater);
    acu_addTestCase(fixture, "size_t GreaterEqual failed", testFailedsize_tGreaterEqual);
    return fixture;
}
