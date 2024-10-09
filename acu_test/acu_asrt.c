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
#include <string.h>
#include <stddef.h>
#include <stdarg.h>

#include "acu_cmmn.h"
#include "acu_eenv.h"
#include "acu_util.h"
#include "acu_rslt.h"
#include "acu_tryc.h"
#include "acu_stck.h"

static const maxPtrLength = 16;

static size_t acu_estimateBufferLength(const char* format, va_list args) {
#ifdef __TOS__
	UNUSED(format);
	UNUSED(args);
	return 512;
#else
    size_t formatLength = strlen(format);
    size_t bufferSize = formatLength;
    char* formatPtr = (char*) format;

    for (; *formatPtr != '\0'; formatPtr++) {
        if (*formatPtr == '%') {
            formatPtr++;
            if (*formatPtr == 's' && *formatPtr != '\0') {
                char* arg = (char*)va_arg(args, char*);
                bufferSize += strlen(arg);
            } else if (*formatPtr != '%' && *formatPtr != '\0') {
                void* temp = va_arg(args, void*);
                bufferSize += 32;
                UNUSED(temp);
            }
        }
    }
    return bufferSize;
#endif
}

static char* acu_sFormatMessage(const char* format, ...)
{
    char* buffer;
    size_t bufferSize;
    va_list args;

    va_start(args, format);

    bufferSize = acu_estimateBufferLength(format, args);

    va_end(args);

    buffer = acu_emalloc(bufferSize);

    if (buffer) {
        va_start(args, format);
        acu_vsprintf_s(buffer, bufferSize, format, args);
        va_end(args);
    }
    return buffer;
}

static char* acu_formatErrorMessage(const char* message, const char* parameterMessage) {
    static char* messageFormat = "Error in: %s: %s";
    size_t bufferSize = strlen(messageFormat) + strlen(message) + strlen(parameterMessage);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, message, parameterMessage);
    }
    return buffer;
}

#define STR(str) #str
#define CREATE_ASSERT_FUNC(type, op, opcode, format) \
static enum ACU_TestResult acu_##type##op(const ACU_Types* actual, const ACU_Types* expected) { \
    return actual->type##Type opcode expected->type##Type; \
} \
static char* acu_##type##op##FormatMessage(const ACU_AssertParameter* parameter) { \
    return acu_sFormatMessage(STR(actual value format not opcode to format: %s), parameter->values.actual.type##Type, parameter->values.expected.type##Type, parameter->message); \
} \
__EXPORT const ACU_Funcs acu_##type##op##Funcs = {acu_##type##op, acu_##type##op##FormatMessage, NULL};

#define __ACU_EMIT_ASSERT_FUNCS__
#include "acu_asrt.h"
#undef __ACU_EMIT_ASSERT_FUNCS__

static enum ACU_TestResult acu_equalPtr(const ACU_Types* actual, const ACU_Types* expected) {
    return actual->voidPtrType == expected->voidPtrType;
}

static char* acu_equalPtrFailedFormatMessage(const ACU_AssertParameter* parameter) {

    static char* messageFormat = "actual value 0X%p equal to 0X%p: %s";
    size_t bufferSize = strlen(messageFormat) + 2 * maxPtrLength + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, parameter->values.actual.voidPtrType, parameter->values.expected.voidPtrType, parameter->message);
    }
    return buffer;
}

__EXPORT const ACU_Funcs acu_equalPtrFuncs = { acu_equalPtr, acu_equalPtrFailedFormatMessage, NULL };

static enum ACU_TestResult acu_notEqualPtr(const ACU_Types* actual, const ACU_Types* expected) {
    return actual->voidPtrType != expected->voidPtrType;
}

static char* acu_notEqualPtrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    static char* messageFormat = "actual value 0X%p equal to 0X%p: %s";
    size_t bufferSize = strlen(messageFormat) + 2 * maxPtrLength + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, parameter->values.actual.voidPtrType, parameter->values.expected.voidPtrType, parameter->message);
    }
    return buffer;
}

__EXPORT const ACU_Funcs acu_notEqualPtrFuncs = { acu_notEqualPtr, acu_notEqualPtrFailedFormatMessage, NULL };

static enum ACU_TestResult acu_containsStr(const ACU_Types* actual, const ACU_Types* expected) {
    if (!actual->voidPtrType || !expected->voidPtrType) {
        return ACU_TEST_ERROR;
    }
    return (strstr(actual->charPtrType, expected->charPtrType) != NULL);
}

static char* acu_containsStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    static char* messageFormat = "actual value \"%s\" does not contain \"%s\": %s";
    const char* actual = parameter->values.actual.charPtrType;
    const char* expected = parameter->values.expected.charPtrType;
    size_t bufferSize = strlen(messageFormat) + strlen(actual) + strlen(expected) + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, actual, expected, parameter->message);
    }
    return buffer;
}

static char* acu_containsStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    static char* message = "acu_containsStr";
    return acu_formatErrorMessage(message, parameter->message);

}

__EXPORT const ACU_Funcs acu_containsStrFuncs = { acu_containsStr, acu_containsStrFailedFormatMessage, acu_containsStrErrorFormatMessage };

static enum ACU_TestResult acu_notContainsStr(const ACU_Types* actual, const ACU_Types* expected) {
    if (!actual->voidPtrType || !expected->voidPtrType) {
        return ACU_TEST_ERROR;
    }
    return (strstr((const char*)(actual->voidPtrType), (const char*)(expected->voidPtrType)) == NULL);
}

