#pragma once
#ifndef _CU_UTILS_H_
#define _CU_UTILS_H_

void cu_eprintf(const char* format, ...);
void cu_weprintf(const char* format, ...);
char* cu_estrdup(const char* s);
void* cu_emalloc(size_t n);
char* cu_progName();
void cu_setProgName(const char* progName);

#endif