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

#include <time.h>
#include <stdlib.h>
#include <stddef.h>

#include "acu_eenv.h"
#include "acu_fxtr.h"
#include "acu_list.h"
#include "acu_rslt.h"
#include "acu_tcse.h"
#include "acu_util.h"
#include "acu_suit.h"
#include "acu_tryc.h"

static enum ACU_TestResult acuTest_run(ACU_TestCase* testCase, const void* context, ACU_Progress* progress) {
    ACU_ExecuteEnv environment;
    ACU_Result* result = acuTest_resultMalloc();
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    frame.exception = 0;

    environment.result = result;
    environment.exceptionFrame = &frame;

    acu_stackPush(frameStack, &frame);

    acuTest_resultInit(result);
    do {
        switch (setjmp(frame.exceptionBuf)) {
            case 0: {
                while (1) {
                    testCase->testFunc(&environment, context);
                    break;
                }
                break;
            }
            case ACU_EXCEPTION_ABORTED: {
                break;
            }
        }
    } while (0);
    result->end = clock();
    acu_stackDrop(frameStack);
    testCase->result = result;
    if (progress && progress->progress) {
        progress->progress(testCase, progress->context);
    }
    return result->status;
}

static void acuTest_testCaseDestroy(void* data) {
    ACU_TestCase* testCase = (ACU_TestCase*) data;
    free(testCase->name);
    if (testCase->result) {
        acuTest_resultDestroy(testCase->result);
    }
}

static ACU_TestCase* acuTest_testCaseMalloc(void) {
    return (ACU_TestCase*)acu_emalloc(sizeof(ACU_TestCase));
}

void acu_fixtureInit(ACU_Fixture* fixture, const char* name) {
    ACU_List* testCases = acu_listMalloc();
    acu_listInit(testCases, acuTest_testCaseDestroy);
    fixture->testCases = testCases;
    fixture->name = acu_estrdup(name);
    fixture->start = (clock_t)-1;
    fixture->end = (clock_t)-1;
}

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char* name, ACU_TestFunc testFunc) {
    ACU_TestCase* testCase = acuTest_testCaseMalloc();
    testCase->name = acu_estrdup(name);
    testCase->testFunc = testFunc;
    testCase->result = NULL;
    testCase->fixture = fixture;
    acu_listAppend(fixture->testCases, (void*)testCase);
}

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context)
{
    fixture->context = context;
}

enum ACU_TestResult acu_fixtureExecute(ACU_Fixture* fixture, ACU_Progress* progress) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);
    enum ACU_TestResult result = ACU_TEST_PASSED;
    fixture->start = clock();
    while (testElement != NULL) {
        result = acuTest_calcResult(result, acuTest_run((ACU_TestCase*) testElement->data, fixture->context, progress));
        testElement = acu_listNext(testElement);
    }
    fixture->end = clock();
    return result;
}

void acu_fixtureAccept(const ACU_Fixture* fixture, ACU_Visitor* visitor) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);
   
    while (testElement != NULL) {
        ACU_TestCase* testCase = (ACU_TestCase*) testElement->data;
        visitor->visitor(testCase, visitor->context);
        testElement = acu_listNext(testElement);
    }
}

ACU_Fixture* acu_fixtureMalloc(void)
{
    return (ACU_Fixture*)acu_emalloc(sizeof(ACU_Fixture));
}

void acu_fixtureDestroy(void* fixture) {
    acu_listDestroy(((ACU_Fixture*) fixture)->testCases);
    free(fixture);
}
