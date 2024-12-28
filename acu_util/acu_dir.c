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

#include "acu_dir.h"
#include "acu_util.h"
#include "acu_list.h"
#include "acu_strg.h"


ACU_Files* acu_filesMalloc(void)
{
    return acu_emalloc(sizeof(ACU_Files));
}

static void acu_fileEntryDestroy(ACU_FileEntry* entry) {
    if (entry->fileName) {
        acu_free(entry->fileName);
    }
    acu_free(entry);
}

void acu_filesInit(ACU_Files* files)
{
    files->fileList = acu_mallocList();
    acu_initList(files->fileList, (ACU_ListDestroyFunc*) acu_fileEntryDestroy, NULL);
}

void acu_filesDestroy(ACU_Files* files)
{
    acu_destroyList(files->fileList);
    acu_free(files->fileList);
    files->fileList = NULL;
}

void acu_filesAccept(const ACU_Files* files, ACU_FilesVisitor* visitor) {
    ACU_ListVisitor listVisitor;
    listVisitor.visitor = (ACU_ListVisitorFunc*) visitor->visitor;
    listVisitor.context = visitor->context;
    acu_acceptList(files->fileList, &listVisitor);
}

char* acu_getPath(const char* file) {
    char* buffer;
    char* token;
    if (!file) {
        return NULL;
    }
    buffer = (char*) acu_estrdup(file);
    token = strrchr(buffer, '\\');
    if (token) {
        *token = '\0';
    } else {
        *buffer = '\0'; 
    }
    return buffer;
}