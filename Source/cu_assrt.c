#include <string.h>
#include <stdio.h>

#define __ACU_EMIT_ASSERT_FUNCS__
#include "cu_assrt.h"
#undef __ACU_EMIT_ASSERT_FUNCS__
#include "cu_eenv.h"
#include "cu_cmmn.h"

int cu_equalPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual == parameter->expected;
}

void cu_equalPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return parameter->actual != parameter->expected;
}

void cu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cu_equalStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void cu_equalStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

int cu_notEqualStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void cu_notEqualStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

void cu_assert(ACU_ExecuteEnv* environment, int(*assertFunc)(const ACU_AssertParameter* parameter), void(*formatMessage)(char* buffer, int bufferSize, const ACU_AssertParameter* parameter), const ACU_AssertParameter* parameter) {
    const int bufferSize = 1024;
    char* buffer = cu_emalloc(bufferSize);
    int assertResult = assertFunc(parameter);

    TRY
        sprintf_s(buffer, bufferSize, "");
        if (!assertResult) {
            if (formatMessage != NULL) {
                formatMessage(buffer, bufferSize, parameter);
            }
        }
        environment->result->message = cu_estrdup(buffer);
        if (!assertResult) {
            environment->result->status = CU_TEST_FAILED;
            longjmp(environment->assertBuf, CU_TEST_FAILED);
        }
    FINALLY
        free(buffer);
    ETRY;
}
