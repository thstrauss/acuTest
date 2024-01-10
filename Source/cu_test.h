#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

typedef struct CU_ExecuteEnv_ CU_ExecuteEnv;

typedef struct CU_List_ CU_List;

typedef struct CU_Fixture_ {
    const char* name;
    const void* context;
    CU_List* testCases;
} CU_Fixture;

void cu_fixtureAddTestCase(CU_Fixture* fixture, const char *name, void (*testFunc)(CU_ExecuteEnv* environment, const void* context));

void cu_fixtureSetContext(CU_Fixture* fixture, const void* context);

void cu_fixtureInit(CU_Fixture* fixture, const char* name);

void cu_fixtureExecute(CU_Fixture* fixture);

int cu_fixturReport(CU_Fixture* fixture);

void cu_fixtureDestroy(CU_Fixture* fixture);

#endif