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
#include <stdio.h>
#include <string.h>

#include "acu_eenv.h"
#include "acu_fxtr.h"
#include "acu_list.h"
#include "acu_rslt.h"
#include "acu_tcse.h"
#include "acu_util.h"
#include "acu_strg.h"
#include "acu_tryc.h"

static ACU_Stack* frameStack = NULL;

static enum ACU_TestResult acuTest_run(ACU_TestCase* testCase) {
    ACU_ExecuteEnv environment;
    ACU_Frame frame;
    ACU_StackElement stackElement;
    clock_t start;

    environment.result = &testCase->result;
    environment.exceptionFrame = &frame;
    stackElement.data = &frame;
    frame.exception = 0;

    ACU_PUSHSTACKELEMENT(frameStack, stackElement);

    ACU_PREPARERESULT(environment.result);
    start = clock();
    do {
        switch (setjmp(frame.exceptionBuf)) {
            case 0: {
                while (1) {
                    testCase->testFunc(&environment, testCase->context);
                    break;
                }
                break;
            }
            case ACU_EXCEPTION_ABORTED: {
                break;
            }
        }
    } while (0);
    environment.result->duration = clock() - start;
    ACU_DROPSTACKELEMENT(frameStack);
    if (testCase->progress) {
        acu_performProgress(testCase->progress, testCase);
    }
    return environment.result->status;
}

static void acuTest_testCaseDestroy(ACU_TestCase* testCase) {
    acu_free(testCase->name);
    acu_destroyResult(&testCase->result);
    acu_free(testCase);
}

#define acuTest_testCaseMalloc() ((ACU_TestCase*) acu_emalloc(sizeof(ACU_TestCase)))

void acu_initFixture(ACU_Fixture* fixture, const char* name) {
    ACU_List* testCases = acu_mallocList();
    ACU_List* childFixtures = acu_mallocList();
    acu_initList(testCases, (ACU_ListDestroyFunc*) acuTest_testCaseDestroy, NULL);
    acu_initList(childFixtures, (ACU_ListDestroyFunc*) acu_fixtureDestroy, NULL);
    fixture->testCases = testCases;
    fixture->childFixtures = childFixtures;
    fixture->name = acu_estrdup(name);
    fixture->duration = (clock_t)0;
}

void acu_addTestCase(ACU_Fixture* fixture, const char* name, ACU_TestFunc testFunc) {
    ACU_TestCase* testCase = acuTest_testCaseMalloc();
    testCase->name = acu_estrdup(name);
    testCase->testFunc = testFunc;
    testCase->fixture = fixture;
    acu_initResult(&testCase->result);
    acu_appendList(fixture->testCases, (void*)testCase);
}

void acu_addChildFixture(ACU_Fixture* fixture, ACU_Fixture* childFixture) {
    childFixture->parentFixture = fixture;
    acu_appendList(fixture->childFixtures, (void*) childFixture);
}

void acu_setFixtureContext(ACU_Fixture* fixture, const void* context)
{
    fixture->context = context;
}

enum ACU_TestResult acu_executeFixture(ACU_Fixture* fixture, ACU_Progress* progress) {
    ACU_ListElement* testElement = fixture->testCases->head;
    ACU_ListElement* childFixture = fixture->childFixtures->head;
    enum ACU_TestResult result = ACU_TEST_PASSED;
    if (!frameStack) {
        frameStack = acu_getFrameStack();
    }
    fixture->duration = 0;

    while (childFixture) {
        enum ACU_TestResult r = acu_executeFixture((ACU_Fixture*)childFixture->data, progress);
        result = acuTest_calcResult(result, r);
        fixture->duration += ((ACU_Fixture*)childFixture->data)->duration;
        childFixture = childFixture->next;
    }
    while (testElement) {
        ACU_TestCase* testCase = (ACU_TestCase*)testElement->data;
        enum ACU_TestResult r;
        testCase->context = fixture->context;
        testCase->progress = progress;
        r = acuTest_run(testCase);
        fixture->duration += (testCase->result).duration;

        result = acuTest_calcResult(result, r);
        testElement = testElement->next;
    }
    return result;
}

void acu_acceptFixture(const ACU_Fixture* fixture, ACU_ReportVisitor* visitor) {
    ACU_ListVisitor listVisitor;

    listVisitor.visitor = (ACU_ListVisitorFunc*) acu_acceptFixture;
    listVisitor.context = visitor;

    acu_acceptList(fixture->childFixtures, &listVisitor);

    listVisitor.visitor = (ACU_ListVisitorFunc*) visitor->visitor;
    listVisitor.context = visitor->context;

    acu_acceptList(fixture->testCases, &listVisitor);
}

void acu_fixtureDestroy(ACU_Fixture* fixture) {
    acu_destroyList(fixture->testCases);
    acu_free(fixture->testCases);
    acu_destroyList(fixture->childFixtures);
    acu_free(fixture->childFixtures);
    acu_free(fixture->name);
    acu_free(fixture);
}
