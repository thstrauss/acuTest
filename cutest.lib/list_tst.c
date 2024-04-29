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

#include "list_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_suit.h>

static void emptyList(ACU_ExecuteEnv* environment, const void* context) {
    UNUSED(environment);
    UNUSED(context);
}

ACU_Fixture* listFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "list tests");

    acu_fixtureAddTestCase(fixture, "emptyList", emptyList);

    return fixture;
}