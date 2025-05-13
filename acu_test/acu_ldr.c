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
#include "acu_rprt.h"

static int checkVersion(ACU_Entry* entry) {
    ACU_Version* libVersion = entry->getAcuTestVersion();
    ACU_Version* version = acu_getVersion();
    if (!acu_compareVersion(version, libVersion)) {
        char libversionStr[8];
        char versionStr[8];
        acu_formatVersion(libversionStr, sizeof libversionStr, libVersion);
        acu_formatVersion(versionStr, sizeof versionStr, version);
        acu_eprintf("Version of acu library %s does not match to runner %s", libversionStr, versionStr);
        return 0;
    }
    return 1;
}

ACU_Entry* acu_mallocEntry(void) {
    return acu_emalloc(sizeof(ACU_Entry));
}

void acu_initEntry(ACU_Entry* entry, ACU_Fixture* fixture) {
    entry->fixture = fixture;
    entry->getAcuTestVersion = acu_getVersion;
    entry->setWriteHandler = acu_setWriteHandler;
    entry->setFrameStack = acu_setFrameStack;
    entry->setAllocTable = acu_setAllocTable;
}

void acu_destroyEntry(ACU_Entry* entry) {
    if (entry) {
        acu_fixtureDestroy((ACU_Fixture*) entry->fixture);
    }
}

struct ACU_Progress_;

void acu_executeEntry(const ACU_Entry* entry, struct ACU_Progress_* progress)
{
    if (entry) {
        entry->setWriteHandler(acu_getWriteHandler());
        entry->setAllocTable(acu_getAllocTable());
        acu_executeFixture(entry->fixture, progress);
    }
}

extern void initFunc(ACU_Plugin* plugin, void* initContext);

ACU_Entry* cup_load(const char* cu_name) {
    ACU_PluginContext pluginContext = { NULL };
    ACU_Plugin* plugin = acu_pluginMalloc();
    acu_pluginLoad(plugin, cu_name);
    if (!plugin->pluginCode) {
        acu_free(plugin);
        return NULL;
    }
    acu_pluginInit(plugin, initFunc, &pluginContext);
    if (!checkVersion(pluginContext.entry)) {
        cup_unload(pluginContext.entry);
        return NULL;
    };
    return pluginContext.entry;
}

void cup_unload(ACU_Entry* entry) {
    acu_pluginUnload(entry->plugin);
    acu_free(entry->plugin);
    acu_destroyEntry(entry);
    acu_free(entry);
}
