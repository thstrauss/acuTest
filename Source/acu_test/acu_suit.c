#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <acu_fxtr.h>
#include <acu_list.h>
#include <acu_suit.h>
#include <acu_util.h>

void acu_suiteAddFixture(ACU_Suite* suite, const ACU_Fixture* fixture)
{
    acu_listAppend(suite->testFixtures, (void*) fixture);
}

void acu_suiteInit(ACU_Suite* suite, const char* name, void (*progress)(const ACU_Result* result))
{
    ACU_List* testFixtures = (ACU_List*)acu_emalloc(sizeof(ACU_List));
    acu_listInit(testFixtures, acu_fixtureDestroy);
    suite->testFixtures = testFixtures;
    suite->name = acu_estrdup(name);
    suite->progress = progress;
}

void acu_suiteExecute(FILE* stream, ACU_Suite* suite)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    while (fixtureElement != NULL) {
        ((ACU_Fixture*)fixtureElement->data)->progress = suite->progress;
        acu_fixtureExecute((ACU_Fixture *) fixtureElement->data);
        ((ACU_Fixture*)fixtureElement->data)->progress = NULL;
        fixtureElement = acu_listNext(fixtureElement);
    }
    fprintf(stream, "\n\r");
}

int acu_suiteReport(FILE* stream, ACU_Suite* suite)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    int accumulatedResult = ACU_TEST_PASSED;

    fprintf(stream, "Suite: %s\n\r", suite->name);
    while (fixtureElement != NULL) {
        ACU_Fixture* fixture = (ACU_Fixture*)fixtureElement->data;
        int fixtureResult = acu_fixtureReport(stream, fixture);
        if (fixtureResult != ACU_TEST_PASSED) {
            accumulatedResult = fixtureResult;
        }
        fixtureElement = acu_listNext(fixtureElement);
    }

    return accumulatedResult;
}

void acu_suiteDestroy(ACU_Suite* suite)
{
    free(suite->name);
    if (suite->testFixtures != NULL) {
        acu_listDestroy(suite->testFixtures);
    }
    memset(suite, 0, sizeof(ACU_Suite));
}
