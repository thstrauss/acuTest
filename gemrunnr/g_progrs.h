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

#ifndef __gem_progress__
#define __gem_progress__

#include <aes.h>
#include "g_cellsz.h"

typedef struct Gem_ProgressBar_ {
    OBJECT* barObject;
    OBJECT* bar;
    OBJECT* info;
    int clipX, clipY, clipWidth, clipHeight;
    int barX, barY;
    int infoX, infoY;
    long progressBarWidth;
    char buffer[25];
} Gem_ProgressBar;

Gem_ProgressBar* gem_mallocProgressBar(void);

void gem_freeProgressBar(const Gem_ProgressBar* progressBar);
void gem_initProgressBar(const Gem_ProgressBar* progressBar, const CellSize* cellSize);
void gem_showProgressBar(const Gem_ProgressBar* progressBar);
void gem_hideProgressBar(const Gem_ProgressBar* progressBar);
void gem_updateProgressBar(const Gem_ProgressBar* progressBar, int position, const char* info);
int gem_calcProgressBarPosition(const Gem_ProgressBar* progressBar, long value, long maxValue);

 #endif