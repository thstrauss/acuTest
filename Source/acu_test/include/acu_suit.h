#pragma once
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

#include <stdio.h>

#include <acu_fxtr.h>
#include <acu_list.h>

typedef struct ACU_Suite_ {
    char* name;
    ACU_List* testFixtures;
} ACU_Suite;

void acu_suiteAddFixture(ACU_Suite* suite, const ACU_Fixture* fixture);

void acu_suiteInit(ACU_Suite* suite, const char* name);

int acu_suiteExecute(ACU_Suite* suite, void (*progress)(const ACU_TestCase* testCase));

void acu_suiteReport(FILE * stream, ACU_Suite* suite);

ACU_Suite* acu_suiteMalloc();

void acu_suiteDestroy(ACU_Suite* suite);


#endif