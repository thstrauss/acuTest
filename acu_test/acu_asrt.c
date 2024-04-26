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

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_util.h>
#include <acu_rslt.h>

#define __ACU_EMIT_ASSERT_FUNCS__
#include <acu_asrt.h>
#undef __ACU_EMIT_ASSERT_FUNCS__

enum ACU_TestResult acu_equalPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual == parameter->expected;
}

char* acu_equalPtrFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(256);
    acu_sprintf_s(buffer, 256, "%actual value %p not equal to %p: %s", parameter->actual, parameter->expected, parameter->message);
    return buffer;
}

enum ACU_TestResult acu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual != parameter->expected;
}

char* acu_notEqualPtrFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(256);
    acu_sprintf_s(buffer, 256, "actual value %p equal to %p: %s", parameter->actual, parameter->expected, parameter->message);
    return buffer;
}

enum ACU_TestResult acu_equalStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

char* acu_equalStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(1024);
    acu_sprintf_s(buffer, 1024, "actual value \"%s\" not equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

char* acu_equalStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(256);
    acu_sprintf_s(buffer, 256, "Error in: %s", "acu_equalStr");
    UNUSED(parameter);
    return buffer;
}

enum ACU_TestResult acu_notEqualStr(const ACU_AssertParameter* parameter) {
    if (!parameter->actual || !parameter->expected) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

char* acu_notEqualStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(1024); 
    acu_sprintf_s(buffer, 1024, "actual value \"%s\" equal to \"%s\": %s", (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
    return buffer;
}

char* acu_notEqualStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    char* buffer = acu_emalloc(256); 
    acu_sprintf_s(buffer, 256, "Error in: %s", "acu_notEqualStr");
    UNUSED(parameter);
    return buffer;
}

static char* acu_formatMessage(enum ACU_TestResult assertResult, const ACU_AssertParameter* parameter) {
    if (assertResult == ACU_TEST_FAILED) {
        return parameter->formatFailedMessage(parameter);
    } else if (parameter->formatErrorMessage) {
        return parameter->formatErrorMessage(parameter);
    } else {
       return acu_estrdup("");
    }
}

void acu_assert(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) {
    enum ACU_TestResult assertResult = parameter->assert(parameter);

    environment->result->status = assertResult;
    if (assertResult != ACU_TEST_PASSED) {
        environment->result->message = acu_formatMessage(assertResult, parameter);
        longjmp(environment->assertBuf, ACU_TEST_ABORTED);
    }
}

char* acu_sformatMessage(const char* format, ...)
{
    char* buffer = acu_emalloc(256); 
    va_list args;
    va_start(args, format);
    acu_vsprintf_s(buffer, 256, format, args);
    va_end(args);
    return buffer; 
}
