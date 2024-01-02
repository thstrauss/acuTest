#pragma once
#ifndef _CU_TEST_H_
#define _CU_TEST_H_

#include "cu_list.h"
#include "cu_tenv.h"

typedef struct TestFixture_ {
	const char* name;
	CU_List* testCases;
} TestFixture;


void cuTest_addTestCase(TestFixture* fixture, const char *name, void (*testFunc)(TestEnvironment* environment));

void cuTest_init(TestFixture* fixture, const char* name);

void cuTest_execute(TestFixture* fixture);

int cuTest_report(TestFixture* fixture);

void cuTest_destroy(TestFixture* fixture);

#endif