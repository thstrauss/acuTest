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

#include "acu_fxtr.h"
#include "acu_vers.h"
#include <acu_plgn.h>
#include <acu_util.h>
#include "acu_tryc.h"

/*
The ACU_Entry describes the acuTest plug-in.
*/
typedef struct ACU_Entry_ {
    ACU_Fixture* fixture;
    ACU_getVersionFunc* getAcuTestVersion;
    ACU_setWriteHandlerFunc* setWriteHandler;
    ACU_setFrameStackFunc* setFrameStack;
    ACU_Plugin* plugin;
} ACU_Entry;

typedef struct ACU_PluginContext_ {
    ACU_Entry* entry;
} ACU_PluginContext;

typedef ACU_Entry* (ACU_initFunc)(void);

/*
Defines the entry point function for the plug-in.
*/
typedef ACU_Entry* ACU_init(void);

/*
Declares the entry point for the plug-in.
*/
__EXPORT ACU_Entry* acu_init(void);

__EXPORT ACU_Entry* cup_load(const char* cu_name);
__EXPORT void cup_unload(ACU_Entry* entry);

__EXPORT ACU_Entry* acu_entryMalloc(void);
__EXPORT void acu_entryInit(ACU_Entry* entry, ACU_Fixture* fixture);
__EXPORT void acu_entryDestroy(ACU_Entry* entry);

/*
Executes the tests in the plug-in.
*/
__EXPORT void acu_entryExecute(const ACU_Entry* entry, ACU_Progress* progress);
 
#endif
