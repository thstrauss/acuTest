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
#include <acu_suit.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include "uuid_tst.h"

#include <acu_uuid.h>
#include <acu_rand.h>
#include <time.h>
#include <stdio.h>

static void uuidTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_UUID uuid;
    ACU_UUID parsed = {0,0,0,0};
    char buffer[] = "00000000-0000-0000-0000-000000000000";
    acu_initUuid(&uuid);
    acu_initUuid(&uuid);
    acu_formatUuid(buffer, &uuid);
    printf("\n\r%08lx %08lx %08lx %08lx, %s\n\r", uuid.longs[0], uuid.longs[1], uuid.longs[2], uuid.longs[3], buffer);

    acu_parseUuid(buffer, &parsed);
    printf("\n\r%08lx %08lx %08lx %08lx", parsed.longs[0], parsed.longs[1], parsed.longs[2], parsed.longs[3]);

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

ACU_Fixture* uuidFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "uuid Tests");

    acu_fixtureAddTestCase(fixture, "uuid test", uuidTest);
    acu_fixtureAddTestCase(fixture, "compare uuid test", compareTest);


    return fixture;
}