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

void gem_initTestModel(TestModel* testModel) {
    testModel->entry = NULL;
    testModel->linesShown = 0;
    testModel->verticalPositionN = 0;
    
    testModel->content = NULL;
    testModel->testList = NULL;

    strcpy(testModel->testFileName, "");
    strcpy(testModel->testFilePath, "*.cup");
}


static size_t gem_getTestContent(char *buffer, size_t bufferSize, const ACU_TestCase* testCase) {
    return acu_sprintf_s(buffer, bufferSize, "%s : %s", testCase->result != NULL ? ((testCase->result->status == ACU_TEST_PASSED) ? "passed" : "failed") : "      ", testCase->name);
}

static void gem_contentLine(const CellSize* cellSize, const TestModel* testModel, const ACU_TestCase* testCase, int lineIndex) {
    OBJECT* line = &testModel->content[lineIndex + 1];

    size_t bufferSize = strlen(testCase->name) + 20;
    char* buffer = acu_emalloc(bufferSize);
    size_t lineLength = gem_getTestContent(buffer, bufferSize, testCase);
    
    line->ob_next = lineIndex + 2;
    line->ob_head = -1;
    line->ob_tail = -1;
    line->ob_type = G_STRING;
    line->ob_flags = NONE;
    line->ob_state = NORMAL;
    line->ob_spec.free_string = buffer;
    line->ob_x = 0;
    line->ob_y = lineIndex * cellSize->height;
    line->ob_width = (int) lineLength * cellSize->width;
    line->ob_height = cellSize->height;
}

void gem_content(const CellSize* cellSize, const TestModel* testModel) {
    static OBJECT box = {-1, -1, -1, G_BOX, NONE, NORMAL, 0, 0, 0, 0, 0 };
    
    int i;
    int numberOfLines;
    ACU_ListElement* testElement;
        
    numberOfLines = (testModel->testList) ? testModel->testList->size : 0;
    testModel->content = (OBJECT*) acu_emalloc(sizeof(OBJECT)*(numberOfLines+1));
    
    memcpy(&testModel->content[0], &box, sizeof(OBJECT));
    testModel->content[0].ob_width = 80 * cellSize->width;
    testModel->content[0].ob_height = numberOfLines * cellSize->height;
    if (testModel->testList) {
        testElement = acu_listHead(testModel->testList);
    }
    for (i=0; i<numberOfLines; i++) {
        gem_contentLine(cellSize, testModel, (ACU_TestCase*)testElement->data, i);
        testElement = acu_listNext(testElement);
    }
    if (numberOfLines > 0) {
        testModel->content[0].ob_head = 1;
        testModel->content[0].ob_tail = numberOfLines;
    }
    testModel->content[numberOfLines].ob_next = -1;
    testModel->content[numberOfLines].ob_flags |= LASTOB;
        
    testModel->linesShown = numberOfLines;
}

static void gem_setInfoLine(const WinData* wd) {
    ACU_Visitor counter = { acu_count, NULL };
    ACU_Count count = {0,0, {NULL, NULL}};
    counter.context = (void*)&count;

    acu_suiteAccept(gem_getTestModel(wd)->entry->suite, &counter);

    if (wd->infoLine) {
        free(wd->infoLine);
    }
    wd->infoLine = acu_emalloc(256);

    sprintf(wd->infoLine, "%d tests in %d fixtures", count.testCaseCount, count.testFixtureCount);
    wind_set(wd->windowHandle, WF_INFO, wd->infoLine);
}

static void gem_setWindowTitle(const WinData* wd) {
	TestModel* testModel;
    if (wd->windowTitle) {
        free(wd->windowTitle);
    }
    wd->windowTitle = acu_emalloc(270);
    testModel = gem_getTestModel(wd);
    sprintf(wd->windowTitle, "GEM Runner: %s\\%s", acu_getPath(testModel->testFilePath), testModel->testFileName);
    wind_set(wd->windowHandle, WF_NAME, wd->windowTitle, 0, 0);
}

static void gem_collectTestCases(const TestModel* testModel){
    ACU_Visitor collect = {acu_collectTestCases, NULL};
    ACU_TestCases testCases;
    
    testModel->testList = acu_listMalloc();
    acu_listInit(testModel->testList, (ACU_ListDestroyFunc*) NULL);
            
    testCases.testCases = testModel->testList;
    collect.context = (void*) &testCases;
    
    acu_suiteAccept(testModel->entry->suite, &collect); 
}

void gem_freeContent(const TestModel* testModel) {
    if (testModel->content) {
        int i;
        int numberOfLines = (testModel->testList) ? testModel->testList->size : 0;
        for (i = 0; i < numberOfLines; i++) {
            free(testModel->content[i + 1].ob_spec.free_string);
        }
        free(testModel->content);
    }
}

void gem_selectFile(const WinData* wd) {
    int button;
    TestModel* testModel = gem_getTestModel(wd);

    fsel_exinput(testModel->testFilePath, testModel->testFileName, &button, "Select test");
    if (button == 1) {
        graf_mouse(BUSYBEE, 0L);

        gem_freeContent(testModel);
        if (testModel->testList) {
            acu_listDestroy(testModel->testList);
            free(testModel->testList);
            testModel->testList = NULL;
        }

        if (testModel->entry) {
            cup_unload(testModel->entry);
            testModel->entry = NULL;
        }
        
        testModel->entry = cup_load(testModel->testFileName);
        if (testModel->entry) {
            gem_setInfoLine(wd);
            gem_setWindowTitle(wd);
            gem_collectTestCases(testModel);
        }
        gem_content(&wd->cellSize, testModel);
        gem_triggerRedraw(wd);
        graf_mouse(ARROW, 0L);
    }
}


static size_t nullHandler(const char* buffer) {
    UNUSED(buffer);
    return 0;
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

void gem_execute(const TestModel* testModel, const CellSize* cellSize) {
    ACU_Progress progress = {progressFunc, NULL};
    GemProgress gemProgress;
    Gem_ProgressBar* bar = gem_mallocProgressBar();

    gem_initProgressBar(bar, cellSize);

    gemProgress.bar = bar;
    gemProgress.testNumber = 0;
    gemProgress.totalTestNumber = testModel->linesShown;
    
    progress.context = &gemProgress;

    acu_setWriteHandler(nullHandler);
    if (testModel->entry) {
        graf_mouse(BUSYBEE, 0L);
        gem_showProgressBar(bar);
        acu_entryExecute(testModel->entry, &progress);
        gem_freeContent(testModel);
        gem_content(cellSize, testModel);
        gem_hideProgressBar(bar);
        graf_mouse(ARROW, 0L);
    } else {
        form_alert(1, "[1][Please load test first!][ OK ]");
    }
    gem_freeProgressBar(bar);
}

void gem_setViewModel(const WinData* wd, const void* viewModel) {
	wd->viewModel = viewModel;
}

TestModel* gem_getTestModel(const WinData* wd) {
	return (TestModel*) wd->viewModel;
}
