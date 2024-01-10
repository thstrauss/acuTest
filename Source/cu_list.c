#include "cu_list.h"
#include "cu_utils.h"
#include <stdlib.h>
#include <string.h>

void cu_listInit(CU_List* list, void (*destroy)(void* data)) {
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;
}

CU_ListElement* cu_listHead(CU_List* list) {
	return list->head;
}

CU_ListElement* cu_listNext(CU_ListElement* element) {
	return element->next;
}

static int cu_listSize(CU_List* list) {
	return list->size;
}

int cu_listAppend(CU_List* list, void* data) {
	CU_ListElement* newElement;
	CU_ListElement* tailElement = list->tail;

	if ((newElement = (CU_ListElement*) cu_emalloc(sizeof(CU_ListElement))) == NULL) {
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

static int cu_listRemoveHead(CU_List* list, void** data) {
	CU_ListElement* oldElement;
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

void cu_listDestroy(CU_List* list) {
	void* data = NULL;

	while (cu_listSize(list) > 0) {
		if (cu_listRemoveHead(list, (void**)&data) == 0 && list->destroy != NULL) {
			list->destroy(data);
		}
	}
	memset(list, 0, sizeof(list));
}
