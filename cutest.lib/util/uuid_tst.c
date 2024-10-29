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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include "uuid_tst.h"

#include <acu_uuid.h>
#include <acu_rand.h>
#include <time.h>

static void uuidTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_UUID uuid;
    ACU_UUID parsed = {0,0,0,0};
    char guidBuffer[] = "00000000-0000-0000-0000-000000000000";
    acu_initUuid(&uuid);
    acu_initUuid(&uuid);
    acu_formatUuid(guidBuffer, &uuid);

    acu_parseUuid(guidBuffer, &parsed);

    ACU_assert(environment, int, Equal, acu_compareUuid(&uuid, &parsed), 1, "not equal");

    UNUSED(context);
}

static void compareTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_UUID uuid1;
    ACU_UUID uuid2;

    acu_initUuid(&uuid1);
    acu_initUuid(&uuid2);

    ACU_assert(environment, int, Equal, acu_compareUuid(&uuid1, &uuid2), 0, "");
    ACU_assert(environment, int, Equal, acu_compareUuid(&uuid1, &uuid1), 1, "");
    ACU_assert(environment, int, Equal, acu_compareUuid(NULL, &uuid1), 0, "");
    ACU_assert(environment, int, Equal, acu_compareUuid(&uuid1, NULL), 0, "");
    ACU_assert(environment, int, Equal, acu_compareUuid(NULL, NULL), 0, "");
    UNUSED(context);
}

ACU_Fixture* uuidTests(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "uuid Tests");

    acu_addTestCase(fixture, "uuid test", uuidTest);
    acu_addTestCase(fixture, "compare uuid test", compareTest);


    return fixture;
}