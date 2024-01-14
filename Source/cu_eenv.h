#pragma once
#ifndef _CU_TEST_ENV_H_
#define _CU_TEST_ENV_H_

#include <setjmp.h>

#include "cu_rslt.h"

typedef struct ACU_ExecuteEnv_ {
    jmp_buf assertBuf;
    ACU_Result* result;
} ACU_ExecuteEnv;

#endif
