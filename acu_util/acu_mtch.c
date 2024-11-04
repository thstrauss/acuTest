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

typedef enum RegExType_ {CHAR, ANY, RANGE} RegExType;

typedef struct RegEx_{
    RegExType type;
    union {
        int matchChar;
    } charClass;
} RegEx;

int matchHere(const char* regexp, const char* text);

static int matchChar(const RegEx* regex, const char* text) {
    switch (regex->type)
    {
        case CHAR: return *text != '\0' && *text == regex->charClass.matchChar;
        case ANY: return *text != '\0';
        default:
            return 0;
    }
}

static int matchStar(int c, const char* regexp, const char* text) {
    RegEx regex;
    regex.type = CHAR;
    regex.charClass.matchChar = c;
    do {
        if (matchHere(regexp, text)) {
            return 1;
        }
    } while (matchChar(&regex, text++));
    return 0;
}

static int matchPlus(int c, const char* regexp, const char* text) {
    RegEx regex;
    regex.type = CHAR;
    regex.charClass.matchChar = c;
    return matchChar(&regex, text) && matchStar(c, regexp, text + 1);
}

static int matchQuery(int c, const char* regexp, const char* text) {
    RegEx regex;
    regex.type = CHAR;
    regex.charClass.matchChar = c;
    if (matchChar(&regex, text)) {
        return matchHere(regexp, text + 1);
    }
    return matchHere(regexp, text);
}

static int matchEscape(const char* regexp, const char* text) {
    return matchHere(regexp, text);
}

static int matchAny(const char* regexp, const char* text) {
    RegEx regex = {ANY};
    return matchChar(&regex, text) && matchHere(regexp+1, text + 1);
}

static int matchHere(const char* regexp, const char* text) {
    RegEx regex;
    if (regexp[0] == '\0') {
        return 1;
    }
    if (regexp[0] == '\\') {
        return matchEscape(regexp+1, text);
    }
    if (regexp[0] == '.') {
        return matchAny(regexp, text);
    }
    if (regexp[1] == '*') {
        return matchStar(regexp[0], regexp + 2, text);
    }    
    if (regexp[1] == '+') {
        return matchPlus(regexp[0], regexp + 2, text);
    }
    if (regexp[1] == '?') {
        return matchQuery(regexp[0], regexp + 2, text);
    }
    if (regexp[0] == '$' && regexp[1] == '\0') {
        return *text == '\0';
    }
    regex.type = CHAR;
    regex.charClass.matchChar = regexp[0];
    if (matchChar(&regex, text)) {
        return matchHere(regexp + 1, text + 1);
    }
    return 0;
}

int acu_match(const char* regexp, const char* text)
{
    if (regexp[0] == '^') {
        return matchHere(regexp + 1, text);
    }
    do {
        if (matchHere(regexp, text)) {
            return 1;
        }
    } while (*text++ != '\0');
    return 0;
}
