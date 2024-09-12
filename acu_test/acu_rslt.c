/*
 * Copyright (c) 2024 Thomas Strauss
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall 
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stddef.h>
#include <stdlib.h>

#include "acu_cmmn.h"
#include "acu_rslt.h"
#include "acu_util.h"

enum ACU_TestResult acuTest_calcResult(enum ACU_TestResult aggregatedResult, enum ACU_TestResult result) {
    return result == ACU_TEST_PASSED ? aggregatedResult : result == ACU_TEST_FAILED ? ACU_TEST_FAILED : ACU_TEST_ERROR;
}

void acuTest_resultDestroy(ACU_Result* result) {
    if (result->message) {
        free(result->message);
        result->message = NULL;
    }
}

void acuTest_resultPrepare(ACU_Result* result) {
    result->status = ACU_TEST_PASSED;
    if (result->message) {
        free(result->message);
        result->message = NULL;
    }
}

ACU_Result* acuTest_resultMalloc(void) {
    return (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
}

void acuTest_resultInit(ACU_Result* result) {
    result->status = ACU_TEST_PASSED;
    result->message = NULL;
}