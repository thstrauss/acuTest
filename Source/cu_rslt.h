#pragma once
#ifndef _CU_TEST_RESULT_H_
#define _CU_TEST_RESULT_H_

typedef struct CU_Result_ {
    int status;
    char* message;
} CU_Result;

#endif