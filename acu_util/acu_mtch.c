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

typedef enum RegExClass_ {CHAR, ANY, RANGE} RegExClass;
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

static RegEx regEx[100];
static int maxRegEx;


int matchHere(int index, const char* text);

static int matchClass(const CharacterClass* class, const char* text) {
    switch (class->type)
    {
        case CHAR: return *text != '\0' && *text == class->charClass.matchChar;
        case ANY: return *text != '\0';
        default:
            return 0;
    }
}

static int matchStar(const CharacterClass* class, int index, const char* text) {
    do {
        if (matchHere(index, text)) {
            return 1;
        }
    } while (matchClass(class, text++));
    return 0;
}

static int matchPlus(int index, const char* text) {
    return matchClass(&regEx[index].class, text) && matchStar(&regEx[index].class, index+1, text);
}

static int matchQuery(int index, const char* text) {
    if (matchClass(&regEx[index].class, text)) {
        return matchHere(index+1, text+1);
    }
    return matchHere(index+1, text);
}

static int matchHere(int index, const char* text) {
    if (index == maxRegEx) {
        return 1;
    }
    if (regEx[index].type == STAR) {
        return matchStar(&regEx[index].class, index+1, text);
    }
    if (regEx[index].type == END) {
        return *text == '\0';
    }
    if (regEx[index].type == QUERY) {
        return matchQuery(index, text);
    }

    if (regEx[index].type == PLUS) {
        return matchPlus(index, text);
    }

    if (regEx[index].type == SINGLE && matchClass(&regEx[index].class, text)) {
        return matchHere(index+1, text + 1);
    }
    return 0;
}


static int compile(const char* regexp) {
    int c;
    int index = 0;
    while ((c = *regexp) != '\0') {
        regEx[index].type = SINGLE;
        regEx[index].class.type = CHAR;
        regEx[index].class.charClass.matchChar = c;
        if (c == '\\') {
            regexp++;
            if (*regexp != '\0') {
                regEx[index].class.charClass.matchChar = *regexp;
            }
        }
        else if (c == '.') {
            regEx[index].class.type = ANY;
        }
        else if (c == '^') {
            regEx[index].type = START;
        }
        else if (c == '$') {
            regEx[index].type = END;
        }
        regexp++;
        if (*regexp != '\0' && *regexp == '*') {
            regEx[index].type = STAR;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '+') {
            regEx[index].type = PLUS;
            regexp++;
        }
        else if (*regexp != '\0' && *regexp == '?') {
            regEx[index].type = QUERY;
            regexp++;
        }
        index++;
    }
    return index;
}

int acu_match(const char* regexp, const char* text)
{
    maxRegEx = compile(regexp);

    if (regEx[0].type == START) {
        return matchHere(1, text);
    }
    do {
        if (matchHere(0, text)) {
            return 1;
        }
    } while (*text++ != '\0');
    return 0;
}
