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