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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <aes.h>

#include <acu_util.h>
#include <acu_dir.h>
#include <acu_ldr.h>
#include <acu_tryc.h>
#include <acu_rprt.h>
#include <acu_suit.h>

#include "gem_modl.h"
#include "gem_util.h"
#include "g_progrs.h"

void gem_initWinData(WinData* wd) {
    wd->testModel.testFileName = NULL;
    wd->testModel.testFilePath = acu_emalloc(256);
    wd->infoLine = NULL;
    wd->windowTitle = NULL;
    wd->testModel.entry = NULL;
    wd->testModel.linesShown = 0;
    
    wd->testModel.content = NULL;
    
    strcpy(wd->testModel.testFilePath, "*.cup");
    gem_content(wd);
}

static char* gem_getTestContent(const ACU_TestCase* testCase) {
    size_t bufferSize; 
    char* buffer;
    char resultBuffer[20] = "";
    
    if (testCase->result) {
        strcpy(resultBuffer, (testCase->result->status == ACU_TEST_PASSED) ? "passed" : "failed");
    }

    bufferSize = strlen(testCase->name) + 20;
    buffer = acu_emalloc(bufferSize);
    acu_sprintf_s(buffer, bufferSize, "%6s : %s", resultBuffer, testCase->name);
    return buffer;
}

static void gem_contentLine(const WinData* wd, const ACU_TestCase* testCase, int lineIndex) {
    static OBJECT line = { -1, -1, -1, G_STRING, NONE, NORMAL, 0, 0, 0, 0, 0 };
    char* lineContent = gem_getTestContent(testCase);
    
    memcpy(&wd->testModel.content[lineIndex + 1], &line, sizeof(OBJECT));
    wd->testModel.content[lineIndex + 1].ob_next = lineIndex + 2;
    wd->testModel.content[lineIndex + 1].ob_y = lineIndex * wd->cellHeight;
    wd->testModel.content[lineIndex + 1].ob_spec.free_string = lineContent;
    wd->testModel.content[lineIndex + 1].ob_width = (int)strlen(lineContent) * wd->cellWidth;
    wd->testModel.content[lineIndex + 1].ob_height = wd->cellHeight;
}

void gem_content(const WinData* wd) {
    static OBJECT box = {-1, -1, -1, G_BOX, NONE, NORMAL, 0, 0, 0, 0, 0 };
    
    int i;
    int numberOfLines;
    ACU_ListElement* testElement;
        
    numberOfLines = (wd->testModel.testList) ? wd->testModel.testList->size : 0;
    wd->testModel.content = (OBJECT*) acu_emalloc(sizeof(OBJECT)*(numberOfLines+1));
    
    memcpy(&wd->testModel.content[0], &box, sizeof(OBJECT));
    wd->testModel.content[0].ob_width = 80 * wd->cellWidth;
    wd->testModel.content[0].ob_height = numberOfLines * wd->cellHeight;
    if (wd->testModel.testList) {
        testElement = acu_listHead(wd->testModel.testList);
    }
    for (i=0; i<numberOfLines; i++) {
        gem_contentLine(wd, (ACU_TestCase*)testElement->data, i);
        testElement = acu_listNext(testElement);
    }
    if (numberOfLines > 0) {
        wd->testModel.content[0].ob_head = 1;
        wd->testModel.content[0].ob_tail = numberOfLines;
    }
    wd->testModel.content[numberOfLines].ob_next = -1;
    wd->testModel.content[numberOfLines].ob_flags |= LASTOB;
        
    wd->testModel.linesShown = numberOfLines;

}

static void gem_setInfoLine(const WinData* wd) {
    ACU_Visitor counter = { acu_count, NULL };
    ACU_Count count = {0,0, {NULL, NULL}};
    counter.context = (void*)&count;

    acu_suiteAccept(wd->testModel.entry->suite, &counter);

    if (wd->infoLine) {
        free(wd->infoLine);
    }
    wd->infoLine = acu_emalloc(256);

    sprintf(wd->infoLine, "%d tests in %d fixtures", count.testCaseCount, count.testFixtureCount);
    wind_set(wd->windowHandle, WF_INFO, wd->infoLine);
}

