#pragma once
#ifndef _CU_TEST_ENV_H_
#define _CU_TEST_ENV_H_

#include <setjmp.h>

#include "cu_rslt.h"

typedef struct CU_ExecuteEnv_ {
	jmp_buf assertBuf;
	CU_Result* result;
} CU_ExecuteEnv;

#endif
