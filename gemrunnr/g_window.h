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

#ifndef __G_WINDOW__
#define __G_WINDOW__

#include "g_cellsze.h"
#include <aes.h>

typedef struct WinData_ {
    int windowHandle;
    int applId;
    int grafHandle;

    CellSize cellSize;

    void* viewModel;
    void (*drawViewModel)(const struct WinData_* winData, const void* testModel, const GRECT* clippingRect, const GRECT* workingRect);
} WinData;

typedef void GEM_DrawViewModelFunc(const WinData* winData, const void* testModel, const GRECT* clippingRect, const GRECT* workingRect);

void gem_initWinData(WinData* winData);

void gem_setViewModel(const WinData* wd, const void* viewModel);
void gem_setDrawViewModelFunc(const WinData* wd, GEM_DrawViewModelFunc* drawViewModelFunc);

void* gem_getViewModel(const WinData* wd);

 #endif