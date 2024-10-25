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

#ifndef __G_SLIDER__
#define __G_SLIDER__

struct WinData_;

typedef struct VerticalSlider_ {
    const struct WinData_* winData;
    void (*updateModel)(const struct VerticalSlider_* slider, const void* model);
    int available;
    int offset;
} VerticalSlider;

typedef struct HorizontalSlider_ {
    const struct WinData_* winData;
    void (*updateModel)(const struct HorizontalSlider_* slider, const void* model);
    int available;
    int offset;
} HorizontalSlider;

typedef void GEM_VerticalUpdateSliderModelFunc(const VerticalSlider* slider, const void* model);
typedef void GEM_HorizontalUpdateSliderModelFunc(const HorizontalSlider* slider, const void* model);

void gem_initVerticalSlider(const VerticalSlider* verticalSlider, const struct WinData_* winData, GEM_VerticalUpdateSliderModelFunc updateModel);
void gem_initHorizontalSlider(const HorizontalSlider* horizontalSlider, const struct WinData_* winData, GEM_HorizontalUpdateSliderModelFunc updateModel);


void gem_updateSliders(const VerticalSlider* verticalSlider);

#endif