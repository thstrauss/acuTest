#pragma once
#ifndef _CU_LIST_H_
#define _CU_LIST_H_

#include <stdlib.h>

typedef struct ACU_ListElement_ {
    void* data;
    struct ACU_ListElement_* next;
} ACU_ListElement;

typedef struct CU_List_ {
    int size;

    void (*destroy)(void* data);

    ACU_ListElement* head;
    ACU_ListElement* tail;
} CU_List;

void acu_listInit(CU_List* list, void (*destroy)(void* data));

ACU_ListElement* acu_listHead(CU_List* list);

ACU_ListElement* acu_listNext(ACU_ListElement* element);

void acu_listDestroy(CU_List* list);

int acu_listAppend(CU_List* list, void* data);

#endif 
