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
#include <vdi.h>
#include <tos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gemrunnr.h"

#include <acu_util.h>
#include "gem_modl.h"
#include "gem_util.h"

int work_in[11];
int work_out[57];
int contrl[12];
int intin[128];
int ptsin[128];
int intout[128];
int ptsout[128];

void open_vwork(const WinData* wd) {
	int dummy;
	int i;

	wd->grafHandle = graf_handle(&wd->cellWidth, &wd->cellHeight, &dummy, &dummy);
	
	work_in[0] = 2 + Getrez();
	for (i = 1; i < 10; i++) {
		work_in[i] = 1;
	}
	work_in[10] = 2;
	v_opnvwk(work_in, &wd->grafHandle, work_out);
}

void setClip(const WinData* wd, const GRECT* rect, int flag) {
	int pxy[4];
	
	pxy[0] = rect->g_x;
	pxy[1] = rect->g_y;
	pxy[2] = rect->g_x + rect->g_w-1;
	pxy[3] = rect->g_y + rect->g_h-1;
	
	vs_clip(wd->grafHandle, flag, pxy);
}

void drawContent(const WinData* wd, const GRECT* clippingRect, const GRECT* workingRect) {
	int i;
	static OBJECT box = {-1, 1, -1, G_BOX, NONE, NORMAL, 0, 0, 0, 0, 0 };
	if (wd->content) {
		free(wd->content);
	}
	wd->content = (OBJECT*) acu_emalloc(sizeof(OBJECT)*20);
	
	memcpy(&wd->content[0], &box, sizeof(OBJECT));
	wd->content[0].ob_width = 3 * wd->cellWidth;
	wd->content[0].ob_height = 19 * wd->cellHeight;
	for (i=0; i<20; i++) {
		static OBJECT line = {-1, -1, -1, G_STRING, NONE, NORMAL, 0, 0, 0, 0, 0 };
		memcpy(&wd->content[i+1], &line, sizeof(OBJECT));

		wd->content[i+1].ob_next = i+2;
		wd->content[i+1].ob_y = i * wd->cellHeight;
		wd->content[i+1].ob_spec.free_string = acu_emalloc(3);
		sprintf(wd->content[i+1].ob_spec.free_string, "%d", i);
		wd->content[i+1].ob_width = (int) strlen(wd->content[i+1].ob_spec.free_string) * wd->cellWidth;
		wd->content[i+1].ob_height = wd->cellHeight;
	}
	wd->content[20].ob_next = -1;
	wd->content[20].ob_flags = NONE | LASTOB;
		
	wd->content[0].ob_x = workingRect->g_x;
	wd->content[0].ob_y = workingRect->g_y - wd->verticalPositionN * wd->cellHeight;
	
	wd->linesShown = 19;
	
	objc_draw(wd->content, 0, 1, clippingRect->g_x, clippingRect->g_y, clippingRect->g_w, clippingRect->g_h);

	UNUSED(wd);
}

void drawInterior(const WinData* wd, const GRECT* clippingRect) {
	GRECT workingRect;
	int pxy[4];
	
	graf_mouse(M_OFF, 0L);
	setClip(wd, clippingRect, 1);
	
	gem_getWorkingRect(wd, &workingRect);
	
	vsf_color(wd->grafHandle, WHITE);
	pxy[0] = workingRect.g_x;
	pxy[1] = workingRect.g_y;
	pxy[2] = workingRect.g_x + workingRect.g_w - 1;
	pxy[3] = workingRect.g_y + workingRect.g_h - 1;
	vr_recfl(wd->grafHandle, pxy);

	drawContent(wd, clippingRect, &workingRect);

	setClip(wd, clippingRect, 0);
	gem_updateSliders(wd);
	graf_mouse(M_ON, 0L);
}

void performRedraw(const WinData* wd, const GRECT* rect) {
	GRECT clippingRect;
	wind_update(BEG_UPDATE);
	wind_get(wd->windowHandle, WF_FIRSTXYWH, &clippingRect.g_x, &clippingRect.g_y, &clippingRect.g_w, &clippingRect.g_h);
	while (clippingRect.g_w && clippingRect.g_h) {
		if (gem_rectIntersect(rect, &clippingRect)) {
			drawInterior(wd, &clippingRect);
		} 
		wind_get(wd->windowHandle, WF_NEXTXYWH, &clippingRect.g_x, &clippingRect.g_y, &clippingRect.g_w, &clippingRect.g_h);
	}
	wind_update(END_UPDATE);
}

void performResize(const WinData* wd, const int* messageBuf) {
	int w1, w2;
	wind_set(wd->windowHandle, WF_CURRXYWH, 
		messageBuf[4], 
		messageBuf[5], 
		max(w, 300, messageBuf[6]), 
		max(w, 200, messageBuf[7]));
	gem_triggerRedraw(wd);
}

