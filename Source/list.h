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

void listInit(List* list, void (*destroy)(void* data));

ListElement* listHead(List* list);

ListElement* listNext(ListElement* element);

void listDestroy(List* list);

int listInsertTail(List* list, void* data);

#endif 
