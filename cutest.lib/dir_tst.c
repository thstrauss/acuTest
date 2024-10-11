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

#include <stddef.h>
#include <stdlib.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include "dir_tst.h"

#include <acu_dir.h>

static void dirTest(ACU_ExecuteEnv* environment, const void* context) {
    char* path = acu_getPath("c:\\test\\test.xxx");
    ACU_TRY
        ACU_assert_strEqual(environment, path, "c:\\test", "path not expected");
    ACU_FINALLY
        acu_free(path);
    ACU_ETRY
    UNUSED(context);
}

static void dirTestEmpty(ACU_ExecuteEnv* environment, const void* context) {
    char* path = acu_getPath("");
    ACU_TRY
        ACU_assert_strIsEmpty(environment, path, "path not empty");
    ACU_FINALLY
        acu_free(path);
    ACU_ETRY
        UNUSED(context);
}

static void dirTestNoSeparator(ACU_ExecuteEnv* environment, const void* context) {
    char* path = acu_getPath("test.xxx");
    ACU_TRY
        ACU_assert_strIsEmpty(environment, path, "path not empty");
    ACU_FINALLY
        acu_free(path);
    ACU_ETRY
        UNUSED(context);
}

static void dirTestNull(ACU_ExecuteEnv* environment, const void* context) {
    char* path = acu_getPath(NULL);
    ACU_assert_ptrIsNull(environment, path, "path not NULL");
    UNUSED(context);
}

ACU_Fixture* dirFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "dir Tests");

    acu_addTestCase(fixture, "dir test", dirTest);
    acu_addTestCase(fixture, "dir test null", dirTestNull);
    acu_addTestCase(fixture, "dir test empty", dirTestEmpty);
    acu_addTestCase(fixture, "dir test no separator", dirTestNoSeparator);

    return fixture;
}