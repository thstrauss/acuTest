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

void cuTest_addTestCase(CU_Fixture* fixture, const char *name, void (*testFunc)(CU_ExecuteEnv* environment, const void* context));

void cuTest_addContext(CU_Fixture* fixture, const void* context);

void cuTest_init(CU_Fixture* fixture, const char* name);

void cuTest_execute(CU_Fixture* fixture);

int cuTest_report(CU_Fixture* fixture);

void cuTest_destroy(CU_Fixture* fixture);

#endif