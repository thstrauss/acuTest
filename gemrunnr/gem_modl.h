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
 
#ifndef __GEM_MODEL__
#define __GEM_MODEL__

#include <acu_cmmn.h>
#include <acu_list.h>
#include <acu_ldr.h>
#include <aes.h>

#include "g_window.h"

typedef struct TestModel_ {
	int totalTestNumber;
	int verticalPositionN;	
	
	ACU_Entry* entry;
	ACU_List* testList;
	char testFileName[13];
	char testFilePath[260];

    char windowTitle[270];
    char infoLine[256];
	
	OBJECT* content; 
} TestModel;


void gem_selectFile(const WinData* wd);

void gem_execute(const TestModel* testModel, const CellSize* cellSize);

void gem_initTestModel(TestModel* testModel);

void gem_content(const CellSize* cellSize, const TestModel* testModel);

void gem_drawContent(const WinData* wd, const void* testModel, const GRECT* clippingRect, const GRECT* workingRect);

#endif