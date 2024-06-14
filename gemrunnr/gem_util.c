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
 
#include <aes.h>
#include "gem_util.h"
#include "gem_modl.h"
 
void gem_triggerRedraw(const WinData* wd) {
 	unsigned int message[8];
	int wrkx, wrky, wrkw, wrkh;

	wind_get(wd->windowHandle, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh);

 	message[0] = WM_REDRAW;
 	message[1] = wd->applId;
 	message[2] = 0;
 	message[3] = wd->windowHandle;
 	message[4] = wrkx;
 	message[5] = wrky;
 	message[6] = wrkw;
 	message[7] = wrkh;
 	appl_write(wd->applId, (int) sizeof(message), &message);
 } 