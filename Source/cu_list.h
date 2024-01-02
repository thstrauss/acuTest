#pragma once
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct CU_ListElement_ {
	void* data;
	struct CU_ListElement_* next;
} CU_ListElement;

typedef struct CU_List_ {
	int size;

	void (*destroy)(void* data);

	CU_ListElement* head;
	CU_ListElement* tail;
} CU_List;

void cu_listInit(CU_List* list, void (*destroy)(void* data));

CU_ListElement* cu_listHead(CU_List* list);

CU_ListElement* cu_listNext(CU_ListElement* element);

void cu_listDestroy(CU_List* list);

int cu_listAppend(CU_List* list, void* data);

#endif 
