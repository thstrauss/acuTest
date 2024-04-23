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

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>
 
static void strEqualBothNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->assertBuf)) {
        ACU_assert_strEqual(testEnvironment, NULL, NULL, "strEqualBothNull"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "strEqualBothNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_equalStr", "strEqualBothNull"); \
    FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
} 

static void strEqualActualNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->assertBuf)) {
        ACU_assert_strEqual(testEnvironment, NULL, "abc", "strEqualActualNull"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "strEqualActualNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_equalStr", "strEqualActualNull"); \
        FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strEqual(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->assertBuf)) {
        ACU_assert_strEqual(testEnvironment, "abc", "abc", "ptrEqual");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "ptrEqual"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "ptrEqual"); \
    FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strNotEqual(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->assertBuf)) {
        int i;
        for (i = 0; i < 10000; i++)
        ACU_assert_strNotEqual(testEnvironment, "ptr1", "ptr2", "strNotEqual");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strNotEqual"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strNotEqual"); \
    FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strNotEqualNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->assertBuf)) {
        ACU_assert_strNotEqual(testEnvironment, NULL, NULL, "ptrNotEqualNull");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "ptrNotEqualNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_notEqualStr", "ptrNotEqualNull"); \
    FINALLY
        if (resultBuf->message != NULL) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
}  
 
ACU_Fixture* strFixture(void)
{
    ACU_Fixture* fixture = acu_fixtureMalloc();

    acu_fixtureInit(fixture, "str tests");
    
    acu_fixtureAddTestCase(fixture, "str Equal both NULL", strEqualBothNull);
    acu_fixtureAddTestCase(fixture, "str Equal actual NULL", strEqualActualNull);
    acu_fixtureAddTestCase(fixture, "str Equal", strEqual);
    acu_fixtureAddTestCase(fixture, "str Not Equal", strNotEqual);
    acu_fixtureAddTestCase(fixture, "str Not Equal NULL", strNotEqualNull);

    return fixture;
}