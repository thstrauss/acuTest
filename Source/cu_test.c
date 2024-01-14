#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "cu_test.h"
#include "cu_tcse.h"
#include "cu_cmmn.h"
#include "cu_list.h"
#include "cu_eenv.h"
#include "cu_utils.h"

static void cuTest_destroyResult(ACU_Result* result) {
    if (result->message != NULL) {
        free(result->message);
    }
    free(result);
}

static void cuTest_run(CU_TestCase* testCase, const void* context) {
    ACU_ExecuteEnv environment;

    ACU_Result* result = (ACU_Result*)cu_emalloc(sizeof(ACU_Result));
    if (result == NULL) {
        printf("E");
        return;
    }
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

static void cuTest_destroyTestCase(void* data) {
    CU_TestCase* testCase = (CU_TestCase*) data;
    free(testCase->name);
    if (testCase->result != NULL) {
        cuTest_destroyResult(testCase->result);
    }
}

void acu_fixtureInit(ACU_Fixture* fixture, const char* name) {
    ACU_List* testCases = (ACU_List*) cu_emalloc(sizeof(ACU_List));
    acu_listInit(testCases, cuTest_destroyTestCase);
    fixture->testCases = testCases;
    fixture->name = cu_estrdup(name);
}

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char* name, void (*testFunc)(ACU_ExecuteEnv* environment, const void* context)) {
    CU_TestCase* testCase = (CU_TestCase*)malloc(sizeof(CU_TestCase));
    if (testCase != NULL) {
        testCase->name = cu_estrdup(name);
        testCase->testFunc = testFunc;
        acu_listAppend(fixture->testCases, (void*)testCase);
    }
}

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context)
{
    fixture->context = context;
}

void acu_fixtureExecute(ACU_Fixture* fixture) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);

    while (testElement != NULL) {
        cuTest_run((CU_TestCase*) testElement->data, fixture->context);
        testElement = acu_listNext(testElement);
    }
}

int acu_fixturReport(ACU_Fixture* fixture) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);

    int accumulatedResult = ACU_TEST_PASSED;
    
    printf("\n\r");
    while (testElement != NULL) {
        CU_TestCase* testCase = (CU_TestCase*) testElement->data;
        if (testCase->result->status != ACU_TEST_PASSED) {
            printf("%s: %s\n\r", testCase->name, testCase->result->message);
            accumulatedResult = testCase->result->status;
        }
        testElement = acu_listNext(testElement);
    }

    return accumulatedResult;
}

void acu_fixtureDestroy(ACU_Fixture* fixture) {
    acu_listDestroy(fixture->testCases);
}
