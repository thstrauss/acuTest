#pragma once
#ifndef _CU_UTILS_H_
#define _CU_UTILS_H_

void acu_eprintf(const char* format, ...);
void acu_weprintf(const char* format, ...);
char* acu_estrdup(const char* s);
void* acu_emalloc(size_t n);
char* acu_progName();
void acu_setProgName(const char* progName);

#endif