static void gem_setWindowTitle(const WinData* wd) {
    if (wd->windowTitle) {
        free(wd->windowTitle);
    }
    wd->windowTitle = acu_emalloc(270);
    sprintf(wd->windowTitle, "GEM Runner: %s\\%s", acu_getPath(wd->testModel.testFilePath), wd->testModel.testFileName);
    wind_set(wd->windowHandle, WF_NAME, wd->windowTitle, 0, 0);
}

void gem_collectTestCases(const WinData* wd){
    ACU_Visitor collect = {acu_collectTestCases, NULL};
    ACU_TestCases testCases;
    
    wd->testModel.testList = acu_listMalloc();
    acu_listInit(wd->testModel.testList, (ACU_ListDestroyFunc*) NULL);
            
    testCases.testCases = wd->testModel.testList;
    collect.context = (void*) &testCases;
    
    acu_suiteAccept(wd->testModel.entry->suite, &collect); 
}

static void gem_freeContent(const WinData* wd) {
    if (wd->testModel.content) {
        int i;
        int numberOfLines = (wd->testModel.testList) ? wd->testModel.testList->size : 0;
        for (i = 0; i < numberOfLines; i++) {
            free(wd->testModel.content[i + 1].ob_spec.free_string);
        }
        free(wd->testModel.content);
    }
}

void gem_selectFile(const WinData* wd) {
    char buf[256];
    int button;

    if (wd->testModel.testFileName) {
        free(wd->testModel.testFileName);
        wd->testModel.testFileName = NULL;
    }
    buf[0] = '\0';
    fsel_exinput(wd->testModel.testFilePath, buf, &button, "Select test");
    if (button == 1) {
        graf_mouse(BUSYBEE, 0L);

        gem_freeContent(wd);
        if (wd->testModel.testList) {
            acu_listDestroy(wd->testModel.testList);
            free(wd->testModel.testList);
            wd->testModel.testList = NULL;
        }

        if (wd->testModel.entry) {
            cup_unload(wd->testModel.entry);
            wd->testModel.entry = NULL;
        }
        
        wd->testModel.entry = cup_load(buf);
        if (wd->testModel.entry) {
            wd->testModel.testFileName = acu_estrdup(buf);
            gem_setInfoLine(wd);
            gem_setWindowTitle(wd);
            gem_collectTestCases(wd);
        }
        gem_content(wd);
        gem_triggerRedraw(wd);
        graf_mouse(ARROW, 0L);
    }
}


static size_t nullHandler(const char* buffer) {
    return strlen(buffer);
}

typedef struct GemProgress_ {
    const Gem_ProgressBar* bar;
    int testNumber;
    int totalTestNumber;
} GemProgress;

static void progressFunc(const ACU_TestCase* testCase, void* progressContext) {
    GemProgress* progress = (GemProgress*) progressContext;
    gem_updateProgressBar(progress->bar, 
    	gem_calcProgressBarPosition(progress->bar, progress->testNumber++, progress->totalTestNumber), testCase->name);
}

void gem_execute(const WinData* wd) {
    ACU_Progress progress = {progressFunc, NULL};
    GemProgress gemProgress;
    Gem_ProgressBar* bar = gem_mallocProgressBar();

    gem_initProgressBar(bar, wd->cellWidth, wd->cellHeight);

    gemProgress.bar = bar;
    gemProgress.testNumber = 0;
    gemProgress.totalTestNumber = wd->testModel.linesShown;
    
    progress.context = &gemProgress;

    acu_setWriteHandler(nullHandler);
    if (wd->testModel.entry) {
        graf_mouse(BUSYBEE, 0L);
        gem_showProgressBar(bar);
        acu_entryExecute(wd->testModel.entry, &progress);
        gem_freeContent(wd);
        gem_content(wd);
        gem_hideProgressBar(bar);
        gem_triggerRedraw(wd);
        graf_mouse(ARROW, 0L);
    } else {
        form_alert(1, "[1][Please load test first!][ OK ]");
    }
    gem_freeProgressBar(bar);
}
