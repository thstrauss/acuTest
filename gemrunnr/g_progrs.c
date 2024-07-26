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

#include "g_progrs.h"

#include <aes.h>
#include <string.h>
#include <stdlib.h>
#include <acu_util.h>

Gem_ProgressBar* gem_mallocProgressBar() {
    return acu_emalloc(sizeof(Gem_ProgressBar));
}

void gem_freeProgressBar(const Gem_ProgressBar* progressBar) {
    free(progressBar->barObject[4].ob_spec.free_string);
    free(progressBar);
}

void gem_initProgressBar(const Gem_ProgressBar* progressBar, int cellWidth, int cellHeight) {
    static OBJECT barObjectTemplate[] = {
        { -1, 1, 3, G_BOX, 0x0400, OUTLINED, 0x21100L, 0,0, 29,5 },
        { 3, 2, 2, G_BOX, NONE, NORMAL, 0xFF1101L, 2,3, 25,1 }, /* PROGRESSBAR */
        { 1, -1, -1, G_BOX, NONE, NORMAL, 0xFF1121L, 0,0, 7,1 },
        { 0, 4, 4, G_BOX, NONE, NORMAL, 0x1101L, 2,1, 25,1 },
        { 3, -1, -1, G_STRING, LASTOB, NORMAL, 0, 0,0, 25,1 } /* TESTNAME */
    };

	int xOffset;
	int yOffset;
    int i;
    progressBar->barObject = acu_emalloc(sizeof(barObjectTemplate));
    memcpy(progressBar->barObject, barObjectTemplate, sizeof(barObjectTemplate));
    
    progressBar->buffer[0] = '\0';
    
    for (i=0; i < 5; i++) {
        progressBar->barObject[i].ob_x *= cellWidth;
        progressBar->barObject[i].ob_y *= cellHeight;
        progressBar->barObject[i].ob_width *= cellWidth;
        progressBar->barObject[i].ob_height *= cellHeight;
    }
    progressBar->barObject[2].ob_width = 0;
    progressBar->barObject[4].ob_spec.free_string = progressBar->buffer;
    form_center(progressBar->barObject, 
    	&progressBar->clipX, 
    	&progressBar->clipY, 
    	&progressBar->clipWidth, 
    	&progressBar->clipHeight);
    xOffset = progressBar->barObject[0].ob_x;
    yOffset = progressBar->barObject[0].ob_y;
    
    progressBar->bar = &progressBar->barObject[2];
    progressBar->info = &progressBar->barObject[3];
    
    progressBar->barX = xOffset + progressBar->barObject[1].ob_x;
    progressBar->barY = yOffset + progressBar->barObject[1].ob_y;
    progressBar->infoX = xOffset + progressBar->info->ob_x;
    progressBar->infoY = yOffset + progressBar->info->ob_y;
    
    progressBar->progressBarWidth = progressBar->barObject[1].ob_width;

}

void gem_showProgressBar(const Gem_ProgressBar* progressBar) {
    form_dial(FMD_START, 
        progressBar->clipX, progressBar->clipY, progressBar->clipWidth, progressBar->clipHeight, 
        progressBar->clipX, progressBar->clipY, progressBar->clipWidth, progressBar->clipHeight);
    objc_draw(progressBar->barObject, 0, 8, progressBar->clipX, progressBar->clipY, progressBar->clipWidth, progressBar->clipHeight);
}

void gem_hideProgressBar(const Gem_ProgressBar* progressBar) {
    form_dial(FMD_FINISH,
        progressBar->clipX, progressBar->clipY, progressBar->clipWidth, progressBar->clipHeight, 
        progressBar->clipX, progressBar->clipY, progressBar->clipWidth, progressBar->clipHeight);
}

void gem_updateProgressBar(const Gem_ProgressBar* progressBar, int position, const char* info) {
    progressBar->bar->ob_width = position;
    acu_ellipsisString(progressBar->buffer, 25, info, 25);

    objc_draw(progressBar->barObject, 2, 1,
        progressBar->barX,
		progressBar->barY,
        progressBar->bar->ob_width,
        progressBar->bar->ob_height);
    objc_draw(progressBar->barObject, 3, 2,
        progressBar->infoX,
		progressBar->infoY,
        progressBar->info->ob_width,
        progressBar->info->ob_height);
}

int gem_calcProgressBarPosition(const Gem_ProgressBar* progressBar, int value, int maxValue) {
	return (int) (((long) value * progressBar->progressBarWidth) / maxValue);
}