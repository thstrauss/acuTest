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
 
#ifndef __GEM_UTILS__
#define __GEM_UTILS__

#include <aes.h>
#include <g_window.h>
#include <g_cellsze.h>

#define max(t, x, y) (##t##1 = (x), ##t##2 = (y), ##t##1 > ##t##2 ? ##t##1 : ##t##2)
#define min(t, x, y) (##t##1 = (x), ##t##2 = (y), ##t##1 < ##t##2 ? ##t##1 : ##t##2)

void gem_getWorkingRect(const WinData* wd, GRECT* rect);

void gem_triggerRedrawDesktop(const WinData* wd);

void gem_triggerRedraw(const WinData* wd);
void gem_triggerRedrawRect(const WinData* wd, GRECT* rect);

int gem_rectIntersect(const GRECT* r1, GRECT* r2);

void gem_scaleObjectTree(const OBJECT* objectTree, int numObjects, const CellSize* cellSize);

#endif