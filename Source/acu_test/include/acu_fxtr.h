#pragma once
#ifndef _ACU_FIXTURE_H_
#define _ACU_FIXTURE_H_

#include <stdio.h>

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_list.h>
#include <acu_rslt.h>
#include <acu_tcse.h>

typedef struct ACU_Fixture_ {
    const char* name;
    const void* context;
    ACU_List* testCases;
} ACU_Fixture;

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char *name, void (*testFunc)(ACU_ExecuteEnv* environment, const void* context));

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context);

void acu_fixtureInit(ACU_Fixture* fixture, const char* name);

int acu_fixtureExecute(ACU_Fixture* fixture, void (*progress)(const ACU_TestCase* testCase));

void acu_fixtureReport(FILE* stream, ACU_Fixture* fixture);

ACU_Fixture* acu_fixtureMalloc();

void acu_fixtureDestroy(void* fixture);

#endif
