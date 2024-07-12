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

#include <stdio.h>
#include <stdlib.h>

#include "acu_ldr.h"
#include "acu_util.h"
#include "acu_vers.h"
#include "acu_tryc.h"
#include "acu_plgn.h"

static int checkVersion(ACU_Entry* entry) {
    ACU_Version* libVersion = entry->getAcuTestVersion();
    ACU_Version* version = acu_getVersion();
    if (!acu_compareVersion(version, libVersion)) {
        char* libversionStr = acu_formatVersion(libVersion);
        char* versionStr = acu_formatVersion(version);
        acu_eprintf("Version of acu library %s does not match to runner %s", libversionStr, versionStr);
        free(versionStr);
        free(libversionStr);
        return 0;
    }
    return 1;
}

#ifdef __TOS__

#include <string.h>
#include <stdlib.h>

#include "acu_tryc.h"
#include "acu_stck.h"

ACU_exitFunc* exitFunc;

static void setExit(ACU_exitFunc* exit) {
	exitFunc = exit;
}

ACU_Entry* cup_load(const char* cu_name) {
	ACU_Plugin* plugin = acu_pluginMalloc();
	ACU_init* init;
    ACU_Entry* entry;
    
	acu_pluginLoad(plugin, cu_name);
    init = (ACU_init*) plugin->pluginCode;
    
    if (!init) {
        return NULL;
    }
    entry = init();
    entry->plugin = plugin;
    entry->setFrameStack(acu_getFrameStack());
    entry->setExit(exit);
    
    if (!checkVersion(entry)) {
        cup_unload(entry);
        return NULL;
    };
    return entry;
}

void cup_unload(ACU_Entry* entry) {
    acu_pluginUnload(entry->plugin);
    acu_entryDestroy(entry);
}

#else

#include <windows.h>

ACU_Entry* cup_load(const char* cu_name) {
    ACU_Plugin* plugin = acu_pluginMalloc();
    acu_pluginLoad(plugin, cu_name);
    if (!plugin->plugin) {
        free(plugin);
        return NULL;
    }
    ACU_init* init = (ACU_init*) GetProcAddress(plugin->plugin, "acu_init");
    ACU_Entry* entry = init();
    entry->plugin = plugin;
    if (!checkVersion(entry)) {
        cup_unload(entry);
        return NULL;
    };
    return entry;
}

void cup_unload(ACU_Entry* entry) {
    acu_pluginUnload(entry->plugin);
    acu_entryDestroy(entry);
}

#endif

ACU_Entry* acu_entryMalloc(void) {
    return acu_emalloc(sizeof(ACU_Entry));
}

void acu_entryInit(ACU_Entry* entry, ACU_Suite* suite) {
    entry->suite = suite;
    entry->getAcuTestVersion = acu_getVersion;
#ifdef __TOS__
    entry->setWriteHandler = acu_setWriteHandler;
    entry->setFrameStack = acu_setFrameStack;
    entry->setExit = setExit;
#endif
}

void acu_entryDestroy(ACU_Entry* entry) {
    if (entry) {
        acu_suiteDestroy((ACU_Suite*)entry->suite);
        free(entry->plugin);
        free(entry);
    }
}

void acu_entryExecute(const ACU_Entry* entry, ACU_Progress* progress)
{
    if (entry) {
#ifdef __TOS__
        entry->setWriteHandler(acu_getWriteHandler());
#endif
        acu_suiteExecute(entry->suite, progress);
    }
}
