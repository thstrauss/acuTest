#pragma once
#ifndef _CU_TEST_ENV_H_
#define _CU_TEST_ENV_H_

#include "cu_trslt.h"
#include <setjmp.h>

typedef struct TestEnvironment_ {
	jmp_buf assertBuf;
	TestResult* result;
} ExecuteEnv;

#endif
