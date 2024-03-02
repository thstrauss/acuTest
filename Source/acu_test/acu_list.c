#include <stdlib.h>
#include <string.h>

#include <acu_list.h>
#include <acu_util.h>

void acu_listInit(ACU_List* list, void (*destroy)(void* data)) {
    list->size = 0;
    list->destroy = destroy;
    list->head = NULL;
    list->tail = NULL;
}

ACU_ListElement* acu_listHead(ACU_List* list) {
    return list->head;
}

ACU_ListElement* acu_listNext(ACU_ListElement* element) {
    return element->next;
}

static int acu_listSize(ACU_List* list) {
    return list->size;
}

int acu_listAppend(ACU_List* list, void* data) {
    ACU_ListElement* newElement;
    ACU_ListElement* tailElement = list->tail;

    if ((newElement = (ACU_ListElement*) acu_emalloc(sizeof(ACU_ListElement))) == NULL) {
        return -1;
    }
    newElement->next = NULL; 
    newElement->data = data;

    if (acu_listSize(list) == 0) {
        list->head = newElement;
    }
    
    if (tailElement != NULL) {
        tailElement->next = newElement;
    }
    list->tail = newElement;

    list->size++;

    return 0;
}

static int acu_listRemoveHead(ACU_List* list, void** data) {
    ACU_ListElement* oldElement;
    if (acu_listSize(list) <= 0) {
        return -1;
    }
    
    *data = list->head->data;
    oldElement = list->head;
    list->head = list->head->next;
    if (acu_listSize(list) == 1) {
        list->tail = NULL;
    }

    free(oldElement);

    list->size--;
    
    return 0;
}

void acu_listDestroy(ACU_List* list) {
    void* data = NULL;

    while (acu_listSize(list) > 0) {
        if (acu_listRemoveHead(list, (void**)&data) == 0 && list->destroy != NULL) {
            list->destroy(data);
        }
    }
    memset(list, 0, sizeof(list));
}
