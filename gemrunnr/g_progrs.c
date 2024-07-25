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
        { 0, 4, 4, G_BOX, NONE, NORMAL, 0xFF1101L, 2,1, 25,1 }, /* TESTNAME */
        { 3, -1, -1, G_STRING, LASTOB, NORMAL, 0, 0,0, 25,1 } /* TESTNAME */
    };

    int i;
    progressBar->barObject = acu_emalloc(sizeof(barObjectTemplate));
    memcpy(progressBar->barObject, barObjectTemplate, sizeof(barObjectTemplate));
    
    for (i=0; i < 5; i++) {
        progressBar->barObject[i].ob_x *= cellWidth;
        progressBar->barObject[i].ob_y *= cellHeight;
        progressBar->barObject[i].ob_width *= cellWidth;
        progressBar->barObject[i].ob_height *= cellHeight;
    }
    progressBar->barObject[2].ob_width = 0;
    progressBar->barObject[4].ob_spec.free_string = acu_estrdup("123");
    form_center(progressBar->barObject, &progressBar->x, &progressBar->y, &progressBar->w, &progressBar->h);
}

void gem_showProgressBar(const Gem_ProgressBar* progressBar) {
    form_dial(FMD_START, 
        progressBar->x, progressBar->y, progressBar->w, progressBar->h, 
        progressBar->x, progressBar->y, progressBar->w, progressBar->h);
    objc_draw(progressBar->barObject, 0, 8, progressBar->x, progressBar->y, progressBar->w, progressBar->h);
}

void gem_hideProgressBar(const Gem_ProgressBar* progressBar) {
    form_dial(FMD_FINISH,
        progressBar->x, progressBar->y, progressBar->w, progressBar->h,
        progressBar->x, progressBar->y, progressBar->w, progressBar->h);
}

void gem_updateProgressBar(const Gem_ProgressBar* progressBar, double percent, const char* info) {
    int pos;
    int xOffset = progressBar->barObject[0].ob_x;
    int yOffset = progressBar->barObject[0].ob_y;

    OBJECT* barObject = &progressBar->barObject[2];
    OBJECT* infoBoxObject = &progressBar->barObject[3];

    free(progressBar->barObject[4].ob_spec.free_string);
    pos = (int) (percent / 100.0 * progressBar->barObject[1].ob_width);
    barObject->ob_width = pos;
    objc_draw(progressBar->barObject, 2, 1,
        xOffset + progressBar->barObject[1].ob_x,
        yOffset + progressBar->barObject[1].ob_y,
        barObject->ob_width,
        barObject->ob_height);
    progressBar->barObject[4].ob_spec.free_string = acu_estrdup(info);
    objc_draw(progressBar->barObject, 3, 2,
        xOffset + infoBoxObject->ob_x,
        yOffset + infoBoxObject->ob_y,
        infoBoxObject->ob_width,
        infoBoxObject->ob_height);
}