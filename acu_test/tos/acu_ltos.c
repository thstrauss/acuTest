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

#include <acu_plgn.h>

#include "..\acu_ldr.h"

#include <string.h>
#include <stdlib.h>

#include "acu_tryc.h"
#include "acu_stck.h"

ACU_exitFunc* exitFunc;

static void setExit(ACU_exitFunc* exit) {
    exitFunc = exit;
}

void initFunc(ACU_Plugin* plugin, void* initContext) {
    ACU_PluginContext* pluginContext = initContext;
    ACU_init* init = (ACU_init*)plugin->pluginCode;

    if (!init) {
        return;
    }
    pluginContext->entry = init();
    pluginContext->entry->setWriteHandler = acu_setWriteHandler;
    pluginContext->entry->setFrameStack = acu_setFrameStack;
    pluginContext->entry->setExit = setExit;

    pluginContext->entry->setFrameStack(acu_getFrameStack());
    pluginContext->entry->setExit(exit);

    pluginContext->entry->plugin = plugin;
}