static char* acu_notContainsStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    static char* messageFormat = "actual value \"%s\" does contain \"%s\": %s";
    const char* actual = parameter->values.actual.charPtrType;
    const char* expected = parameter->values.expected.charPtrType;
    size_t bufferSize = strlen(messageFormat) + strlen(actual) + strlen(expected) + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, actual, expected, parameter->message);
    }
    return buffer;
}


static char* acu_notContainsStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    static char* message = "acu_notContainsStr";
    return acu_formatErrorMessage(message, parameter->message);
}

__EXPORT const ACU_Funcs acu_notContainsStrFuncs = { acu_notContainsStr, acu_notContainsStrFailedFormatMessage, acu_notContainsStrFailedFormatMessage };

static enum ACU_TestResult acu_equalStr(const ACU_Types* actual, const ACU_Types* expected) {
    if (!actual->voidPtrType || !expected->voidPtrType) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(actual->voidPtrType), (const char*)(expected->voidPtrType)) == 0;
}

static char* acu_equalStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    static char* messageFormat = "actual value \"%s\" not equal to \"%s\": %s";
    const char* actual = parameter->values.actual.charPtrType;
    const char* expected = parameter->values.expected.charPtrType;
    size_t bufferSize = strlen(messageFormat) + strlen(actual) + strlen(expected) + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, actual, expected, parameter->message);
    }
    return buffer;
}

static char* acu_equalStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    static char* message = "acu_equalStr";
    return acu_formatErrorMessage(message, parameter->message);
}

__EXPORT const ACU_Funcs acu_equalStrFuncs = { acu_equalStr, acu_equalStrFailedFormatMessage, acu_equalStrErrorFormatMessage };

static enum ACU_TestResult acu_notEqualStr(const ACU_Types* actual, const ACU_Types* expected) {
    if (!actual->voidPtrType || !expected->voidPtrType) {
        return ACU_TEST_ERROR;
    }
    return strcmp((const char*)(actual->voidPtrType), (const char*)(expected->voidPtrType)) != 0;
}

static char* acu_notEqualStrFailedFormatMessage(const ACU_AssertParameter* parameter) {
    static char* messageFormat = "actual value \"%s\" equal to \"%s\": %s";
    const char* actual = parameter->values.actual.charPtrType;
    const char* expected = parameter->values.expected.charPtrType;
    size_t bufferSize = strlen(messageFormat) + strlen(actual) + strlen(expected) + strlen(parameter->message);
    char* buffer = acu_emalloc(bufferSize);

    if (buffer) {
        acu_sprintf_s(buffer, bufferSize, messageFormat, (const char*)parameter->values.actual.voidPtrType, (const char*)parameter->values.expected.voidPtrType, parameter->message);
    }
    return buffer;
}

static char* acu_notEqualStrErrorFormatMessage(const ACU_AssertParameter* parameter) {
    static char* message = "acu_notEqualStr";
    return acu_formatErrorMessage(message, parameter->message);
}

__EXPORT const ACU_Funcs acu_notEqualStrFuncs = { acu_notEqualStr, acu_notEqualStrFailedFormatMessage, acu_notEqualStrErrorFormatMessage };

static char* acu_formatMessage(enum ACU_TestResult assertResult, const ACU_AssertParameter* parameter) {
    if (assertResult == ACU_TEST_FAILED) {
        return parameter->funcs->formatFailedMessage(parameter);
    } else if (assertResult == ACU_TEST_ERROR && parameter->funcs->formatErrorMessage) {
        return parameter->funcs->formatErrorMessage(parameter);
    } else {
       return NULL;
    }
}

static void acu_finalizeFailed(enum ACU_TestResult resultValue, ACU_Result* result, const ACU_AssertParameter* parameter) {
    result->message = acu_formatMessage(resultValue, parameter);
    result->sourceFileName = parameter->sourceFileName; 
    result->sourceLine = parameter->sourceLine;
}

void acu_assert(ACU_ExecuteEnv* environment, const ACU_AssertParameter* parameter) {
    register const ACU_Values* values;
    register enum ACU_TestResult assertResult;
    ACU_Result* result = environment->result;

    values = &(parameter->values);

    assertResult = parameter->funcs->assert(&values->actual, &values->expected);
    result->status = assertResult;
    if (assertResult != ACU_TEST_PASSED) {
        ACU_Frame* frame = acu_stackPeek(acu_getFrameStack());
        acu_finalizeFailed(assertResult, result, parameter);
        environment->exceptionFrame->exception = ACU_EXCEPTION_ABORTED;
        longjmp(frame->exceptionBuf, frame->exception != 0 ? frame->exception : 0xffff);
    }
}

void acu_assertFail(ACU_ExecuteEnv* environment, ACU_AssertParameter* parameter)
{
    ACU_Result* result = environment->result;
    ACU_Frame* frame = acu_stackPeek(acu_getFrameStack());

    result->status = ACU_TEST_FAILED;
    result->message = acu_sFormatMessage("Failed: %s", parameter->message);
    result->sourceFileName = parameter->sourceFileName;
    result->sourceLine = parameter->sourceLine;
    environment->exceptionFrame->exception = ACU_EXCEPTION_ABORTED;
    longjmp(frame->exceptionBuf, frame->exception != 0 ? frame->exception : 0xffff);
}
