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

#include "perf_fxt.h"

#include <acu_ldr.h>
#include <acu_fxtr.h>
#include <acu_util.h>
#include <acu_list.h>
#include <acu_tryc.h>

#include "palc_tst.h"
#include "plst_tst.h"
#include "pmsc_tst.h"
#include "pmth_tst.h"

ACU_Fixture* performanceFixture() {
    ACU_Fixture* suite = acu_mallocFixture();

    acu_initFixture(suite, "performance test suite");

    acu_addChildFixture(suite, allocPerformanceFixture());
    acu_addChildFixture(suite, listPerformanceFixture());
    acu_addChildFixture(suite, miscPerformanceFixture());
    acu_addChildFixture(suite, matchPerformanceFixture());

    return suite;
}


