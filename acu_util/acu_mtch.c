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
#include "acu_strg.h"

typedef enum RegExClass_ { CHAR_CLASS, ANY_CLASS, CLASS_CLASS } RegExClass;
typedef enum RegExOperation_ { SINGLE_OP, STAR_OP, PLUS_OP, QUERY_OP, START_OP, END_OP } RegExOperation;

typedef struct CharacterClass_ {
    RegExClass type;
    union {
        int matchChar;
        char* class;
    } charClass;
} CharacterClass;

typedef struct RegExp_{
    RegExOperation type;
    CharacterClass class;
} RegExp;

int matchHere(ACU_ListElement* regEx, const char* text);

static int matchClass(const CharacterClass* class, const char* text) {
    switch (class->type)
    {
        case CHAR_CLASS: return *text != '\0' && *text == class->charClass.matchChar;
        case ANY_CLASS: return *text != '\0';
        case CLASS_CLASS: {
            char* c = class->charClass.class;
            int ac;
            while ((ac = *c++) != '\0') {
                if (ac == *text) {
                    return 1;
                }
            }
            return 0;
        }
        default:
            return 0;
    }
}

static int matchHere(ACU_ListElement* regExpElement, const char* text) {
    while (regExpElement != NULL) {
        RegExp* regExp = (RegExp*)regExpElement->data;
        if (regExp->type == STAR_OP) {
            RegExp* nextRegExp = (RegExp*)regExpElement->next->data;
            while (matchClass(&regExp->class, text)) {
                if (matchClass(&nextRegExp->class, text + 1)) {
                    text++;
                    break;
                }
                text++;
            }
            regExpElement = regExpElement->next;
            continue;
        }
        if (regExp->type == PLUS_OP && matchClass(&regExp->class, text++)) {
            RegExp* nextRegExp = (RegExp*) regExpElement->next->data;
            while (matchClass(&regExp->class, text)) {
                if (matchClass(&nextRegExp->class, text + 1)) {
                    text++;
                    break;
                }
                text++;
            }
            regExpElement = regExpElement->next;
            continue;
        }
        if (regExp->type == QUERY_OP) {
            if (matchClass(&regExp->class, text)) {
                text++;
            }
            regExpElement = regExpElement->next;
            continue;
        }
        if (regExp->type == END_OP) {
            return *text == '\0';
        }
        if (regExp->type == SINGLE_OP && !matchClass(&regExp->class, text)) {
            return 0;
        }
        text++;
        regExpElement = regExpElement->next;
    }
    return 1;
}

static void compile(ACU_List* regexpList, const char* regexp) {
    int c;
    while ((c = *regexp) != '\0') {
        RegExp* regEx = acu_emalloc(sizeof(RegExp));
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
        else if (c == '[') {
        	const char* start;
            size_t size;
        	regexp++;
            start = regexp;
            while (*regexp != ']') {
            	regexp++;
            }
            size = regexp - start;
            regEx->class.charClass.class = acu_emalloc(size+1);
            acu_strncpy(regEx->class.charClass.class, start, size);
            *(regEx->class.charClass.class + size) = '\0';
            regEx->class.type = CLASS_CLASS;
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
    }
}

static void __free(void* data) {
    if (((RegExp*)data)->class.type == CLASS_CLASS) {
        acu_free(((RegExp*)data)->class.charClass.class);
    }
    acu_free(data);
}

int acu_match(const char* regexp, const char* text)
{
    int retval = 0;
    ACU_List* regexpList = acu_mallocList();
    acu_initList(regexpList, __free, NULL);
    compile(regexpList, regexp);

    if (((RegExp*) regexpList->head->data)->type == START_OP) {
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
