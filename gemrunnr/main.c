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
#include <screen.h>

#include "gemrunnr.h"

#include <acu_util.h>
#include "gem_modl.h"
#include "gem_util.h"
#include "g_slider.h"

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

    wd->grafHandle = graf_handle(&wd->cellSize.width, &wd->cellSize.height, &dummy, &dummy);
    
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

void drawInterior(const WinData* wd, const GRECT* clippingRect) {
    GRECT workingRect;
    int pxy[4];
    VerticalSlider slider;
    
    gem_initVerticalSlider(&slider, wd);
    
    graf_mouse(M_OFF, NULL);
    setClip(wd, clippingRect, 1);
    
    gem_getWorkingRect(wd, &workingRect);
    
    vsf_color(wd->grafHandle, WHITE);
    pxy[0] = workingRect.g_x;
    pxy[1] = workingRect.g_y;
    pxy[2] = workingRect.g_x + workingRect.g_w - 1;
    pxy[3] = workingRect.g_y + workingRect.g_h - 1;
    vr_recfl(wd->grafHandle, pxy);

    wd->drawViewModel(wd, gem_getViewModel(wd), clippingRect, &workingRect);

    setClip(wd, clippingRect, 0);
    gem_updateSliders(&slider);
    graf_mouse(M_ON, NULL);
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
    TestModel* testModel = gem_getViewModel(wd);

    gem_getWorkingRect(wd, &rect);
    linesAvailable = rect.g_h / wd->cellSize.height;
    testModel->verticalPositionN = (int) ((verticalPositionN * (long) (testModel->totalTestNumber - linesAvailable)) / 1000);
    if (testModel->verticalPositionN < 0) {
        testModel->verticalPositionN = 0;
    }
    wind_set(wd->windowHandle, WF_VSLIDE, verticalPositionN, 0, 0, 0);
    
    gem_triggerRedrawRect(wd, &rect);
}


void doPageUpDown(const WinData* wd, int arrow) {
    int linesAvailable;
    GRECT rect;
    TestModel* testModel = gem_getViewModel(wd);

    gem_getWorkingRect(wd, &rect);
    linesAvailable = rect.g_h / wd->cellSize.height;
    if (arrow == WA_UPPAGE) {
        if (testModel->verticalPositionN == 0) {
            return;
        }
        testModel->verticalPositionN -= linesAvailable;
    } 
    if (arrow == WA_DNPAGE) {
        if ((linesAvailable > testModel->totalTestNumber && testModel->verticalPositionN == 0) || testModel->verticalPositionN == testModel->totalTestNumber - linesAvailable) {
            return;
        }
        testModel->verticalPositionN += linesAvailable;
    }
    if (testModel->verticalPositionN < 0 || linesAvailable > testModel->totalTestNumber) {
        testModel->verticalPositionN = 0;
    }
    if (linesAvailable <= testModel->totalTestNumber && testModel->verticalPositionN > testModel->totalTestNumber - linesAvailable) {
        testModel->verticalPositionN = testModel->totalTestNumber - linesAvailable;
    } 

    gem_triggerRedraw(wd);
}

void doDownLine(const WinData* wd) {
    MFDB source;
    MFDB destination;
    GRECT rect;
    int pxy[8];
    int linesAvailable;
    TestModel* testModel = gem_getViewModel(wd);
    
    gem_getWorkingRect(wd, &rect);
    linesAvailable = rect.g_h / wd->cellSize.height;
    
    if (testModel->verticalPositionN >= testModel->totalTestNumber - linesAvailable) {
        return;
    }
    
    testModel->verticalPositionN++;
    if (testModel->verticalPositionN > linesAvailable && testModel->verticalPositionN > testModel->totalTestNumber - linesAvailable) {
        testModel->verticalPositionN = testModel->totalTestNumber - linesAvailable;
    } else if (linesAvailable >= testModel->totalTestNumber) {
        testModel->verticalPositionN = 0;
    }
    
    setClip(wd, &rect, 1);
    graf_mouse(M_OFF, NULL);
    
    source.fd_addr = NULL;
    destination.fd_addr = NULL;
    
    pxy[0] = rect.g_x;
    pxy[1] = rect.g_y + wd->cellSize.height + 1;
    pxy[2] = rect.g_x + rect.g_w;
    pxy[3] = rect.g_y + rect.g_h - 1;
    pxy[4] = rect.g_x;
    pxy[5] = rect.g_y + 1;
    pxy[6] = rect.g_x + rect.g_w;
    pxy[7] = rect.g_y + rect.g_h - wd->cellSize.height - 1;
    
    vro_cpyfm(wd->applId, S_ONLY, pxy, &source, &destination);
    
    graf_mouse(M_ON, NULL);
    setClip(wd, &rect, 0);
    
    rect.g_y = rect.g_y + rect.g_h - 2 * wd->cellSize.height;
    rect.g_h = 2 * wd->cellSize.height;
    gem_triggerRedrawRect(wd, &rect);
}

