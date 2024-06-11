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
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_list.h>
#include <acu_tryc.h>


#include "dir_tst.h"
#include "fxtr_tst.h"
#include "flt_test.h"
#include "tryc_tst.h"
#include "int_tst.h"
#include "uint_tst.h"
#include "long_tst.h"
#include "ulng_tst.h"
#include "char_tst.h"
#include "uchr_tst.h"
#include "shrt_tst.h"
#include "usht_tst.h"
#include "ptr_tst.h"
#include "str_tst.h"
#include "list_tst.h"
#include "mtch_tst.h"
#include "rnd_tst.h"
#include "rslt_tst.h"
#include "util_tst.h"
#include "uuid_tst.h"

ACU_Entry* acu_init() {
    ACU_Suite* suite = acu_suiteMalloc();
    ACU_Entry* entry = acu_entryMalloc();

    acu_entryInit(entry, suite);

    acu_suiteInit(suite, "acu test suite");

    acu_suiteAddFixture(suite, dirFixture());
    acu_suiteAddFixture(suite, fixtureFixture());
    acu_suiteAddFixture(suite, floatFixture());
    acu_suiteAddFixture(suite, intFixture());
    acu_suiteAddFixture(suite, listFixture());
    acu_suiteAddFixture(suite, longFixture());
    acu_suiteAddFixture(suite, matchFixture());
    acu_suiteAddFixture(suite, ptrFixture());
    acu_suiteAddFixture(suite, randomFixture());
    acu_suiteAddFixture(suite, resultFixture());
    acu_suiteAddFixture(suite, shortFixture());
    acu_suiteAddFixture(suite, signedCharFixture());
    acu_suiteAddFixture(suite, strFixture());
    acu_suiteAddFixture(suite, tryCatchFixture());
    acu_suiteAddFixture(suite, unsignedCharFixture());
    acu_suiteAddFixture(suite, unsignedIntFixture());
    acu_suiteAddFixture(suite, unsignedLongFixture());
    acu_suiteAddFixture(suite, unsignedShortFixture());
    acu_suiteAddFixture(suite, utilFixture());
    acu_suiteAddFixture(suite, uuidFixture());


    return entry;
}


