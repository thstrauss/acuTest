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

#include <stdio.h>

#include "acu_plgn.h"
#include "acu_util.h"

ACU_Plugin* acu_pluginMalloc(void)
{
    return acu_emalloc(sizeof(ACU_Plugin));
}

#ifdef __TOS__

#include <string.h>
#include <stdlib.h>
#include <portab.h>
#include <tos.h>

#define PH_MAGIC 0x601a

typedef struct PH_
{
    WORD  ph_branch;
    LONG  ph_tlen;
    LONG  ph_dlen;
    LONG  ph_blen;
    LONG  ph_slen;
    LONG  ph_res1;
    LONG  ph_prgflags;
    WORD  ph_absflag;
} PH;

static void* relocate(const void* code, const unsigned char* relocData)
{
    unsigned char* lpRelocTable = (unsigned char*)relocData;
    register unsigned long lOffset = *(unsigned long*)lpRelocTable;
    unsigned char* lpText = (unsigned char*)code;

    lpRelocTable += sizeof(unsigned long*);

    if (lOffset) {
        *(unsigned long*)&lpText[lOffset] += (unsigned long)lpText;
        while (*lpRelocTable) {
            register unsigned char lFix = *lpRelocTable++;
            if (1 == lFix) {
                lOffset += 254;
            }
            else {
                lOffset += lFix;
                *(unsigned long*)&lpText[lOffset] += (unsigned long)lpText;
            }
        };
    }
    return code;
}

static unsigned char* readRelocationData(long handle, long fsize, const PH* programHeader) {
    unsigned char* relo_mem = NULL;
    long TD_len = programHeader->ph_tlen + programHeader->ph_dlen;
    long relo_len = fsize - sizeof(PH) - TD_len - programHeader->ph_slen;

    if ((programHeader->ph_absflag == 0) && relo_len) {
        relo_mem = malloc(relo_len);
        if (relo_mem) {
            if (!Fread((int)handle, relo_len, relo_mem) == relo_len) {
                free(relo_mem);
            }
        }
    }
    return relo_mem;
}

static void skipSymbolTable(long handle, const PH* programHeader) {
    Fseek(programHeader->ph_slen, (int)handle, 1);
}

static void cleanBSS(const PH* programHeader, void* textAndData) {
    memset((char*)textAndData + programHeader->ph_tlen + programHeader->ph_dlen, 0, programHeader->ph_blen);
}

static void* load_and_reloc(long handle, long fsize, const PH* programHeader)
{
    void* textAndData = NULL;
    void* relocatedTextAndData = NULL;

    long TD_len = programHeader->ph_tlen + programHeader->ph_dlen;
    long TDB_len = TD_len + programHeader->ph_blen;

    textAndData = malloc(TDB_len);
    if (textAndData) {

        if (Fread((int)handle, TD_len, textAndData) == TD_len) {
            unsigned char* relocationData;

            cleanBSS(programHeader, textAndData);

            skipSymbolTable(handle, programHeader);

            relocationData = readRelocationData(handle, fsize, programHeader);
            if (relocationData != NULL) {
                relocatedTextAndData = relocate(textAndData, relocationData);
                free(relocationData);
            }
        }
    }

    if (!relocatedTextAndData) {
        free(textAndData);
    }

    return relocatedTextAndData;
}

void acu_pluginLoad(ACU_Plugin* plugin, const char* cu_name)
{
    void* addr = NULL;
    long handle;
    long fileSize;
    
    plugin->pluginCode = NULL;

    handle = Fopen(cu_name, O_RDONLY);
    if (handle > 0)
    {
        PH programHeader;

        if (!Fread((int)handle, sizeof(PH), &programHeader) == sizeof(PH))
        {
            return;
        }

        if (programHeader.ph_branch != PH_MAGIC) {
            Fclose((short)handle);
            return;
        }
        fileSize = Fseek(0, (int)handle, 2);
        Fseek(sizeof(PH), (int)handle, 0);

        addr = load_and_reloc(handle, fileSize, &programHeader);
        Fclose((short)handle);
    }

    plugin->pluginCode = addr;
}

void acu_pluginUnload(ACU_Plugin* plugin) {
	free(plugin->pluginCode);
}

#else

#include <windows.h>

void acu_pluginLoad(ACU_Plugin* plugin, const char* cu_name)
{
    wchar_t  name[256];
    swprintf(name, 255, L"%hs", cu_name);
    HMODULE module = LoadLibraryW(name);
    if (module == 0) {
        return;
    }
    plugin->plugin = module;
}

void acu_pluginUnload(ACU_Plugin* plugin)
{
    FreeLibrary(plugin->plugin);
}

#endif
