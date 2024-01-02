#pragma once
#ifndef _CU_TEST_CASE_H
#define _CU_TEST_CASE_H

#include "cu_trslt.h"
#include "cu_tenv.h"

typedef struct TestCase_ {
	char* name;
	void (*testFunc)(ExecuteEnv* environment);
	TestResult* result;
} TestCase;

#endif
