#pragma once
#ifndef _ACU_TEST_ENV_H_
#define _ACU_TEST_ENV_H_

#include <setjmp.h>

#include "acu_rslt.h"

typedef struct ACU_ExecuteEnv_ {
    jmp_buf assertBuf;
    ACU_Result* result;
} ACU_ExecuteEnv;

#endif
