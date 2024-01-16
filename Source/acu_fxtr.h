#pragma once
#ifndef _ACU_FIXTURE_H_
#define _ACU_FIXTURE_H_

#include "acu_cmmn.h"

typedef struct ACU_ExecuteEnv_ ACU_ExecuteEnv;

typedef struct ACU_List_ ACU_List;

typedef struct ACU_Fixture_ {
    const char* name;
    const void* context;
    ACU_List* testCases;
} ACU_Fixture;

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char *name, void (*testFunc)(ACU_ExecuteEnv* environment, const void* context));

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context);

void acu_fixtureInit(ACU_Fixture* fixture, const char* name);

void acu_fixtureExecute(ACU_Fixture* fixture);

int acu_fixtureReport(ACU_Fixture* fixture);

void acu_fixtureDestroy(void* fixture);

#endif