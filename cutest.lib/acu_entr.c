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

#include <acu_ldr.h>
#include <acu_fxtr.h>
#include <acu_util.h>
#include <acu_list.h>
#include <acu_tryc.h>

#include "asrt_tst.h"
#include "char_tst.h"
#include "dir_tst.h"
#include "flt_test.h"
#include "fxtr_tst.h"
#include "hstb_tst.h"
#include "int_tst.h"
#include "list_tst.h"
#include "long_tst.h"
#include "misc_tst.h"
#include "mtch_tst.h"
#include "ptr_tst.h"
#include "rnd_tst.h"
#include "rprt_tst.h"
#include "rslt_tst.h"
#include "shrt_tst.h"
#include "sizt_tst.h"
#include "str_tst.h"
#include "tryc_tst.h"
#include "uchr_tst.h"
#include "uint_tst.h"
#include "ulng_tst.h"
#include "usht_tst.h"
#include "util_tst.h"
#include "uuid_tst.h"

ACU_Entry* acu_init() {
    ACU_Fixture* suite = acu_mallocFixture();
    ACU_Entry* entry = acu_mallocEntry();

    acu_initEntry(entry, suite);

    acu_initFixture(suite, "acu test suite");
    
    acu_addChildFixture(suite, assertFixture());
    acu_addChildFixture(suite, miscFixture());
    acu_addChildFixture(suite, dirFixture());
    acu_addChildFixture(suite, fixtureFixture());
    acu_addChildFixture(suite, floatFixture());
    acu_addChildFixture(suite, hashTableFixture());
    acu_addChildFixture(suite, intFixture());
    acu_addChildFixture(suite, listFixture());
    acu_addChildFixture(suite, longFixture());
    acu_addChildFixture(suite, matchFixture());
    acu_addChildFixture(suite, ptrFixture());
    acu_addChildFixture(suite, randomFixture());
    acu_addChildFixture(suite, resultFixture());
    acu_addChildFixture(suite, reportFixture());
    acu_addChildFixture(suite, shortFixture());
    acu_addChildFixture(suite, signedCharFixture());
    acu_addChildFixture(suite, strFixture());
    acu_addChildFixture(suite, size_tFixture());
    acu_addChildFixture(suite, tryCatchFixture());
    acu_addChildFixture(suite, unsignedCharFixture());
    acu_addChildFixture(suite, unsignedIntFixture());
    acu_addChildFixture(suite, unsignedLongFixture());
    acu_addChildFixture(suite, unsignedShortFixture());
    acu_addChildFixture(suite, utilFixture());
    acu_addChildFixture(suite, uuidFixture()); 

    return entry;
}


