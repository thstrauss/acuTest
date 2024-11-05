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

typedef enum RegExClass_ {CHAR, ANY, CLASSES } RegExClass;
typedef enum RegExOperation_ { SINGLE, STAR, PLUS, QUERY, START, END } RegExOperation;

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
        case CHAR: return *text != '\0' && *text == class->charClass.matchChar;
        case ANY: return *text != '\0';
        default:
            return 0;
    }
}

static int matchStar(const CharacterClass* class, ACU_ListElement* regEx, const char* text) {
    do {
        if (matchHere(regEx, text)) {
            return 1;
        }
    } while (matchClass(class, text++));
    return 0;
}

static int matchPlus(ACU_ListElement* regEx, const char* text) {
    return matchClass(&((RegEx*)regEx->data)->class, text) && matchStar(&((RegEx*)regEx->data)->class, regEx->next, text);
}

static int matchQuery(ACU_ListElement* regEx, const char* text) {
    if (matchClass(&((RegEx*)regEx->data)->class, text)) {
        return matchHere(regEx->next, text+1);
    }
    return matchHere(regEx->next, text);
}

static int matchHere(ACU_ListElement* regEx, const char* text) {
    if (regEx == NULL) {
        return 1;
    }
    if (((RegEx*) regEx->data)->type == STAR) {
        return matchStar(&((RegEx*)regEx->data)->class, regEx->next, text);
    }
    if (((RegEx*)regEx->data)->type == END) {
        return *text == '\0';
    }
    if (((RegEx*)regEx->data)->type == QUERY) {
        return matchQuery(regEx, text);
    }

    if (((RegEx*)regEx->data)->type == PLUS) {
        return matchPlus(regEx, text);
    }

    if (((RegEx*)regEx->data)->type == SINGLE && matchClass(&((RegEx*)regEx->data)->class, text)) {
        return matchHere(regEx->next, text + 1);
    }
    return 0;
}


static int compile(ACU_List* regexList, const char* regexp) {
    int c;
    int index = 0;
    while ((c = *regexp) != '\0') {
        RegEx* regEx = acu_emalloc(sizeof(RegEx));
        acu_appendList(regexList, regEx);
        regEx->type = SINGLE;
        regEx->class.type = CHAR;
        regEx->class.charClass.matchChar = c;
        if (c == '\\') {
            regexp++;
            if (*regexp != '\0') {
                regEx->class.charClass.matchChar = *regexp;
            }
        }
        else if (c == '.') {
            regEx->class.type = ANY;
        }
        else if (c == '^') {
            regEx->type = START;
        }
        else if (c == '$') {
            regEx->type = END;
        }
        regexp++;
        if (*regexp != '\0' && *regexp == '*') {
            regEx->type = STAR;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '+') {
            regEx->type = PLUS;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '?') {
            regEx->type = QUERY;
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
    ACU_List* regexList = acu_mallocList();
    acu_initList(regexList, __free);
    compile(regexList, regexp);

    if (((RegEx *) regexList->head->data)->type == START) {
        retval = matchHere(regexList->head->next, text);
    } else {
        do {
            if (matchHere(regexList->head, text)) {
                retval = 1;
                break;
            }
        } while (*text++ != '\0');
    }
    acu_destroyList(regexList);
    acu_free(regexList);
    return retval;
}
