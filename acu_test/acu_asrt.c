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

int acu_equalPtr(const ACU_AssertParameter* parameter) {
    return *(void**) parameter->actual == *(void**) parameter->expected;
}

void acu_equalPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%actual value %p not equal to expected value %p: %s", *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
}

int acu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return *(void**)parameter->actual != *(void**)parameter->expected;
}

void acu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "actual value %p equal to expected value %p: %s", *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
}

int acu_equalStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void acu_equalStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "actual value \"%s\" not equal to expected value \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

int acu_notEqualStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void acu_notEqualStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "actual value \"%s\" equal to expected value \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

void acu_assert(ACU_ExecuteEnv* environment, int(*assertFunc)(const ACU_AssertParameter* parameter), void(*formatMessage)(char* buffer, int bufferSize, const ACU_AssertParameter* parameter), const ACU_AssertParameter* parameter) {
    const int bufferSize = 1024;
    char* buffer = acu_emalloc(bufferSize);
    int assertResult = assertFunc(parameter);

    TRY_CTX(acu_assert)
        acu_sprintf_s(buffer, bufferSize, "");
        if (!assertResult) {
            if (formatMessage != NULL) {
                formatMessage(buffer, bufferSize, parameter);
            }
        }
        environment->result->message = acu_estrdup(buffer);
        if (!assertResult) {
            environment->result->status = ACU_TEST_FAILED;
            longjmp(environment->assertBuf, ACU_TEST_FAILED);
        }
    FINALLY
        free(buffer);
    ETRY;
}
