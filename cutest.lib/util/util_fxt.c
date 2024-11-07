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

#include "util_fxt.h"

#include <acu_ldr.h>
#include <acu_fxtr.h>
#include <acu_util.h>
#include <acu_list.h>
#include <acu_tryc.h>

#include "dir_tst.h"
#include "hstb_tst.h"
#include "list_tst.h"
#include "misc_tst.h"
#include "mtch_tst.h"
#include "rnd_tst.h"
#include "stck_tst.h"
#include "util_tst.h"
#include "uuid_tst.h"

ACU_Fixture* utilFixture() {
    ACU_Fixture* suite = acu_mallocFixture();

    acu_initFixture(suite, "utility test suite");
    acu_addChildFixture(suite, dirTests());

    acu_addChildFixture(suite, hashTableTests());
    acu_addChildFixture(suite, listTests());
    acu_addChildFixture(suite, matchTests());
    acu_addChildFixture(suite, miscTests());

    acu_addChildFixture(suite, randomTests());
    acu_addChildFixture(suite, stackFixture());

    acu_addChildFixture(suite, utilTests());
    acu_addChildFixture(suite, uuidTests()); 

    return suite;
}


