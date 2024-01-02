#pragma once
#ifndef _CU_TEST_RESULT_H_
#define _CU_TEST_RESULT_H_

typedef struct TestResult_ {
	int status;
	char* message;
} TestResult;

#endif