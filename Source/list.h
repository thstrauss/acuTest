#pragma once
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct ListElement_ {
	void* data;
	struct ListElement_* next;
} ListElement;

typedef struct List_ {
	int size;

	void (*destroy)(void* data);

	ListElement* head;
	ListElement* tail;
} List;

void cu_listInit(List* list, void (*destroy)(void* data));

ListElement* cu_listHead(List* list);

ListElement* cu_listNext(ListElement* element);

void cu_listDestroy(List* list);

int cu_listAppend(List* list, void* data);

#endif 
