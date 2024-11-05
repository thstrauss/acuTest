/*
 * Copyright (c) 2024 Thomas Strauss
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall 
 * be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "acu_mtch.h"

#include "acu_list.h"
#include "acu_util.h"
#include <stdlib.h>

typedef enum RegExClass_ { CHAR_CLASS, ANY_CLASS } RegExClass;
typedef enum RegExOperation_ { SINGLE_OP, STAR_OP, PLUS_OP, QUERY_OP, START_OP, END_OP } RegExOperation;

typedef struct CharacterClass_ {
    RegExClass type;
    union {
        int matchChar;
    } charClass;
} CharacterClass;

typedef struct RegEx_{
    RegExOperation type;
    CharacterClass class;
} RegEx;

int matchHere(ACU_ListElement* regEx, const char* text);

static int matchClass(const CharacterClass* class, const char* text) {
    switch (class->type)
    {
        case CHAR_CLASS: return *text != '\0' && *text == class->charClass.matchChar;
        case ANY_CLASS: return *text != '\0';
        default:
            return 0;
    }
}

static int matchStar(const CharacterClass* class, ACU_ListElement* regExElement, const char* text) {
    do {
        if (matchHere(regExElement, text)) {
            return 1;
        }
    } while (matchClass(class, text++));
    return 0;
}

static int matchPlus(ACU_ListElement* regExElement, const char* text) {
    RegEx* r = (RegEx*)regExElement->data;
    return matchClass(&r->class, text) && matchStar(&r->class, regExElement->next, text);
}

static int matchQuery(ACU_ListElement* regExElement, const char* text) {
    if (matchClass(&((RegEx*)regExElement->data)->class, text)) {
        return matchHere(regExElement->next, text+1);
    }
    return matchHere(regExElement->next, text);
}

static int matchHere(ACU_ListElement* regExElement, const char* text) {
    RegEx* r; 
    if (regExElement == NULL) {
        return 1;
    }
    r = (RegEx*)regExElement->data;
    if (r->type == STAR_OP) {
        return matchStar(&r->class, regExElement->next, text);
    }
    if (r->type == END_OP) {
        return *text == '\0';
    }
    if (r->type == QUERY_OP) {
        return matchQuery(regExElement, text);
    }

    if (r->type == PLUS_OP) {
        return matchPlus(regExElement, text);
    }

    if (r->type == SINGLE_OP && matchClass(&r->class, text)) {
        return matchHere(regExElement->next, text + 1);
    }
    return 0;
}


static int compile(ACU_List* regexpList, const char* regexp) {
    int c;
    int index = 0;
    while ((c = *regexp) != '\0') {
        RegEx* regEx = acu_emalloc(sizeof(RegEx));
        acu_appendList(regexpList, regEx);
        regEx->type = SINGLE_OP;
        if (c == '\\') {
            regexp++;
            if (*regexp != '\0') {
                regEx->class.type = CHAR_CLASS;
                regEx->class.charClass.matchChar = *regexp;
            }
        }
        else if (c == '.') {
            regEx->class.type = ANY_CLASS;
        }
        else if (c == '^') {
            regEx->type = START_OP;
        }
        else if (c == '$') {
            regEx->type = END_OP;
        }
        else {
            regEx->class.type = CHAR_CLASS;
            regEx->class.charClass.matchChar = c;
        }
        regexp++;
        if (*regexp != '\0' && *regexp == '*') {
            regEx->type = STAR_OP;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '+') {
            regEx->type = PLUS_OP;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '?') {
            regEx->type = QUERY_OP;
            regexp++;
        }
        index++;
    }
    return index;
}

static void __free(void* data) {
    acu_free(data);
}

int acu_match(const char* regexp, const char* text)
{
    int retval = 0;
    ACU_List* regexpList = acu_mallocList();
    acu_initList(regexpList, __free);
    compile(regexpList, regexp);

    if (((RegEx *) regexpList->head->data)->type == START_OP) {
        retval = matchHere(regexpList->head->next, text);
    } else {
        do {
            if (matchHere(regexpList->head, text)) {
                retval = 1;
                break;
            }
        } while (*text++ != '\0');
    }
    acu_destroyList(regexpList);
    acu_free(regexpList);
    return retval;
}
