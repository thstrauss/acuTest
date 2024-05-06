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

#include "acu_cmmn.h"
#include "acu_eenv.h"
#include "acu_util.h"
#include "acu_rslt.h"
#include "acu_tryc.h"
#include "acu_stck.h"

#define __ACU_EMIT_ASSERT_FUNCS__
#include "acu_asrt.h"
#undef __ACU_EMIT_ASSERT_FUNCS__

static enum ACU_TestResult acu_equalPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual == parameter->expected;
}

static char* acu_equalPtrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(128);
    acu_sprintf_s(buffer, 128, "actual value %p not equal to %p: %s", parameter->actual, parameter->expected, parameter->message);
    return buffer;
}

__EXPORT const ACU_Funcs acu_equalPtrFuncs = { acu_equalPtr, acu_equalPtrFailedFormatMessage, NULL };

static enum ACU_TestResult acu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual != parameter->expected;
}

static char* acu_notEqualPtrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(128);
    acu_sprintf_s(buffer, 128, "actual value %p equal to %p: %s", parameter->actual, parameter->expected, parameter->message);
    return buffer;
}

__EXPORT const ACU_Funcs acu_notEqualPtrFuncs = { acu_notEqualPtr, acu_notEqualPtrFailedFormatMessage, NULL };

static enum ACU_TestResult acu_containsStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return (strstr((const char*)(parameter->actual), (const char*)(parameter->expected)) != NULL);
}

static size_t acu_assertAllocBuffer(char** buffer, const ACU_AssertParameter* parameter) {
    size_t bufSize = 50 + strlen((const char*)parameter->actual) + strlen((const char*)parameter->expected) + strlen(parameter->message);
    *buffer = acu_emalloc(bufSize);
    return bufSize;
}

static char* acu_containsStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer;
    size_t bufSize = acu_assertAllocBuffer(&buffer, parameter);
    acu_sprintf_s(buffer, bufSize, "actual value \"%s\" does not contain \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

static char* acu_containsStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(50);
    acu_sprintf_s(buffer, 50, "Error in: %s", "acu_containsStr");
    UNUSED(parameter);
    return buffer;
}

__EXPORT const ACU_Funcs acu_containsStrFuncs = { acu_containsStr, acu_containsStrFailedFormatMessage, acu_containsStrErrorFormatMessage };

static enum ACU_TestResult acu_notContainsStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return (strstr((const char*)(parameter->actual), (const char*)(parameter->expected)) == NULL);
}

static char* acu_notContainsStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer;
    size_t bufSize = acu_assertAllocBuffer(&buffer, parameter);
    acu_sprintf_s(buffer, bufSize, "actual value \"%s\" does contain \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

static char* acu_notContainsStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(50);
    acu_sprintf_s(buffer, 50, "Error in: %s", "acu_notContainsStr");
    UNUSED(parameter);
    return buffer;
}

__EXPORT const ACU_Funcs acu_notContainsStrFuncs = { acu_notContainsStr, acu_notContainsStrFailedFormatMessage, acu_notContainsStrFailedFormatMessage };

static enum ACU_TestResult acu_equalStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

static char* acu_equalStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer;
    size_t bufSize = acu_assertAllocBuffer(&buffer, parameter);
    acu_sprintf_s(buffer, bufSize, "actual value \"%s\" not equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

static char* acu_equalStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(50);
    acu_sprintf_s(buffer, 50, "Error in: %s", "acu_equalStr");
    UNUSED(parameter);
    return buffer;
}

__EXPORT const ACU_Funcs acu_equalStrFuncs = { acu_equalStr, acu_equalStrFailedFormatMessage, acu_equalStrErrorFormatMessage };

static enum ACU_TestResult acu_notEqualStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

static char* acu_notEqualStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer;
    size_t bufSize = acu_assertAllocBuffer(&buffer, parameter);
    acu_sprintf_s(buffer, bufSize, "actual value \"%s\" equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

static char* acu_notEqualStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(50); 
    acu_sprintf_s(buffer, 50, "Error in: %s", "acu_notEqualStr");
    UNUSED(parameter);
    return buffer;
}

__EXPORT const ACU_Funcs acu_notEqualStrFuncs = { acu_notEqualStr, acu_notEqualStrFailedFormatMessage, acu_notEqualStrErrorFormatMessage };

char* acu_sformatMessage(const char* format, ...)
{
    char* buffer = acu_emalloc(256);
    va_list args;
    va_start(args, format);
    acu_vsprintf_s(buffer, 256, format, args);
    va_end(args);
    return buffer;
}

static char* acu_formatMessage(enum ACU_TestResult assertResult, const ACU_AssertParameter* parameter) {
    if (assertResult == ACU_TEST_FAILED) {
        return parameter->funcs->formatFailedMessage(parameter);
    } else if (parameter->funcs->formatErrorMessage) {
        return parameter->funcs->formatErrorMessage(parameter);
    } else {
       return acu_estrdup("");
    }
}

static void acu_finalizeFailed(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) {
    ACU_Frame* frame = acu_stackPeek(acu_getFrameStack());
    environment->result->message = acu_formatMessage(environment->result->status, parameter);
    environment->result->file = acu_estrdup(parameter->fileName); 
    environment->result->line = parameter->line;
    environment->exceptionFrame->exception = ACU_TEST_ABORTED;
    longjmp(frame->exceptionBuf, frame->exception != 0 ? frame->exception: 0xffff);
}

void acu_assert(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) {
    enum ACU_TestResult assertResult = parameter->funcs->assert(parameter);
    environment->result->status = assertResult;
    if (assertResult != ACU_TEST_PASSED) {
        acu_finalizeFailed(environment, parameter);
    }
}