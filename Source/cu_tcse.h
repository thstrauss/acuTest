#pragma once
#ifndef _CU_TEST_CASE_H_
#define _CU_TEST_CASE_H_

#include "cu_rslt.h"
#include "cu_eenv.h"

typedef struct CU_TestCase_ {
    char* name;
    void (*testFunc)(ACU_ExecuteEnv* environment, const void* context);
    ACU_Result* result;
} CU_TestCase;

#endif
