#pragma once
#ifndef _ACU_TEST_CASE_H_
#define _ACU_TEST_CASE_H_

#include <acu_eenv.h> 
#include <acu_rslt.h>

struct ACU_Fixture_;

typedef struct ACU_TestCase_ {
    char* name;
    void (*testFunc)(ACU_ExecuteEnv* environment, const void* context);
    ACU_Result* result;
    struct ACU_Fixture_* fixture;
} ACU_TestCase;

#endif
