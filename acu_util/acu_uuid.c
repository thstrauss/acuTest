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

#include "acu_uuid.h"
#include "acu_rand.h"
#include "acu_cmmn.h"
#include "acu_util.h"


#include <ctype.h>
#include <string.h>
#include <time.h>

static ACU_UUID _NIL = { 0,0,0,0 };

ACU_UUID* acu_NIL_UUID = &_NIL;

static ACU_UUID _MAX = { 0xFFFFFFFFUL,0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL };

ACU_UUID* acu_MAX_UUID = &_MAX;

__EXPORT ACU_UUID* acu_mallocUuid(void)
{
    return (ACU_UUID*) acu_emalloc(sizeof(ACU_UUID));
}

__EXPORT void acu_initUuid(ACU_UUID* uuid)
{
    static ACU_RandState state;
    static int init = 1;
    int i = 3;

    if (init) {
        clock_t time = clock();
        init = !init;
        acu_srand(&state, time ? time : 123);
    }
    while (i >= 0) {
        uuid->longs[i--] = acu_rand(&state);
    }
    uuid->bytes[6] = 0x40 | (uuid->bytes[6] & 0x0F);
    uuid->bytes[8] = 0x80 | (uuid->bytes[8] & 0x3F);
}

__EXPORT int acu_compareUuid(const ACU_UUID* uuid1, const ACU_UUID* uuid2)
{
    return uuid1 && uuid2 && ((uuid1 == uuid2) || 
        (uuid1->longs[0] == uuid2->longs[0] && 
        uuid1->longs[1] == uuid2->longs[1] &&
        uuid1->longs[2] == uuid2->longs[2] &&
        uuid1->longs[3] == uuid2->longs[3]));
}

static const char nibbles[] = "0123456789ABCDEF";

__EXPORT void acu_formatUuid(char* buffer, const ACU_UUID* uuid)
{
#define __format(ii) \
    *buffer++ = nibbles[uuid->bytes[(ii)] >> 4]; \
    *buffer++ = nibbles[uuid->bytes[(ii++)] & 0x0f];

    int i=0;

    __format(i);
    __format(i);
    __format(i);
    __format(i);
    *buffer++ = '-';
    __format(i);
    __format(i);
    *buffer++ = '-';
    __format(i);
    __format(i);
    *buffer++ = '-';
    __format(i);
    __format(i);
    *buffer++ = '-';
    __format(i);
    __format(i);
    __format(i);
    __format(i);
    __format(i);
    __format(i);

    *buffer = 0;
#undef __format
}

static unsigned char acu_hexChar(unsigned char c)
{
    return 9 * (c >> 6) + (c & 0xf);
}

static int acu_isValidUuid(const char* buffer) {
    return buffer && strlen(buffer) == 36 &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        *buffer++ == '-' &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        *buffer++ == '-' &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        *buffer++ == '-' &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        *buffer++ == '-' &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer++) &&
        isxdigit(*buffer);
}

__EXPORT void acu_parseUuid(const char* buffer, ACU_UUID* uuid)
{
#define __parse(ii) \
    uuid->bytes[ii] = acu_hexChar(*buffer++); \
    uuid->bytes[ii] = (uuid->bytes[ii++] << 4) | acu_hexChar(*buffer++);

    if (acu_isValidUuid(buffer)) {
        int i = 0;
        __parse(i);
        __parse(i);
        __parse(i);
        __parse(i);
        buffer++;
        __parse(i);
        __parse(i);
        buffer++;
        __parse(i);
        __parse(i);
        buffer++;
        __parse(i);
        __parse(i);
        buffer++;
        __parse(i);
        __parse(i);
        __parse(i);
        __parse(i);
        __parse(i);
        __parse(i);
    } else {
        int i;
        for (i = 0; i < 4; i++) {
            uuid->longs[i] = _NIL.longs[i];
        }
    }
#undef __parse
}
