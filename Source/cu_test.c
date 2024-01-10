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

static void cuTest_destroyResult(CU_Result* result) {
    if (result->message != NULL) {
        free(result->message);
    }
    free(result);
}

static void cuTest_run(CU_TestCase* testCase, const void* context) {
    CU_ExecuteEnv environment;

    CU_Result* result = (CU_Result*)cu_emalloc(sizeof(CU_Result));
    if (result == NULL) {
        printf("E");
        return;
    }
    result->status = CU_TEST_PASSED;
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
            case CU_TEST_FAILED: {
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

void cuTest_init(CU_Fixture* fixture, const char* name) {
    CU_List* testCases = (CU_List*) cu_emalloc(sizeof(CU_List));
    cu_listInit(testCases, cuTest_destroyTestCase);
    fixture->testCases = testCases;
    fixture->name = cu_estrdup(name);
}

void cuTest_addTestCase(CU_Fixture* fixture, const char* name, void (*testFunc)(CU_ExecuteEnv* environment, const void* context)) {
    CU_TestCase* testCase = (CU_TestCase*)malloc(sizeof(CU_TestCase));
    if (testCase != NULL) {
        testCase->name = cu_estrdup(name);
        testCase->testFunc = testFunc;
        cu_listAppend(fixture->testCases, (void*)testCase);
    }
}

void cuTest_addContext(CU_Fixture* fixture, const void* context)
{
    fixture->context = context;
}

void cuTest_execute(CU_Fixture* fixture) {
    CU_ListElement* testElement = cu_listHead(fixture->testCases);

    while (testElement != NULL) {
        cuTest_run((CU_TestCase*) testElement->data, fixture->context);
        testElement = cu_listNext(testElement);
    }
}

int cuTest_report(CU_Fixture* fixture) {
    CU_ListElement* testElement = cu_listHead(fixture->testCases);

    int accumulatedResult = CU_TEST_PASSED;
    
    printf("\n\r");
    while (testElement != NULL) {
        CU_TestCase* testCase = (CU_TestCase*) testElement->data;
        if (testCase->result->status != CU_TEST_PASSED) {
            printf("%s: %s\n\r", testCase->name, testCase->result->message);
            accumulatedResult = testCase->result->status;
        }
        testElement = cu_listNext(testElement);
    }

    return accumulatedResult;
}

void cuTest_destroy(CU_Fixture* fixture) {
    cu_listDestroy(fixture->testCases);
}
