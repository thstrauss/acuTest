#pragma once
#ifndef _CU_TEST_ENV_H_
#define _CU_TEST_ENV_H_

#include "cu_trslt.h"
#include <setjmp.h>

typedef struct CU_ExecuteEnv_ {
	jmp_buf assertBuf;
	CU_Result* result;
} CU_ExecuteEnv;

#endif
