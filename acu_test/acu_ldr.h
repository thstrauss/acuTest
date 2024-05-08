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

#ifndef __ACU_LOADER__
#define __ACU_LOADER__

#include "acu_suit.h"
#include "acu_vers.h"

#ifdef __TOS__
#include "acu_tryc.h"
#include "acu_tryc.h"
#else
#include <windows.h>
#endif

typedef struct ACU_Entry_ {
    ACU_Suite* suite;
    ACU_getVersionFunc* getAcuTestVersion;
#ifdef __TOS__
    void* cup_code;
    ACU_setFrameStackFunc* setFrameStackFunc;
#else
    HMODULE module;
#endif
} ACU_Entry;

typedef __EXPORT  ACU_Entry* (ACU_initFunc)(void);

typedef ACU_Entry* ACU_init(void);

__EXPORT ACU_Entry* acu_init(void);

__EXPORT ACU_Entry* cup_load(const char* cu_name);
__EXPORT void cup_unload(ACU_Entry* entry);

__EXPORT ACU_Entry* acu_entryMalloc(void);
__EXPORT void acu_entryInit(ACU_Entry* entry, ACU_Suite* suite);
__EXPORT void acu_entryDestroy(ACU_Entry* entry);
 
#endif
