#pragma once
#ifndef _ACU_TEST_CASE_H_
#define _ACU_TEST_CASE_H_

#include <acu_eenv.h> 

struct ACU_Fixture_;
struct ACU_Result_;

typedef void ACU_TestFunc(ACU_ExecuteEnv* environment, const void* context);

typedef struct ACU_TestCase_ {
    char* name;
    ACU_TestFunc* testFunc;
    struct ACU_Result_* result;
    struct ACU_Fixture_* fixture;
} ACU_TestCase;

#endif
