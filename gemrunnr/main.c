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

#include <gemrunnr.h>

#include "gem_modl.h"

int work_in[11];
int work_out[57];
int contrl[12];
int intin[128];
int ptsin[128];
int intout[128];
int ptsout[128];

int open_vwork(void) {
	int dummy;
	int i;

	int grafHandle = graf_handle(&dummy, &dummy, &dummy, &dummy);
	
	work_in[0] = 2 + Getrez();
	for (i = 1; i < 10; i++) {
		work_in[i] = 1;
	}
	work_in[10] = 2;
	v_opnvwk(work_in, &grafHandle, work_out);
	
	return grafHandle;
}

void setClip(int grafHandle, const GRECT* rect, int flag) {
	int pxy[4];
	
	pxy[0] = rect->g_x;
	pxy[1] = rect->g_y;
	pxy[2] = rect->g_x + rect->g_w-1;
	pxy[3] = rect->g_y + rect->g_h-1;
	
	vs_clip(grafHandle, flag, pxy);
}

void drawContent(const WinData* wd, const GRECT* rect, int x, int y, int w, int h) {
	static OBJECT content[4] = {
		{-1, 1, 2, G_BOX, NONE, NORMAL, 0, 0, 0, 10*8, 16*3 },
		{2, -1, -1, G_STRING, SELECTABLE, NORMAL, 0, 0, 0, 3*8, 16 },
		{3, -1, -1, G_STRING, SELECTABLE, NORMAL, 0, 0, 16, 3*8, 16 },
		{-1, -1, -1, G_STRING, SELECTABLE | LASTOB, NORMAL, 0, 0, 2*16, 3*8, 16 }
	};

	content[1].ob_spec.free_string = "123";
	content[2].ob_spec.free_string = "456";
	content[3].ob_spec.free_string = "789";
	content[0].ob_x = x+5;
	content[0].ob_y = y+5;
	objc_draw(content, 0, 1, rect->g_x, rect->g_y, rect->g_w, rect->g_h);
}

void drawInterior(const WinData* wd, const GRECT* rect) {
	int wrkx, wrky, wrkw, wrkh;
	int pxy[4];
	
	graf_mouse(M_OFF, 0L);
	setClip(wd->grafHandle, rect, 1);
	
	wind_get(wd->windowHandle, WF_WORKXYWH, &wrkx, &wrky, &wrkw, &wrkh);
	
	vsf_color(wd->grafHandle, WHITE);
	pxy[0] = wrkx;
	pxy[1] = wrky;
	pxy[2] = wrkx + wrkw - 1;
	pxy[3] = wrky + wrkh - 1;
	vr_recfl(wd->grafHandle, pxy);

	drawContent(wd, rect, wrkx, wrky, wrkw, wrkh);

	setClip(wd->grafHandle, rect, 0);
	graf_mouse(M_ON, 0L);
}

#define max(t, x, y) (##t##1 = (x), ##t##2 =(y), ##t##1 > ##t##2 ? ##t##1 : ##t##2)
#define min(t, x, y) (##t##1 = (x), ##t##2 =(y), ##t##1 < ##t##2 ? ##t##1 : ##t##2)

int rc_intersect(const GRECT* r1, GRECT* r2) {
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

void performRedraw(const WinData* wd, const GRECT* rect) {
	GRECT rect2;
	wind_update(BEG_UPDATE);
	wind_get(wd->windowHandle, WF_FIRSTXYWH, &rect2.g_x, &rect2.g_y, &rect2.g_w, &rect2.g_h);
	while (rect2.g_w && rect2.g_h) {
		if (rc_intersect(rect, &rect2)) {
			drawInterior(wd, &rect2);
		} 
		wind_get(wd->windowHandle, WF_NEXTXYWH, &rect2.g_x, &rect2.g_y, &rect2.g_w, &rect2.g_h);
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

void handleWindowMessage(const WinData* wd, const int* messageBuf) {
	switch (messageBuf[0]) {
		case WM_MOVED: {
			wind_set(messageBuf[3], WF_CURRXYWH, 
				messageBuf[4], messageBuf[5], messageBuf[6], messageBuf[7]);
		} break;
		case WM_TOPPED: {
			wind_set(messageBuf[3], WF_TOP, 0, 0);
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

int startProgram(int applId, int grafHandle) {
	int fullx, fully, fullw, fullh;
	WinData wd;
	
	gem_initWinData(&wd, applId, grafHandle);
	
	if (gemrunnr_rsc_load(8,16) == 0) {
		form_alert(1, "[3][Could not load rsc][Exit]");
		return 2;
	} else {
		OBJECT* menu_addr;
		
		gemrunnr_rsc_gaddr(R_TREE, MAIN_MENU, &menu_addr);
		menu_bar(menu_addr, 1);
	
		graf_mouse(ARROW, 0L);
		wind_get(0, WF_WORKXYWH, &fullx, &fully, &fullw, &fullh);
	
		wd.windowHandle = wind_create(NAME | MOVER | SIZER | FULLER | INFO, 
			fullx, fully, fullw, fullh);
			
		wind_set(wd.windowHandle, WF_NAME, "GEM Runner", 0, 0);
		wind_open(wd.windowHandle, fullx, fully, 300, 200);
	
		eventLoop(&wd, menu_addr);
		
		menu_bar(menu_addr, 0);
	
		wind_close(wd.windowHandle);
		wind_delete(wd.windowHandle);
		
		gemrunnr_rsc_free();
	}
	return 0;
}

int main(void) {
	int result;
	int grafHandle;

	int applId = appl_init();
	if (applId < 0) {
		form_alert(1, "[3][Could not initialize GEM][Exit]");
		return 2;
	}
	
	grafHandle = open_vwork();
	
	result = startProgram(applId, grafHandle);
	
	v_clsvwk(grafHandle);
	
	appl_exit();
	return result;
}