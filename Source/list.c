#include "list.h"

#include <stdlib.h>
#include <string.h>

void cu_listInit(List* list, void (*destroy)(void* data)) {
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

ListElement* cu_listHead(List* list) {
	return list->head;
}

ListElement* cu_listNext(ListElement* element) {
	return element->next;
}

static int cu_listSize(List* list) {
	return list->size;
}

int cu_listAppend(List* list, void* data) {
	ListElement* newElement;
	ListElement* tailElement = list->tail;

	if ((newElement = (ListElement*)malloc(sizeof(ListElement))) == NULL) {
		return -1;
	}
	newElement->next = NULL; 
	newElement->data = data;

	if (cu_listSize(list) == 0) {
		list->head = newElement;
	}
	
	if (tailElement != NULL) {
		tailElement->next = newElement;
	}
	list->tail = newElement;

	list->size++;

	return 0;
}

static int cu_listRemoveHead(List* list, void** data) {
	ListElement* oldElement;
	if (cu_listSize(list) <= 0) {
		return -1;
	}
	
	*data = list->head->data;
	oldElement = list->head;
	list->head = list->head->next;
	if (cu_listSize(list) == 1) {
		list->tail = NULL;
	}

	free(oldElement);

	list->size--;
	
	return 0;
}

void cu_listDestroy(List* list) {
	void* data;

	while (cu_listSize(list) > 0) {
		if (cu_listRemoveHead(list, (void**)&data) == 0 && list->destroy != NULL) {
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(list));
}
