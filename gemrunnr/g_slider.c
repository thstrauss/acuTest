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

#include "g_slider.h"
#include "g_util.h"
#include "g_window.h"
#include <aes.h>

static int gem_sliderSize(int numAvailable, int numShown) {
    if (numAvailable >= numShown) {
        return 1000;
    }
    return (int)((1000L * numAvailable) / numShown);
}

static int gem_sliderPositionN(int numAvailable, int numShown, int offset) {
    if (numAvailable >= numShown) {
        return 0;
    } else {
        int scrollableRegion = numShown - numAvailable;
        return (int)(offset * 1000L / scrollableRegion);
    }
}

void gem_initVerticalSlider(const VerticalSlider* verticalSlider, GEM_VerticalUpdateSliderModelFunc* updateModel)
{
    verticalSlider->updateModel = updateModel;
}

void gem_initHorizontalSlider(const HorizontalSlider* horizontalSlider, GEM_HorizontalUpdateSliderModelFunc* updateModel)
{
    horizontalSlider->updateModel = updateModel;
}

void gem_updateSliders(const WinData* winData) {
    int linesShown;
    int columnsShown;
    GRECT rect;
    void* model = gem_getViewModel(winData);
    VerticalSlider* verticalSlider = &winData->verticalSlider;
    HorizontalSlider* horizontalSlider = &winData->horizontalSlider;
    
    gem_getWorkingRect(winData, &rect);
    linesShown = rect.g_h / winData->cellSize.height;
    columnsShown = rect.g_w / winData->cellSize.width;
    verticalSlider->updateModel(verticalSlider, model);
    horizontalSlider->updateModel(horizontalSlider, model);

    wind_set(winData->windowHandle, WF_VSLSIZE,
        gem_sliderSize(linesShown, verticalSlider->available), 0, 0, 0);
    wind_set(winData->windowHandle, WF_VSLIDE,
        gem_sliderPositionN(linesShown, verticalSlider->available, verticalSlider->offset), 0, 0, 0);
    wind_set(winData->windowHandle, WF_HSLSIZE,
        gem_sliderSize(columnsShown, horizontalSlider->available), 0, 0, 0);
    wind_set(winData->windowHandle, WF_HSLIDE,
        gem_sliderPositionN(columnsShown, horizontalSlider->available, horizontalSlider->offset), 0, 0, 0);
}