void performFullScreen(const WinData* wd) {
	int currx, curry, currw, currh;
	int fullx, fully, fullw, fullh;
	
	wind_get(wd->windowHandle, WF_CURRXYWH, 
		&currx, &curry, &currw, &currh);
	wind_get(wd->windowHandle, WF_FULLXYWH, 
		&fullx, &fully, &fullw, &fullh);
		
	if (currx == fullx && curry == fully && currw == fullw && currh == fullh) {
		int oldx, oldy, oldw, oldh;	
		wind_get(wd->windowHandle, WF_PREVXYWH, 
			&oldx, &oldy, &oldw, &oldh);
		wind_set(wd->windowHandle, WF_CURRXYWH, 
			oldx, oldy, oldw, oldh);
	} else {
		wind_set(wd->windowHandle, WF_CURRXYWH, 
			fullx, fully, fullw, fullh);
	}
} 

void doVerticalSlide(const WinData* wd, int verticalPositionN) {
	GRECT rect;
	int linesAvailable;
	
	gem_getWorkingRect(wd, &rect);
	linesAvailable = rect.g_h / wd->cellHeight;
	wd->verticalPositionN = (int) ((verticalPositionN * (long) (wd->linesShown - linesAvailable)) / 1000);
	if (wd->verticalPositionN < 0) {
		wd->verticalPositionN = 0;
	}
	wind_set(wd->windowHandle, WF_VSLIDE, verticalPositionN, 0, 0, 0);
	
	gem_triggerRedraw(wd);
}

void handleWindowMessage(const WinData* wd, const int* messageBuf) {
	switch (messageBuf[0]) {
		case WM_MOVED: {
			wind_set(wd->windowHandle, WF_CURRXYWH, 
				messageBuf[4], messageBuf[5], messageBuf[6], messageBuf[7]);
		} break;
		case WM_TOPPED: {
			wind_set(wd->windowHandle, WF_TOP, 0, 0);
		} break;
		case WM_REDRAW: {
			performRedraw(wd, (GRECT*) &messageBuf[4]);
		} break;
		case WM_SIZED: {
			performResize(wd, messageBuf);
		} break;
		case WM_FULLED: {
			performFullScreen(wd);
		} break;
		case WM_VSLID: {
			wind_set(wd->windowHandle, WF_TOP, 0, 0);
			doVerticalSlide(wd, messageBuf[4]);
		} break;
	}
}

void handleMenueMessage(const WinData* wd, int menuItem) {
	switch (menuItem) {
		case PROGRAM_ABOUT: {
			form_alert(1, "[0][ASC - GEM Runner|][ OK ]");
		} break;
		case FILE_LOAD_TEST: {
			gem_selectFile(wd);
		} break;	
	}
}

void eventLoop(const WinData* wd, OBJECT* menuAddr) {
	EVENT event;
	
	event.ev_mflags = MU_MESAG;
	
	do {
		int eventType = EvntMulti(&event);
		if (eventType & MU_MESAG) {
			switch(event.ev_mmgpbuf[0]) {
				case MN_SELECTED: {
					handleMenueMessage(wd, event.ev_mmgpbuf[4]);
					menu_tnormal(menuAddr, event.ev_mmgpbuf[3], 1);
				} break;
				default: {
					handleWindowMessage(wd, event.ev_mmgpbuf);
				} break;
			}
		}
	} while(
		!(event.ev_mmgpbuf[0] == MN_SELECTED && event.ev_mmgpbuf[4] == FILE_QUIT)
	);
}

int startProgram(WinData* wd) {
	int fullx, fully, fullw, fullh;
	
	gem_initWinData(wd);
	
	if (gemrunnr_rsc_load(wd->cellWidth, wd->cellHeight) == 0) {
		form_alert(1, "[3][Could not load rsc][Exit]");
		return 2;
	} else {
		OBJECT* menu_addr;
		
		gemrunnr_rsc_gaddr(R_TREE, MAIN_MENU, &menu_addr);
		menu_bar(menu_addr, 1);
	
		graf_mouse(ARROW, 0L);
		wind_get(0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);
	
		wd->windowHandle = wind_create(
			NAME | MOVER | SIZER | FULLER | INFO | VSLIDE | UPARROW | DNARROW, 
			fullx, fully, fullw, fullh);
			
		wind_set(wd->windowHandle, WF_NAME, "GEM Runner", 0, 0);
		wind_open(wd->windowHandle, fullx, fully, 300, 200);
	
		eventLoop(wd, menu_addr);
		
		menu_bar(menu_addr, 0);
	
		wind_close(wd->windowHandle);
		wind_delete(wd->windowHandle);
		
		gemrunnr_rsc_free();
	}
	return 0;
}

int main(void) {
	int result;
	WinData wd;

	wd.applId = appl_init();
	if (wd.applId < 0) {
		form_alert(1, "[3][Could not initialize GEM][Exit]");
		return 2;
	}
	
	open_vwork(&wd);
	
	result = startProgram(&wd);
	
	v_clsvwk(wd.grafHandle);
	
	appl_exit();
	return result;
}