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
#include "gem_modl.h"
#include "gem_util.h"
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
    }
    else {
        int scrollableRegion = numShown - numAvailable;
        int temp1 = offset / scrollableRegion;
        int temp2 = offset % scrollableRegion;

        return (int)((1000L * temp1) + ((1000L * temp2) / scrollableRegion));
    }
}

void gem_initVerticalSlider(const VerticalSlider* verticalSlider, const WinData* winData)
{
    verticalSlider->winData = winData;
}

void gem_updateSliders(const VerticalSlider* verticalSlider) {
    int linesAvailable;
    GRECT rect;
    TestModel* testModel = gem_getViewModel(verticalSlider->winData);

    gem_getWorkingRect(verticalSlider->winData, &rect);

    linesAvailable = rect.g_h / verticalSlider->winData->cellSize.height;

    wind_set(verticalSlider->winData->windowHandle, WF_VSLSIZE,
        gem_sliderSize(linesAvailable, testModel->totalTestNumber), 0, 0, 0);
    wind_set(verticalSlider->winData->windowHandle, WF_VSLIDE,
        gem_sliderPositionN(linesAvailable, testModel->totalTestNumber, testModel->verticalPositionN), 0, 0, 0);
}