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

#pragma once
#ifndef __acu_dir__
#define __acu_dir__

#include <acu_cmmn.h>
#include <acu_list.h>

typedef struct ACU_FileEntry_ {
    char* fileName;
} ACU_FileEntry;

typedef struct ACU_Files_ {
    ACU_List* fileList;
} ACU_Files;

__EXPORT ACU_Files* acu_filesMalloc(void);

__EXPORT void acu_filesInit(ACU_Files* files);

__EXPORT void acu_filesDestroy(ACU_Files* files);

__EXPORT void acu_filesCollect(ACU_Files* files, const char* fileName);

typedef void ACU_FilesVisitorFunc(const ACU_FileEntry* file, void* context);

__EXPORT void acu_filesAccept(const ACU_Files* files, ACU_FilesVisitorFunc visitor, void* visitorContext);

#endif
