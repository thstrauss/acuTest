#pragma once
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

#include <stdio.h>

#include <acu_fxtr.h>
#include <acu_list.h>

typedef struct ACU_Suite_ {
    char* name;
    ACU_List* testFixtures;
    void (*progress)(const ACU_Result* result);
} ACU_Suite;

void acu_suiteAddFixture(ACU_Suite* suite, const ACU_Fixture* fixture);

void acu_suiteInit(ACU_Suite* suite, const char* name, void (*progress)(const ACU_Result* result));

void acu_suiteExecute(FILE* stream, ACU_Suite* suite);

int acu_suiteReport(FILE * stream, ACU_Suite* suite);

void acu_suiteDestroy(ACU_Suite* suite);


#endif