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
#include <acu_suit.h>

static void acuTest_resultDestroy(ACU_Result* result) {
    if (result->message != NULL) {
        free(result->message);
    }
    if (result->file != NULL) {
        free(result->file);
    }
    free(result);
}

static ACU_Result* acuTest_resultMalloc() {
    return (ACU_Result*) acu_emalloc(sizeof(ACU_Result));
}

static int acuTest_run(ACU_TestCase* testCase, const void* context, void (*progress)(const ACU_TestCase* testCase)) {
    ACU_ExecuteEnv environment;
    ACU_Result* result = acuTest_resultMalloc();

    result->status = ACU_TEST_PASSED;
    result->message = NULL;
    result->file = NULL;
    result->line = -1;

    environment.result = result;

    do {
        switch (setjmp(environment.assertBuf)) {
            case 0: {
                while (1) {
                    testCase->testFunc(&environment, context);
                    break;
                }
                break;
            }
            case ACU_TEST_FAILED: {
                break;
            }
        }
    } while (0);
    testCase->result = result;
    progress(testCase);
    return result->status;
}

static void acuTest_testCaseDestroy(void* data) {
    ACU_TestCase* testCase = (ACU_TestCase*) data;
    free(testCase->name);
    if (testCase->result != NULL) {
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
}

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char* name, void (*testFunc)(ACU_ExecuteEnv* environment, const void* context)) {
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

int acu_fixtureExecute(ACU_Fixture* fixture, void (*progress)(const ACU_TestCase* testCase)) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);
    int result = ACU_TEST_PASSED;
    while (testElement != NULL) {
        result |= acuTest_run((ACU_TestCase*) testElement->data, fixture->context, progress);
        testElement = acu_listNext(testElement);
    }
    return result;
}

void acu_fixtureReport(FILE* stream, ACU_Fixture* fixture) {
    ACU_ListElement* testElement = acu_listHead(fixture->testCases);
   
    fprintf(stream, "  Fixture: %s\n\r", fixture->name);
    while (testElement != NULL) {
        ACU_TestCase* testCase = (ACU_TestCase*) testElement->data;
        if (testCase->result != NULL && testCase->result->status != ACU_TEST_PASSED) {
            fprintf(stream, "    %s: %s:%d:\n\r      %s\n\r", testCase->name, testCase->result->file, testCase->result->line, testCase->result->message);
        }
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
