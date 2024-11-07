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
 
#include <ext.h>
#include <string.h>
 
#include "acu_dir.h"
#include "acu_util.h"
#include "acu_list.h"
#include "acu_strg.h"
 
 void acu_filesCollect(ACU_Files* files, const char* fileMask)
{
    struct ffblk findFirst;
    
    if (findfirst(fileMask, &findFirst, 0) == 0) {
        do {
            ACU_FileEntry* entry = acu_emalloc(sizeof(ACU_FileEntry));
            entry->fileName = acu_estrdup(findFirst.ff_name);
            acu_appendList(files->fileList, (void*) entry);
        } while (findnext(&findFirst) == 0);
    }
}
