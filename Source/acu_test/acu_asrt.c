#include <string.h>
#include <stdlib.h>

#define __ACU_EMIT_ASSERT_FUNCS__
#include <acu_asrt.h>
#undef __ACU_EMIT_ASSERT_FUNCS__

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_util.h>

int acu_equalPtr(const ACU_AssertParameter* parameter) {
    return *(void**) parameter->actual == *(void**) parameter->expected;
}

void acu_equalPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p not equal to expected value %p: %s", parameter->fileName, parameter->line, *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
}

int acu_notEqualPtr(const ACU_AssertParameter* parameter) {
    return *(void**)parameter->actual != *(void**)parameter->expected;
}

void acu_notEqualPtrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%s:%d -> actual value %p equal to expected value %p: %s", parameter->fileName, parameter->line, *(void**)parameter->actual, *(void**)parameter->expected, parameter->message);
}

int acu_equalStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) == 0;
}

void acu_equalStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" not equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
}

int acu_notEqualStr(const ACU_AssertParameter* parameter) {
    return strcmp((const char*)(parameter->actual), (const char*)(parameter->expected)) != 0;
}

void acu_notEqualStrFormatMessage(char* buffer, int bufferSize, const ACU_AssertParameter* parameter) {
    acu_sprintf_s(buffer, bufferSize, "%s:%d -> actual value \"%s\" equal to expected value \"%s\": %s", parameter->fileName, parameter->line, (const char*)parameter->actual, (const char*)parameter->expected, parameter->message);
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
