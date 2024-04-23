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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <acu_fxtr.h>
#include <acu_list.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rprt.h>

void acu_suiteAddFixture(ACU_Suite* suite, ACU_Fixture* fixture)
{
    fixture->suite = suite;
    acu_listAppend(suite->testFixtures, (void*) fixture);
}

void acu_suiteInit(ACU_Suite* suite, const char* name)
{
    ACU_List* testFixtures = acu_listMalloc();
    acu_listInit(testFixtures, acu_fixtureDestroy);
    suite->testFixtures = testFixtures;
    suite->name = acu_estrdup(name);
    suite->start = (clock_t)-1;
    suite->end = (clock_t)-1;
}

enum ACU_TestResult acu_suiteExecute(ACU_Suite* suite, ACU_ProgressFunc progress)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);
    enum ACU_TestResult result = ACU_TEST_PASSED;

    suite->start = clock();
    while (fixtureElement) {
        result = acuTest_calcResult(result, acu_fixtureExecute((ACU_Fixture*)fixtureElement->data, progress));
        fixtureElement = acu_listNext(fixtureElement);
    }
    suite->end = clock();
    return result;
}

void* acu_suiteReport(const ACU_Suite* suite, void* context, ACU_ReportFunc report)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    while (fixtureElement) {
        ACU_Fixture* fixture = (ACU_Fixture*)fixtureElement->data;
        context = acu_fixtureReport(fixture, context, report);
        fixtureElement = acu_listNext(fixtureElement);
    }
    return context;
}

ACU_Suite* acu_suiteMalloc(void)
{
    return (ACU_Suite*) acu_emalloc(sizeof(ACU_Suite));
}

void acu_suiteDestroy(ACU_Suite* suite)
{
    free(suite->name);
    if (suite->testFixtures) {
        acu_listDestroy(suite->testFixtures);
    }
    free(suite);
}
