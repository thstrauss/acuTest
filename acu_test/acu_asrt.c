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

#include <string.h>
#include <stdlib.h>

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_util.h>
#include <acu_rslt.h>

#define __ACU_EMIT_ASSERT_FUNCS__
#include <acu_asrt.h>
#undef __ACU_EMIT_ASSERT_FUNCS__

enum ACU_TestResult acu_equalPtr(const ACU_AssertParameter* parameter) {
    return *(void**) parameter->actual == *(void**) parameter->expected;
}

void acu_equalPtrFormatMessage(char* buffer, int bufferSize, enum ACU_TestResult result, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%actual value %p not equal to %p: %s", *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
    UNUSED(result);
}

enum ACU_TestResult acu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return *(void**)parameter->actual != *(void**)parameter->expected;
}

void acu_notEqualPtrFormatMessage(char* buffer, int bufferSize, enum ACU_TestResult result, const ACU_AssertParameter* parameter) {
    UNUSED(result);
    acu_sprintf_s(buffer, bufferSize, "actual value %p equal to %p: %s", *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
}

enum ACU_TestResult acu_equalStr(const ACU_AssertParameter* parameter) {
    if (parameter->actual == NULL || parameter->expected == NULL) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void acu_equalStrFormatMessage(char* buffer, int bufferSize, enum ACU_TestResult result, const ACU_AssertParameter* parameter) {
    if (result == ACU_TEST_FAILED) {
        acu_sprintf_s(buffer, bufferSize, "actual value \"%s\" not equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    }
    else if (result == ACU_TEST_ERROR) {
        acu_sprintf_s(buffer, bufferSize, "Error in: %s", "acu_equalStr");
    }
}

enum ACU_TestResult acu_notEqualStr(const ACU_AssertParameter* parameter) {
    if (parameter->actual == NULL || parameter->expected == NULL) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void acu_notEqualStrFormatMessage(char* buffer, int bufferSize, enum ACU_TestResult result, const ACU_AssertParameter* parameter) {
    if (result == ACU_TEST_FAILED) {
        acu_sprintf_s(buffer, bufferSize, "actual value \"%s\" equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    }
    else if (result == ACU_TEST_ERROR) {
        acu_sprintf_s(buffer, bufferSize, "Error in: %s", "acu_notEqualStr");
    }
}

void acu_assert(ACU_ExecuteEnv* environment,
    assertFunc assert,
    formatMessageFunc formatMessage,
    const ACU_AssertParameter* parameter) {
    enum ACU_TestResult assertResult = assert(parameter);

    char* buffer;
    if (assertResult != ACU_TEST_PASSED && formatMessage) {
        buffer = acu_emalloc(1024);
        formatMessage(buffer, 1024, assertResult, parameter);
    }
    else {
        buffer = NULL;
    }
    environment->result->message = buffer;
    environment->result->status = assertResult;
    if (assertResult != ACU_TEST_PASSED) {
        longjmp(environment->assertBuf, ACU_TEST_ABORTED);
    }
}
