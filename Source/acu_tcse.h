#pragma once
#ifndef _ACU_TEST_CASE_H_
#define _ACU_TEST_CASE_H_

#include "acu_rslt.h"
#include "acu_eenv.h"

typedef struct ACU_TestCase_ {
    char* name;
    void (*testFunc)(ACU_ExecuteEnv* environment, const void* context);
    ACU_Result* result;
} ACU_TestCase;

#endif
