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

#include "gem_modl.h"

void gem_initWinData(WinData* wd, int applId, int grafHandle) {
	wd->applId = applId;
	wd->grafHandle = grafHandle;
	wd->testFileName = NULL;
	wd->testFilePath = acu_emalloc(256);
	strcpy(wd->testFilePath, "*.cup");
}

void gem_selectFile(const WinData* wd) {
	char buf[256];
	int button;
			
	memset(buf, 0, sizeof(buf));
	fsel_exinput(wd->testFilePath, buf, &button, "Select test");
	if (button == 1) {
		if (wd->testFileName) {
			free(wd->testFileName);
		}
		wd->testFileName = acu_estrdup(buf);
		wind_set(wd->windowHandle, WF_INFO, wd->testFileName);
		sprintf(buf, "GEM Runner: %s\\%s", acu_getPath(wd->testFilePath), wd->testFileName);
		wd->windowTitle = acu_estrdup(buf);
		wind_set(wd->windowHandle, WF_NAME, wd->windowTitle, 0, 0);
	}
}