void doUpLine(const WinData* wd) {
    MFDB source;
    MFDB destination;
    GRECT rect;
    int pxy[8];
    TestModel* testModel = gem_getViewModel(wd);
    
    if (testModel->verticalPositionN == 0) {
        return;
    }
    gem_getWorkingRect(wd, &rect);
    testModel->verticalPositionN--;
    if (testModel->verticalPositionN < 0) {
        testModel->verticalPositionN = 0;
    }
    setClip(wd, &rect, 1);
    graf_mouse(M_OFF, NULL);
    
    source.fd_addr = NULL;
    destination.fd_addr = NULL;
    
    pxy[0] = rect.g_x;
    pxy[1] = rect.g_y + 1;
    pxy[2] = rect.g_x + rect.g_w;
    pxy[3] = rect.g_y + rect.g_h - wd->cellSize.height - 1;
    pxy[4] = rect.g_x;
    pxy[5] = rect.g_y + wd->cellSize.height + 1;
    pxy[6] = rect.g_x + rect.g_w;
    pxy[7] = rect.g_y + rect.g_h - 1;
    
    vro_cpyfm(wd->applId, S_ONLY, pxy, &source, &destination);
    
    graf_mouse(M_ON, NULL);
    setClip(wd, &rect, 0);
    
    rect.g_h = 2 * wd->cellSize.height;
    gem_triggerRedrawRect(wd, &rect);
}

void doArrowed(const WinData* wd, int arrow) {
    switch (arrow) {
        case WA_UPPAGE: 
        case WA_DNPAGE: {
            doPageUpDown(wd, arrow);
        } break;
        case WA_UPLINE: {
            doUpLine(wd);
        } break;
        case WA_DNLINE: {
            doDownLine(wd);
        } break;
    }
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
        case WM_ARROWED: {
            doArrowed(wd, messageBuf[4]);
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
        case TEST_EXECUTE: {
            TestModel* testModel = gem_getViewModel(wd);
            gem_execute(testModel, &wd->cellSize);
            gem_triggerRedraw(wd);
        } break;	
    }
}

void handleKeyboard(const WinData* wd, int mkReturn) {
    int ascii = mkReturn & 0xFF;
    int scanCode = (mkReturn >> 8) & 0xFF;
    
    switch (scanCode) {
        case 0x48: {
            if (ascii == 0x38) {
                doArrowed(wd, WA_UPPAGE);
            } else {
                doArrowed(wd, WA_UPLINE);
            }
        } break;
        case 0x50: {
            if (ascii == 0x32) {
                doArrowed(wd, WA_DNPAGE);
            } else {
                doArrowed(wd, WA_DNLINE);
            }
        } break;
    }
}

void eventLoop(const WinData* wd, OBJECT* menuAddr) {
    EVENT event;

    int previousObject = -1;
    
    event.ev_mflags = MU_MESAG | MU_KEYBD | MU_BUTTON;
    event.ev_mbclicks = 1;
    event.ev_mbmask = 1;
    event.ev_mbstate = 1;

    
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
        if (eventType & MU_KEYBD) {
            if (event.ev_mkreturn == 0x1011) {
                break;
            }
            handleKeyboard(wd, event.ev_mkreturn);
        }
        if ((eventType & MU_BUTTON)) {
            if (wind_find(event.ev_mmox, event.ev_mmoy) == wd->windowHandle) {
                TestModel* testModel = gem_getViewModel(wd);
                int nextObject = objc_find(testModel->content, 0, 7, event.ev_mmox, event.ev_mmoy);
                if (nextObject != -1) {
                    if (previousObject != -1) {
                        objc_change(testModel->content, previousObject, 0, 0, 0, 0, 0, NORMAL, 0);					
                    }
                    objc_change(testModel->content, nextObject, 0, 0, 0, 0, 0, OUTLINED, 0);
                    previousObject = nextObject;
                    gem_triggerRedraw(wd);
                }
            }
        }
    } while(
        !(event.ev_mmgpbuf[0] == MN_SELECTED && event.ev_mmgpbuf[4] == FILE_QUIT)
    );
}

int startProgram(WinData* wd) {
    int fullx, fully, fullw, fullh;
    
    gem_initWinData(wd);

    gem_setViewModel(wd, acu_emalloc(sizeof(TestModel)));

    gem_initTestModel(gem_getViewModel(wd));

    gem_content(&wd->cellSize, gem_getViewModel(wd));

    gem_setDrawViewModelFunc(wd, gem_drawContent);

    if (gemrunnr_rsc_load(wd->cellSize.width, wd->cellSize.height) == 0) {
        form_alert(1, "[3][Could not load rsc][ Exit ]");
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