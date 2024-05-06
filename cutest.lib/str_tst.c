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
#include <acu_tryc.h>
 
static void strEqualBothNull(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strEqual(testEnvironment, NULL, NULL, "strEqualBothNull"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "strEqualBothNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_equalStr", "strEqualBothNull"); \
    FINALLY
        if (resultBuf->message) {
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
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strEqual(testEnvironment, NULL, "abc", "strEqualActualNull"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "strEqualActualNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_equalStr", "strEqualActualNull"); \
        FINALLY
        if (resultBuf->message) {
            free(resultBuf->message);
        }
        free(resultBuf);
        free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strIsEmpty(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strIsEmpty(testEnvironment, "", "strIsEmpty"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strIsEmpty"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strIsEmpty"); \
        FINALLY
        if (resultBuf->message) {
            free(resultBuf->message);
        }
    free(resultBuf);
    free(testEnvironment);
    ETRY;
    UNUSED(context);
}


static void strIsNotEmpty(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strIsNotEmpty(testEnvironment, "abc", "strIsNotEmpty"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strIsNotEmpty"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strIsNotEmpty"); \
        FINALLY
        if (resultBuf->message) {
            free(resultBuf->message);
        }
    free(resultBuf);
    free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strIsNotEmptyFails(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strIsNotEmpty(testEnvironment, "", "strIsNotEmptyFails"); \
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_FAILED, "strIsNotEmptyFails"); \
        ACU_assert_ptrIsNotNull(environment, testEnvironment->result->message, "strIsNotEmptyFails"); \
        FINALLY
        if (resultBuf->message) {
            free(resultBuf->message);
        }
    free(resultBuf);
    free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strContains(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strContains(testEnvironment, "qwertyabc", "abc", "strContains"); 
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strContains"); 
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strContains"); 
    FINALLY
        if (resultBuf->message) {
            free(resultBuf->message);
        }
    free(resultBuf);
    free(testEnvironment);
    ETRY;
    UNUSED(context);
}

static void strNotContains(ACU_ExecuteEnv* environment, const void* context) {
    ACU_ExecuteEnv* testEnvironment = acu_emalloc(sizeof(ACU_ExecuteEnv));
    ACU_Result* resultBuf = (ACU_Result*)acu_emalloc(sizeof(ACU_Result));
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL;

    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strNotContains(testEnvironment, "qwertyabc", "abcd", "strNotContains");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strNotContains");
    ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strNotContains");
    FINALLY
        if (resultBuf->message) {
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
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;
    testEnvironment->result = resultBuf;

    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strEqual(testEnvironment, "abc", "abc", "strEqual");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strEqual"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strEqual"); \
    FINALLY
        if (resultBuf->message) {
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
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        long i;
        for (i = 0; i < 10000L; i++) {
        	ACU_assert_strNotEqual(testEnvironment, "ptr1", "ptr2", "strNotEqual");
    	}
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_PASSED, "strNotEqual"); \
        ACU_assert_ptrEqual(environment, testEnvironment->result->message, NULL, "strNotEqual"); \
    FINALLY
        if (resultBuf->message) {
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
    ACU_Stack* frameStack = acu_getFrameStack();
    ACU_Frame frame;
    acu_stackPush(frameStack, &frame);
    testEnvironment->exceptionFrame = &frame;

    testEnvironment->result = resultBuf;
    resultBuf->status = ACU_TEST_PASSED;
    resultBuf->message = NULL;
    resultBuf->file = NULL; 
    
    if (!setjmp(testEnvironment->exceptionFrame->exceptionBuf)) {
        ACU_assert_strNotEqual(testEnvironment, NULL, NULL, "ptrNotEqualNull");
    }
    TRY
        ACU_assert(environment, int, Equal, testEnvironment->result->status, ACU_TEST_ERROR, "strNotEqualNull"); \
        ACU_assert_strEqual(environment, testEnvironment->result->message, "Error in: acu_notEqualStr", "strNotEqualNull"); \
    FINALLY
        if (resultBuf->message) {
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
    
    acu_fixtureAddTestCase(fixture, "str Not Empty Fails", strIsNotEmptyFails);
    acu_fixtureAddTestCase(fixture, "str Not Empty", strIsNotEmpty);
    acu_fixtureAddTestCase(fixture, "str Empty", strIsEmpty);
    acu_fixtureAddTestCase(fixture, "str Not Contains", strNotContains);
    acu_fixtureAddTestCase(fixture, "str Contains", strContains);
    acu_fixtureAddTestCase(fixture, "str Equal both NULL", strEqualBothNull);
    acu_fixtureAddTestCase(fixture, "str Equal actual NULL", strEqualActualNull);
    acu_fixtureAddTestCase(fixture, "str Equal", strEqual);
    acu_fixtureAddTestCase(fixture, "str Not Equal", strNotEqual);
    acu_fixtureAddTestCase(fixture, "str Not Equal NULL", strNotEqualNull);

    return fixture;
}