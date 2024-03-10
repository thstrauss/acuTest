#pragma once
#ifndef _ACU_TEST_RESULT_H_
#define _ACU_TEST_RESULT_H_

typedef struct ACU_Result_ {
    int status;
    char* message;
    char* file;
    int line;
} ACU_Result;

#endif