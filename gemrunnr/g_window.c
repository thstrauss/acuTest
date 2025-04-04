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

#include <stddef.h>
#include "g_window.h"
#include "g_slider.h"
#include "acu_cmmn.h"

void gem_initWinData(WinData* wd) {
    UNUSED(wd);
}

void gem_setViewModel(const WinData* wd, const void* viewModel) {
    wd->viewModel = viewModel;
}

void gem_setDrawViewModelFunc(const WinData* wd, GEM_DrawViewModelFunc* drawViewModel, GEM_VerticalUpdateSliderModelFunc* updateVerticalSlider, GEM_HorizontalUpdateSliderModelFunc* updateHorizontalSlider)
{
    wd->drawViewModel = drawViewModel;
    gem_initVerticalSlider(&wd->verticalSlider, updateVerticalSlider);
    gem_initHorizontalSlider(&wd->horizontalSlider, updateHorizontalSlider);
}

void* gem_getViewModel(const WinData* wd) {
    return wd->viewModel;
}