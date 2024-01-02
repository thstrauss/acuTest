#pragma once
#ifndef _CU_TEST_CASE_H
#define _CU_TEST_CASE_H

#include "cu_rslt.h"
#include "cu_eenv.h"

typedef struct CU_TestCase_ {
	char* name;
	void (*testFunc)(CU_ExecuteEnv* environment);
	CU_Result* result;
} CU_TestCase;

#endif
