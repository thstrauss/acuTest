#pragma once
#ifndef _CU_TEST_CASE_H_
#define _CU_TEST_CASE_H_

#include "cu_rslt.h"
#include "cu_eenv.h"

typedef struct ACU_TestCase_ {
    char* name;
    void (*testFunc)(ACU_ExecuteEnv* environment, const void* context);
    ACU_Result* result;
} ACU_TestCase;

#endif
