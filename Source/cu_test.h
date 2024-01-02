#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include "cu_list.h"
#include "cu_tenv.h"

typedef struct CU_Fixture_ {
	const char* name;
	CU_List* testCases;
} CU_Fixture;


void cuTest_addTestCase(CU_Fixture* fixture, const char *name, void (*testFunc)(CU_ExecuteEnv* environment));

void cuTest_init(CU_Fixture* fixture, const char* name);

void cuTest_execute(CU_Fixture* fixture);

int cuTest_report(CU_Fixture* fixture);

void cuTest_destroy(CU_Fixture* fixture);

#endif