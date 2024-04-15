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

#include <acu_ldr.h>
#include <acu_util.h>

#ifdef __TOS__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <portab.h>
#include <tos.h>



#define PH_MAGIC 0x601a

typedef struct PH_
{
   WORD  ph_branch;        /* Branch zum Anfang des Programms  */
                           /* (muss 0x601a sein!)               */

   LONG  ph_tlen;          /* Länge  des TEXT - Segments       */
   LONG  ph_dlen;          /* Länge  des DATA - Segments       */
   LONG  ph_blen;          /* Länge  des BSS  - Segments       */
   LONG  ph_slen;          /* Länge  der Symboltabelle         */
   LONG  ph_res1;          /* reserviert, sollte 0 sein        */
                           /* wird von PureC benötigt          */
   LONG  ph_prgflags;      /* Programmflags                    */
   WORD  ph_absflag;       /* 0 = Relozierungsinf. vorhanden   */
} PH;

static void relocate(const void* code, const unsigned char* relocData)
{
    unsigned char *	lpText;
    unsigned char *	lpRelocTable;
    unsigned char		lFix;
    unsigned long		lOffset;

    lpText  = (unsigned char*) code;

    lpRelocTable  = (unsigned char*) relocData;

    lOffset = *(unsigned long*)lpRelocTable;
    lpRelocTable += sizeof(unsigned long*);

    if( lOffset )
    {
        *(unsigned long*)&lpText[ lOffset ] += (unsigned long)lpText;
        while( *lpRelocTable )
        {
            lFix = *lpRelocTable++;
            if( 1 == lFix )
            {
                lOffset += 254;
            }
            else
            {
                lOffset += lFix;
                *(unsigned long*)&lpText[ lOffset ] += (unsigned long)lpText;
            }
        };
    }
}

static void* load_and_reloc(long handle, long fsize, PH* phead)
{
    void* addr = NULL;
    long TD_len, TDB_len;

    /* Laenge von Text- und Data-Segment */
    TD_len = phead->ph_tlen + phead->ph_dlen;
    /* Laenge von Text-, Data- und BSS-Segment */
    TDB_len = TD_len + phead->ph_blen;
    
    /* Speicher fuer Text-, Data- und BSS-Segment anfordern */
    addr = malloc(TDB_len);
    if (addr)
    {
        long relo_len;

        /* Laenge der Relokationsdaten */
        relo_len = fsize - sizeof(PH) - TD_len - phead->ph_slen;

        /* Text- und Data-Segment laden */
        if (Fread((int)handle, TD_len, addr) == TD_len)
        {   
            /* Symboltabelle ueberspringen */
            Fseek(phead->ph_slen, (int)handle, 1);

            /* BSS-Segment loeschen */
            memset((char*)addr + TD_len, 0, phead->ph_blen);

            /* Datei relozieren */
            if ((phead->ph_absflag == 0) && relo_len)
            {
                unsigned char* relo_mem;

                /* Speicher fuer Relokationsdaten anfordern */
                relo_mem = malloc(relo_len);
                if (relo_mem)
                {
                    if (Fread((int)handle, relo_len, relo_mem) == relo_len)
                    {
                        relocate(addr, relo_mem);
                    }
                    else
                    {
                        free(addr);
                        addr = NULL;
                    }

                    /* Speicher fuer Relokationsdaten freigeben */
                    free(relo_mem);
                }
                else
                {
                    free(addr);
                    addr = NULL;
                }
            }
        }
        else
        {
            free(addr);
            addr = NULL;
        }
    }

    return addr;
}

static void* load_cu(const char* cu_name)
{
    void* addr = NULL;
    long handle;
    long filesize;
    
    handle = Fopen(cu_name, O_RDONLY);
    if (handle > 0)
    {
        PH phead;
        
        /* load program header */
        if (Fread((int)handle, sizeof(phead), &phead) == sizeof(phead))
        {
            /* bra.s am Anfang? */
            if (phead.ph_branch == PH_MAGIC)
                filesize = Fseek(0, (int) handle, 2);
                Fseek(sizeof(PH), (int) handle, 0);
            
                /* load and relocate */
                addr = load_and_reloc(handle, filesize, &phead);
        }
        Fclose((short)handle);
    } else {
        printf("%s not opened", cu_name);
    }

    return addr;
}

typedef ACU_Entry* ACU_init(void);

ACU_Entry* cup_load(const char* cu_name) {
	void* addr = load_cu(cu_name);
	ACU_init* init = addr;
	ACU_Entry* entry = init();
	entry->cup_code = addr;
	return entry;
}

void cup_unload(ACU_Entry* entry) {
	entry->destroy(entry->suite);
	free(entry->cup_code);
	free(entry);
}

#else

#include <windows.h>

typedef __EXPORT  ACU_Entry* (ACU_init)(void);

ACU_Entry* cup_load(const char* cu_name) {
    wchar_t  name[256];
    swprintf(name, 100, L"%hs", cu_name);
    HMODULE module = LoadLibraryW(name);
    if (module == 0) {
        acu_eprintf("Could not load: %s", cu_name);
        return NULL;
    }
    ACU_init* init = (ACU_init*) GetProcAddress(module, "acu_init");
    ACU_Entry* entry = init();
    entry->module = module;
    return entry;
}

void cup_unload(ACU_Entry* entry) {
    entry->destroy(entry->suite);
    FreeLibrary(entry->module);
}

#endif