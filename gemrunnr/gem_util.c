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

#include <stdio.h>

void gem_triggerRedrawDesktop(const WinData* wd) {
	GRECT rect;
 	unsigned int message[8];
	int ap_id = appl_find("SCRENMGR");

	wind_get(0, WF_WORKXYWH, &rect.g_x, &rect.g_y, &rect.g_w, &rect.g_h);

 	message[0] = WM_REDRAW;
 	message[1] = wd->applId;
 	message[2] = 0;
 	message[3] = 0;
 	message[4] = rect.g_x;
 	message[5] = rect.g_y;
 	message[6] = rect.g_w;
 	message[7] = rect.g_h;
 	appl_write(ap_id, (int) sizeof(message), &message);
}

void gem_getWorkingRect(const WinData* wd, GRECT* rect) {
	wind_get(wd->windowHandle, WF_WORKXYWH, &rect->g_x, &rect->g_y, &rect->g_w, &rect->g_h);
}

void gem_triggerRedrawRect(const WinData* wd, GRECT* rect) {
 	unsigned int message[8];

 	message[0] = WM_REDRAW;
 	message[1] = wd->applId;
 	message[2] = 0;
 	message[3] = wd->windowHandle;
 	message[4] = rect->g_x;
 	message[5] = rect->g_y;
 	message[6] = rect->g_w;
 	message[7] = rect->g_h;
 	appl_write(wd->applId, (int) sizeof(message), &message);
}

void gem_triggerRedraw(const WinData* wd) {
	GRECT rect;

	gem_getWorkingRect(wd, &rect);

	gem_triggerRedrawRect(wd, &rect);
}

int gem_sliderSize(int numAvailable, int numShown) {
	if (numAvailable >= numShown) {
		return 1000;
	}
	return (int) ((1000L * numAvailable) / numShown);
} 

int gem_sliderPositionN(int numAvailable, int numShown, int offset) {
	if (numAvailable >= numShown) {
		return 0;
	} else {
		int scrollableRegion = numShown - numAvailable;
		int temp1 = offset / scrollableRegion;
		int temp2 = offset % scrollableRegion;
	
		return (int) ((1000L * temp1) + ((1000L * temp2) / scrollableRegion));
	}
}

void gem_updateSliders(const WinData* wd) {
	int linesAvailable;
	GRECT rect;
	TestModel* testModel = gem_getTestModel(wd);
	
	gem_getWorkingRect(wd, &rect);
	
	linesAvailable = rect.g_h / wd->cellHeight;
	
	wind_set(wd->windowHandle, WF_VSLSIZE, 
		gem_sliderSize(linesAvailable, testModel->linesShown), 0, 0, 0);
	wind_set(wd->windowHandle, WF_VSLIDE, 
		gem_sliderPositionN(linesAvailable, testModel->linesShown, testModel->verticalPositionN), 0, 0, 0);
}

int gem_rectIntersect(const GRECT* r1, GRECT* r2) {
	int ret;
	int tx, tw;	
	int w1, w2;
	
	tx = max(w, r2->g_x, r1->g_x);
	tw = min(w, r2->g_x + r2->g_w, r1->g_x + r1->g_w);
	
	ret = (tw > tx);
	if (ret) {
		int ty, th;
		ty = max(w, r2->g_y, r1->g_y);
		th = min(w, r2->g_y + r2->g_h, r1->g_y + r1->g_h);
		ret = (th > ty);
		if (ret) {
			r2->g_x = tx;
			r2->g_y = ty;
			r2->g_w = tw - tx;
			r2->g_h = th - ty;
		}
	}
	
	return ret;
}