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

#include <windows.h>

#include "..\acu_dir.h"
#include "..\acu_util.h"

void acu_filesCollect(ACU_Files* files, const char* fileMask)
{
    WIN32_FIND_DATA findFileData;
    wchar_t wc_maskName[260];
    size_t filenameSize = 0;
    mbstowcs_s(&filenameSize, wc_maskName, 260, fileMask, _TRUNCATE);

    wchar_t exeFileName[260];
    GetModuleFileNameW(NULL, exeFileName, 260);
    *wcsrchr(exeFileName, '\\') = '\0';
    SetCurrentDirectoryW(exeFileName);
    HANDLE hFind = FindFirstFileW(wc_maskName, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do
        {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

            }
            else {
                char buffer[260];
                size_t size;
                wcstombs_s(&size, buffer, 260, findFileData.cFileName, _TRUNCATE);
                ACU_FileEntry* entry = acu_emalloc(sizeof(ACU_FileEntry));
                entry->fileName = acu_estrdup(buffer);
                acu_listAppend(files->fileList, (void*)entry);
            }
        } while (FindNextFileW(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
    else {
        char buffer[512];
        DWORD error = GetLastError();
        acu_printf_s(buffer, sizeof(buffer), "FindFirstFile failed (%d)\n", error);
    }
}
