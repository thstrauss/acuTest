#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_fxtr.h>
#include <acu_list.h>
#include <acu_rslt.h>
#include <acu_tcse.h>
#include <acu_util.h>

static void acuTest_destroyResult(ACU_Result* result) {
    if (result->message != NULL) {
        free(result->message);
    }
    free(result);
}

static void acuTest_run(ACU_TestCase* testCase, const void* context) {
    ACU_ExecuteEnv environment;
    ACU_Result* result = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));

    result->status = ACU_TEST_PASSED;
    result->message = NULL;

    environment.result = result;

    do {
        switch (setjmp(environment.assertBuf)) {
            case 0: {
                while (1) {
                    testCase->testFunc(&environment, context);
                    break;
                }
                printf(".");
                break;
            }
            case ACU_TEST_FAILED: {
                printf("F");
                break;
            }
        }
    } while (0);
    testCase->result = result;
}

static void acuTest_destroyTestCase(void* data) {
    ACU_TestCase* testCase = (ACU_TestCase*) data;
    free(testCase->name);
    if (testCase->result != NULL) {
        acuTest_destroyResult(testCase->result);
    }
}

void acu_fixtureInit(ACU_Fixture* fixture, const char* name) {
    ACU_List* testCases = (ACU_List*) acu_emalloc(sizeof(ACU_List));
    acu_listInit(testCases, acuTest_destroyTestCase);
    fixture->testCases = testCases;
    fixture->name = acu_estrdup(name);
}

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char* name, void (*testFunc)(ACU_ExecuteEnv* environment, const void* context)) {
    ACU_TestCase* testCase = (ACU_TestCase*)acu_emalloc(sizeof(ACU_TestCase));
    testCase->name = acu_estrdup(name);
    testCase->testFunc = testFunc;
    testCase->result = NULL;
    acu_listAppend(fixture->testCases, (void*)testCase);
}

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context)
{
    fixture->context = context;
}

void acu_fixtureExecute(ACU_Fixture* fixture) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);

    while (testElement != NULL) {
        acuTest_run((ACU_TestCase*) testElement->data, fixture->context);
        testElement = acu_listNext(testElement);
    }
}

int acu_fixtureReport(ACU_Fixture* fixture) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);

    int accumulatedResult = ACU_TEST_PASSED;
    
    printf("\n\r  Fixture: %s", fixture->name);
    while (testElement != NULL) {
        ACU_TestCase* testCase = (ACU_TestCase*) testElement->data;
        if (testCase->result != NULL && testCase->result->status != ACU_TEST_PASSED) {
            printf("\n\r    %s: %s", testCase->name, testCase->result->message);
            accumulatedResult = testCase->result->status;
        }
        testElement = acu_listNext(testElement);
    }

    return accumulatedResult;
}

void acu_fixtureDestroy(void* fixture) {
    acu_listDestroy(((ACU_Fixture*) fixture)->testCases);
    memset(fixture, 0, sizeof(ACU_Fixture));
}
