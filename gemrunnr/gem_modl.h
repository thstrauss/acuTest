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

typedef struct WinData_ {
	int windowHandle;
	int applId;
	int grafHandle;
	
	int cellWidth;
	int cellHeight;
	
	int linesShown;
	int verticalPositionN;	
	
	ACU_Entry* entry;
	ACU_List* testList;
	char* testFileName;
	char* testFilePath;
	char* windowTitle;
	char* infoLine;
	
	OBJECT* content; 
} WinData;

void gem_initWinData(WinData* winData);

void gem_content(const WinData* wd);

void gem_selectFile(const WinData* wd);

#endif