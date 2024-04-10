#pragma once
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

#include <stdio.h>

#include <acu_fxtr.h>

struct ACU_List_;

typedef struct ACU_Suite_ {
    char* name;
    struct ACU_List_* testFixtures;
} ACU_Suite;

void acu_suiteAddFixture(ACU_Suite* suite, ACU_Fixture* fixture);

void acu_suiteInit(ACU_Suite* suite, const char* name);

int acu_suiteExecute(ACU_Suite* suite, ACU_ProgressFunc progress);

void* acu_suiteReport(ACU_Suite* suite, void* context, ACU_ReportFunc report);

ACU_Suite* acu_suiteMalloc(void);

void acu_suiteDestroy(ACU_Suite* suite);


#endif