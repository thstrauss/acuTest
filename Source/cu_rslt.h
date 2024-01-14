#pragma once
#ifndef _CU_TEST_RESULT_H_
#define _CU_TEST_RESULT_H_

typedef struct ACU_Result_ {
    int status;
    char* message;
} ACU_Result;

#endif