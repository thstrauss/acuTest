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

static OBJECT progressBarTemplate[] = {
    { -1, 1, 3, G_BOX, 0x0400, OUTLINED, 0x21100L, 0,0, 29,5 },
    { 3, 2, 2, G_BOX, NONE, NORMAL, 0xFF1101L, 2,3, 25,1 }, /* PROGRESSBAR */
    { 1, -1, -1, G_BOX, NONE, NORMAL, 0xFF1121L, 0,0, 7,1 },
    { 0, -1, -1, G_STRING, LASTOB, NORMAL, 0, 2,1, 25,1 } /* TESTNAME */
};

