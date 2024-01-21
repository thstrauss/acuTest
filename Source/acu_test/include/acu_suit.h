#pragma once
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

typedef struct ACU_List_ ACU_List;

typedef struct ACU_Fixture_ ACU_Fixture;

typedef struct ACU_Suite_ {
    char* name;
    ACU_List* testFixtures;
} ACU_Suite;

void acu_suiteAddFixture(ACU_Suite* suite, const ACU_Fixture* fixture);

void acu_suiteInit(ACU_Suite* suite, const char* name);

void acu_suiteExecute(ACU_Suite* suite);

int acu_suiteReport(ACU_Suite* suite);

void acu_suiteDestroy(ACU_Suite* suite);


#endif