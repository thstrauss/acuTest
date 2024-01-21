#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "include/acu_fxtr.h"
#include "include/acu_suit.h"
#include "include/acu_util.h"
#include "include/acu_list.h"

void acu_suiteAddFixture(ACU_Suite* suite, const ACU_Fixture* fixture)
{
    acu_listAppend(suite->testFixtures, (void*) fixture);
}

void acu_suiteInit(ACU_Suite* suite, const char* name)
{
    ACU_List* testFixtures = (ACU_List*)acu_emalloc(sizeof(ACU_List));
    acu_listInit(testFixtures, acu_fixtureDestroy);
    suite->testFixtures = testFixtures;
    suite->name = acu_estrdup(name);
}

void acu_suiteExecute(ACU_Suite* suite)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    while (fixtureElement != NULL) {
        acu_fixtureExecute((ACU_Fixture *) fixtureElement->data);
        fixtureElement = acu_listNext(fixtureElement);
    }
}

int acu_suiteReport(ACU_Suite* suite)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    int accumulatedResult = ACU_TEST_PASSED;

    printf("\n\rSuite: %s", suite->name);
    while (fixtureElement != NULL) {
        ACU_Fixture* fixture = (ACU_Fixture*)fixtureElement->data;
        int fixtureResult = acu_fixtureReport(fixture);
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
