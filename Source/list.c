#include "list.h"

#include <stdlib.h>
#include <string.h>

void listInit(List* list, void (*destroy)(void* data)) {
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

ListElement* listHead(List* list) {
	return list->head;
}

ListElement* listNext(ListElement* element) {
	return element->next;
}

static int listSize(List* list) {
	return list->size;
}

int listInsertTail(List* list, void* data) {
	ListElement* newElement;
	ListElement* tailElement = list->tail;

	if ((newElement = (ListElement*)malloc(sizeof(ListElement))) == NULL) {
		return -1;
	}
	newElement->next = NULL; 
	newElement->data = data;

	if (listSize(list) == 0) {
		list->head = newElement;
	}
	
	if (tailElement != NULL) {
		tailElement->next = newElement;
	}
	list->tail = newElement;

	list->size++;

	return 0;
}

static int listRemoveHead(List* list, void** data) {
	ListElement* oldElement;
	if (listSize(list) <= 0) {
		return -1;
	}
	
	*data = list->head->data;
	oldElement = list->head;
	list->head = list->head->next;
	if (listSize(list) == 1) {
		list->tail = NULL;
	}

	free(oldElement);

	list->size--;
	
	return 0;
}

void listDestroy(List* list) {
	void* data;

	while (listSize(list) > 0) {
		if (listRemoveHead(list, (void**)&data) == 0 && list->destroy != NULL) {
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(list));
}
