#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include "cu_list.h"
#include "cu_tenv.h"

typedef struct Fixture_ {
	const char* name;
	CU_List* testCases;
} Fixture;


void cuTest_addTestCase(Fixture* fixture, const char *name, void (*testFunc)(ExecuteEnv* environment));

void cuTest_init(Fixture* fixture, const char* name);

void cuTest_execute(Fixture* fixture);

int cuTest_report(Fixture* fixture);

void cuTest_destroy(Fixture* fixture);

#endif