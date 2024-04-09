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

void acu_suiteInit(ACU_Suite* suite, const char* name)
{
    ACU_List* testFixtures = acu_listMalloc();
    acu_listInit(testFixtures, acu_fixtureDestroy);
    suite->testFixtures = testFixtures;
    suite->name = acu_estrdup(name);
}

int acu_suiteExecute(ACU_Suite* suite, void (*progress)(const ACU_TestCase* testCase))
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);
    int result = ACU_TEST_PASSED;

    while (fixtureElement != NULL) {
        result |= acu_fixtureExecute((ACU_Fixture *) fixtureElement->data, progress);
        fixtureElement = acu_listNext(fixtureElement);
    }
    return result;
}

void acu_suiteReport(FILE* stream, ACU_Suite* suite)
{
    ACU_ListElement* fixtureElement = acu_listHead(suite->testFixtures);

    fprintf(stream, "Suite: %s\n\r", suite->name);
    while (fixtureElement != NULL) {
        ACU_Fixture* fixture = (ACU_Fixture*)fixtureElement->data;
        acu_fixtureReport(stream, fixture);
        fixtureElement = acu_listNext(fixtureElement);
    }
}

ACU_Suite* acu_suiteMalloc(void)
{
    return (ACU_Suite*) acu_emalloc(sizeof(ACU_Suite));
}

void acu_suiteDestroy(ACU_Suite* suite)
{
    free(suite->name);
    if (suite->testFixtures != NULL) {
        acu_listDestroy(suite->testFixtures);
    }
    free(suite);
}
