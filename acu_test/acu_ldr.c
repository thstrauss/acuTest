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
#include <stdlib.h>

#include "acu_ldr.h"
#include "acu_util.h"
#include "acu_vers.h"
#include "acu_tryc.h"

static int checkVersion(ACU_Entry* entry) {
    ACU_Version* libVersion = entry->getAcuTestVersion();
    ACU_Version* version = acu_getVersion();
    if (!acu_compareVersion(version, libVersion)) {
        char* libversionStr = acu_formatVersion(libVersion);
        char* versionStr = acu_formatVersion(version);
        acu_eprintf("Version of acu library %s does not match to runner %s", libversionStr, versionStr);
        free(versionStr);
        free(libversionStr);
        return 0;
    }
    return 1;
}


#ifdef __TOS__

#include <string.h>
#include <stdlib.h>
#include <portab.h>
#include <tos.h>
#include "acu_tryc.h"
#include "acu_stck.h"

#define PH_MAGIC 0x601a

ACU_exitFunc* exitFunc;

static void setExit(ACU_exitFunc* exit) {
	exitFunc = exit;
}

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
    unsigned char *	lpRelocTable = (unsigned char*) relocData;
    register unsigned long lOffset = *(unsigned long*) lpRelocTable;
    unsigned char *	lpText = (unsigned char*) code;
    
    lpRelocTable += sizeof(unsigned long*);

    if( lOffset ) {
        *(unsigned long*)&lpText[ lOffset ] += (unsigned long)lpText;
        while( *lpRelocTable ) {
        	register unsigned char lFix = *lpRelocTable++;
            if( 1 == lFix ) {
                lOffset += 254;
            } else {
                lOffset += lFix;
                *(unsigned long*)&lpText[ lOffset ] += (unsigned long)lpText;
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

static ACU_init* load_cu(const char* cu_name)
{
    void* addr = NULL;
    long handle;
    long fileSize;
    
    handle = Fopen(cu_name, O_RDONLY);
    if (handle > 0)
    {
        PH programHeader;
        
        if (!Fread((int)handle, sizeof(PH), &programHeader) == sizeof(PH))
        {
            return NULL;
        }

        if (programHeader.ph_branch != PH_MAGIC) {
            Fclose((short)handle);
            return NULL;
        }
        fileSize = Fseek(0, (int) handle, 2);
        Fseek(sizeof(PH), (int) handle, 0);
            
        addr = load_and_reloc(handle, fileSize, &programHeader);
        Fclose((short)handle);
    }

    return (ACU_init*) addr;
}

ACU_Entry* cup_load(const char* cu_name) {
    ACU_init* init = load_cu(cu_name);
    ACU_Entry* entry;
    
    if (init == NULL) {
        return NULL;
    }
    entry = init();
    entry->setFrameStack(acu_getFrameStack());
    entry->setExit(exit);
    entry->cup_code = init;
    if (!checkVersion(entry)) {
        cup_unload(entry);
        return NULL;
    };
    return entry;
}

void cup_unload(ACU_Entry* entry) {
    free(entry->cup_code);
    acu_entryDestroy(entry);
}

#else

#include <windows.h>

ACU_Entry* cup_load(const char* cu_name) {
    wchar_t  name[256];
    swprintf(name, 255, L"%hs", cu_name);
    HMODULE module = LoadLibraryW(name);
    if (module == 0) {
        return NULL;
    }
    ACU_init* init = (ACU_init*) GetProcAddress(module, "acu_init");
    ACU_Entry* entry = init();
    entry->module = module;
    if (!checkVersion(entry)) {
        cup_unload(entry);
        return NULL;
    };
    return entry;
}

void cup_unload(ACU_Entry* entry) {
    FreeLibrary(entry->module);
    acu_entryDestroy(entry);
}

#endif

ACU_Entry* acu_entryMalloc(void) {
    return acu_emalloc(sizeof(ACU_Entry));
}

void acu_entryInit(ACU_Entry* entry, ACU_Suite* suite) {
    entry->suite = suite;
    entry->getAcuTestVersion = acu_getVersion;
#ifdef __TOS__
    entry->setFrameStack = acu_setFrameStack;
    entry->setExit = setExit;
#endif
}

void acu_entryDestroy(ACU_Entry* entry) {
    if (entry) {
        acu_suiteDestroy((ACU_Suite*)entry->suite);
        free(entry);
    }
}