#include <string.h>
#include <stdio.h>

#include "cu_assrt.h"
#include "cu_eenv.h"
#include "cu_cmmn.h"

int cuTest_equalInt(const AssertParameter* parameter) {
    return *(const int*)parameter->actual == *(const int*)parameter->expected;
}

void cuTest_equalIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d not equal to expected value %d: %s", parameter->fileName, parameter->line, *(const int*)parameter->actual, *(const int*)parameter->expected, parameter->message);
}

int cuTest_notEqualInt(const AssertParameter* parameter) {
    return *(const int*)parameter->actual != *(const int*)parameter->expected;
}

void cuTest_notEqualIntFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %d equal to expected value %d: %s", parameter->fileName, parameter->line, *(const int*)parameter->actual, *(const int*)parameter->expected, parameter->message);
}

int cuTest_equalPtr(const AssertParameter* parameter) {
    return parameter->actual == parameter->expected;
}

void cuTest_equalPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_notEqualPtr(const AssertParameter* parameter) {
    return parameter->actual != parameter->expected;
}

void cuTest_notEqualPtrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, parameter->actual, parameter->expected, parameter->message);
}

int cuTest_equalStr(const AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void cuTest_equalStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

int cuTest_notEqualStr(const AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void cuTest_notEqualStrFormatMessage(char* buffer, int bufferSize, const AssertParameter* parameter) {
    sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

static void cuTest_assertCore(CU_ExecuteEnv* environment, int conditionResult, const char* message) {
    environment->result->message = cu_estrdup(message);
    if (!conditionResult) {
        environment->result->status = CU_TEST_FAILED;
        longjmp(environment->assertBuf, CU_TEST_FAILED);
    }
};

void cuTest_assert(CU_ExecuteEnv* environment, int(*assertFunc)(const AssertParameter* parameter), void(*formatMessage)(char* buffer, int bufferSize, const AssertParameter* parameter), const AssertParameter* parameter) {
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
        cuTest_assertCore(environment, assertResult, buffer);
    FINALLY
        free(buffer);
    ETRY;
}
