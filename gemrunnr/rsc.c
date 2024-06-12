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

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <aes.h>
#include <vdi.h>

#include <portab.h>

#ifndef _WORD
#define _WORD	WORD
#endif
#ifndef _UWORD
#define _UWORD	UWORD
#endif
#ifndef _LONG
#define _LONG	LONG
#endif
#ifndef _ULONG
#define _ULONG	ULONG
#endif
#ifndef _BOOL
#define _BOOL int
#endif


#define RSC_NAMED_FUNCTIONS 1

#include "gemrunnr.h" 

struct userdef {
	OBJECT orig;
	USERBLK user;
	_WORD obj;
};

struct userdef_list {
	struct userdef_list *next;
	RSHDR *hdr;
	OBJECT *objects;
	_WORD nobjs;
	_WORD nuserobjs;
	struct userdef userobjs[1];
};

/*
 * This function is called by inlined sources
 */
void *hfix_objs(RSHDR *hdr, OBJECT *objects, _WORD num_objs)
{
	(void) hdr;
	(void) objects;
	(void) num_objs;
	return NULL;
}

void hrelease_objs(OBJECT* rs_object, _WORD numObjects) {
	(void) rs_object;
	(void) numObjects;
}

#include "gemrunnr.rsh"