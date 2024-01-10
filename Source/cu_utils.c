#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "cu_utils.h"

static char* name = NULL;

typedef enum CU_Level_ 
{
    cu_error = 1,
    cu_warning = 2,
} CU_Level;

char* cu_progName() {
    return name;
}

char* cu_estrdup(const char* s) {
    char* temp = _strdup(s);
    if (temp == NULL) {
        cu_eprintf("cu_estrdup(\"%.20s\") failed:", s);
        return NULL;
    }
    return temp;
}

void cu_setProgName(const char* progName) {
    if (name != NULL) {
        free(name);
    }
    if (progName != NULL) {
        name = cu_estrdup(progName);
    }
    else {
        name = NULL;
    }
}

static void cu_printf(CU_Level level, const char* format, ...) {
    va_list args;

    fflush(stdout);
    if (cu_progName() != NULL) {
        fprintf(stderr, "%s: ", cu_progName());
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
    if (level == cu_error) {
        exit(2);
    }
}

void cu_eprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    cu_printf(cu_error, format, args);
    va_end(args);
}

void cu_weprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    cu_printf(cu_warning, format, args);
    va_end(args);
}



void* cu_emalloc(size_t size) {
    void* p;
    p = malloc(size);
    if (p == NULL) {
        cu_eprintf("malloc of %u bytes failed:", size);
        return NULL;
    }
    return p;
}