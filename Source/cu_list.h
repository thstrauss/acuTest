#pragma once
#ifndef _CU_LIST_H_
#define _CU_LIST_H_

#include <stdlib.h>

typedef struct ACU_ListElement_ {
    void* data;
    struct ACU_ListElement_* next;
} ACU_ListElement;

typedef struct ACU_List_ {
    int size;

    void (*destroy)(void* data);

    ACU_ListElement* head;
    ACU_ListElement* tail;
} ACU_List;

void acu_listInit(ACU_List* list, void (*destroy)(void* data));

ACU_ListElement* acu_listHead(ACU_List* list);

ACU_ListElement* acu_listNext(ACU_ListElement* element);

void acu_listDestroy(ACU_List* list);

int acu_listAppend(ACU_List* list, void* data);

#endif 
