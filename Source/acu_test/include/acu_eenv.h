#pragma once
#ifndef _ACU_TEST_ENV_H_
#define _ACU_TEST_ENV_H_

#include <setjmp.h>

struct ACU_Result_;

typedef struct ACU_ExecuteEnv_ {
    jmp_buf assertBuf;
    struct ACU_Result_* result;
} ACU_ExecuteEnv;

#endif
