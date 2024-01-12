#include <string.h>
#include <stdio.h>

#define __emit 1
#include "cu_assrt.h"
#undef __emit
#include "cu_eenv.h"
#include "cu_cmmn.h"

int cu_equalPtr(const AssertParameter* parameter) {
    return parameter->actual == parameter->expected;
}

void cu_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cu_notEqualPtr(const AssertParameter* parameter) {
    return parameter->actual != parameter->expected;
}

void cu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cu_equalStr(const AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void cu_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

int cu_notEqualStr(const AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void cu_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

void cu_assert(CU_ExecuteEnv* environment, int(*assertFunc)(const AssertParameter* parameter), void(*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter), const AssertParameter* parameter) {
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
