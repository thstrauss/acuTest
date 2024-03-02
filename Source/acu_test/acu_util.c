#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <acu_util.h>

static char* programName = NULL;

typedef enum ACU_Level_ 
{
    acu_error = 1,
    acu_warning = 2,
} ACU_Level;

char* acu_progName(void) {
    return programName;
}

char* acu_estrdup(const char* s) {
    char* temp = _strdup(s);
    if (temp == NULL) {
        acu_eprintf("acu_estrdup(\"%.20s\") failed:", s);
    }
    return temp;
}

void acu_setProgName(const char* progName) {
    if (programName != NULL) {
        free(programName);
    }
    if (progName != NULL) {
        programName = acu_estrdup(progName);
    }
    else {
        programName = NULL;
    }
}

static void acu_printf(ACU_Level level, const char* format, ...) {
    va_list args;

    fflush(stdout);
    if (acu_progName() != NULL) {
        fprintf(stderr, "%s: ", acu_progName());
    }
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    if (format[0] != '\0' && format[strlen(format) - 1] == ':') {
        char buffer[50];
        strerror_s(buffer, 50, errno);
        fprintf(stderr, " %s", buffer);
    }
    fprintf(stderr, "\r\n");
    if (level == acu_error) {
        exit(2);
    }
}

void acu_eprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    acu_printf(acu_error, format, args);
    va_end(args);
}

void acu_weprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    acu_printf(acu_warning, format, args);
    va_end(args);
}

void* acu_emalloc(size_t size) {
    void* p;
    p = malloc(size);
    if (p == NULL) {
        acu_eprintf("acu_emalloc of %u bytes failed:", size);
    }
    return p;
}