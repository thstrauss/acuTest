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
#include <stdio.h>

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_util.h>
#include <acu_suit.h>
#include <acu_tryc.h>
#include <acu_list.h>

#include "util_tst.h"

static int visited = 0;
static const char* actualErrorMessage;
static enum ACU_Level actualErrorLevel;

static void error(enum ACU_Level errorLevel, const char* errorMessage) {
    visited++;
    actualErrorMessage = errorMessage;
    actualErrorLevel = errorLevel;
}

static void errorHandlerTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TRY
        visited = 0;

        acu_setProgName("acu_test");
        acu_setErrorHandler(error); 

        acu_eprintf("test:");
        ACU_assert(environment, int, Equal, visited, 1, "Error handler not used");
        ACU_assert(environment, int, Equal, actualErrorLevel, acu_error, "no error");
        ACU_assert_strContains(environment, actualErrorMessage, "acu_test", "Does not contain program name");
        ACU_assert_strContains(environment, actualErrorMessage, "test:", "Does not contain message");
    ACU_FINALLY
        acu_setErrorHandler((ACU_ErrorHandlerFunc*) NULL);
        acu_setProgName(NULL);
    ACU_ETRY
    UNUSED(context);
}

static void warningErrorHandlerTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TRY
        visited = 0;

        acu_setProgName("acu_test");
        acu_setErrorHandler(error);

        acu_wprintf("test:");
        ACU_assert(environment, int, Equal, visited, 1, "Error handler not used");
        ACU_assert(environment, int, Equal, actualErrorLevel, acu_warning, "No warning");
        ACU_assert_strContains(environment, actualErrorMessage, "acu_test", "Does not contain program name");
        ACU_assert_strContains(environment, actualErrorMessage, "test:", "Does not contain message");
    ACU_FINALLY
        acu_setErrorHandler((ACU_ErrorHandlerFunc*)NULL);
        acu_setProgName(NULL);
    ACU_ETRY
    UNUSED(context);
}

static void programNameTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_TRY
        acu_setProgName("acu_test");
        ACU_assert_strEqual(environment, acu_progName(), "acu_test", "program name not set.");
    ACU_FINALLY
        acu_setProgName(NULL);
    ACU_ETRY
        UNUSED(context);
}


ACU_Fixture* utilFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "utility Tests");
    acu_fixtureAddTestCase(fixture, "error Handler", errorHandlerTest);
    acu_fixtureAddTestCase(fixture, "warning Handler", warningErrorHandlerTest);
    acu_fixtureAddTestCase(fixture, "program Name", programNameTest);

    return fixture;
}