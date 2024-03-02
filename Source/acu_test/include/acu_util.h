#pragma once
#ifndef _ACU_UTILS_H_
#define _ACU_UTILS_H_

void acu_eprintf(const char* format, ...);
void acu_weprintf(const char* format, ...);
char* acu_estrdup(const char* s);
void* acu_emalloc(size_t n);
char* acu_progName(void);
void acu_setProgName(const char* progName);

#ifdef __TOS__
#define acu_sprintf_s(buffer, sizeOfBuffer, format, ...) sprintf(buffer, format, __VA_ARGS__) 
#else
#define acu_sprintf_s(buffer, sizeOfBuffer, format, ...) sprintf_s(buffer, sizeOfBuffer, format, __VA_ARGS__) 
#endif

#endif