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

__EXPORT ACU_UUID* acu_mallocUuid(void)
{
    return (ACU_UUID*) acu_emalloc(sizeof(ACU_UUID));
}

__EXPORT void acu_initUuid(ACU_UUID* uuid)
{
    static ACU_RandState state = {0,0,0,0};
    static int init = 0;
    int i = 3;

    if (!init) {
        init = !init;
        acu_srand(&state, clock());
    }
    while (i >= 0) {
        uuid->longs[i--] = acu_rand(&state);
    }
    uuid->bytes[6] = 0x40 | (uuid->bytes[6] & 0x0F);
    uuid->bytes[8] = 0xA0 | (uuid->bytes[8] & 0x3F);
}

__EXPORT int acu_compareUuid(const ACU_UUID* uuid1, const ACU_UUID* uuid2)
{
    return uuid1 && uuid2 && uuid1 == uuid2 && 
        uuid1->longs[0] == uuid2->longs[0] && 
        uuid1->longs[1] == uuid2->longs[1] &&
        uuid1->longs[2] == uuid2->longs[2] &&
        uuid1->longs[3] == uuid2->longs[3];
}

static const char nibbles[] = "0123456789ABCDEF";

__EXPORT void acu_formatUuid(char* buffer, const ACU_UUID* uuid)
{
#define format(ii) \
    *buffer++ = nibbles[uuid->bytes[(ii)] >> 4]; \
    *buffer++ = nibbles[uuid->bytes[(ii++)] & 0x0f];

    int i=0;

    format(i);
    format(i);
    format(i);
    format(i);
    *buffer++ = '-';
    format(i);
    format(i);
    *buffer++ = '-';
    format(i);
    format(i);
    *buffer++ = '-';
    format(i);
    format(i);
    *buffer++ = '-';
    format(i);
    format(i);
    format(i);
    format(i);
    format(i);
    format(i);

    *buffer = 0;
#undef format
}

static unsigned char HexChar(char c)
{
    if ('0' <= c && c <= '9') return (unsigned char)(c - '0');
    if ('A' <= c && c <= 'F') return (unsigned char)(c - 'A' + 10);
    if ('a' <= c && c <= 'f') return (unsigned char)(c - 'a' + 10);
    return 0xFF;
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
    if (acu_isValidUuid(buffer)) {
        uuid->bytes[0] = HexChar(*buffer++);
        uuid->bytes[0] = (uuid->bytes[0] << 4) + HexChar(*buffer++);
        uuid->bytes[1] = HexChar(*buffer++);
        uuid->bytes[1] = (uuid->bytes[1] << 4) + HexChar(*buffer++);
        uuid->bytes[2] = HexChar(*buffer++);
        uuid->bytes[2] = (uuid->bytes[2] << 4) + HexChar(*buffer++);
        uuid->bytes[3] = HexChar(*buffer++);
        uuid->bytes[3] = (uuid->bytes[3] << 4) + HexChar(*buffer++);
        buffer++;
        uuid->bytes[4] = HexChar(*buffer++);
        uuid->bytes[4] = (uuid->bytes[4] << 4) + HexChar(*buffer++);
        uuid->bytes[5] = HexChar(*buffer++);
        uuid->bytes[5] = (uuid->bytes[5] << 4) + HexChar(*buffer++);
        buffer++;
        uuid->bytes[6] = HexChar(*buffer++);
        uuid->bytes[6] = (uuid->bytes[6] << 4) + HexChar(*buffer++);
        uuid->bytes[7] = HexChar(*buffer++);
        uuid->bytes[7] = (uuid->bytes[7] << 4) + HexChar(*buffer++);
        buffer++;
        uuid->bytes[8] = HexChar(*buffer++);
        uuid->bytes[8] = (uuid->bytes[8] << 4) + HexChar(*buffer++);
        uuid->bytes[9] = HexChar(*buffer++);
        uuid->bytes[9] = (uuid->bytes[9] << 4) + HexChar(*buffer++);
        buffer++;
        uuid->bytes[10] = HexChar(*buffer++);
        uuid->bytes[10] = (uuid->bytes[10] << 4) + HexChar(*buffer++);
        uuid->bytes[11] = HexChar(*buffer++);
        uuid->bytes[11] = (uuid->bytes[11] << 4) + HexChar(*buffer++);
        uuid->bytes[12] = HexChar(*buffer++);
        uuid->bytes[12] = (uuid->bytes[12] << 4) + HexChar(*buffer++);
        uuid->bytes[13] = HexChar(*buffer++);
        uuid->bytes[13] = (uuid->bytes[13] << 4) + HexChar(*buffer++);
        uuid->bytes[14] = HexChar(*buffer++);
        uuid->bytes[14] = (uuid->bytes[14] << 4) + HexChar(*buffer++);
        uuid->bytes[15] = HexChar(*buffer++);
        uuid->bytes[15] = (uuid->bytes[15] << 4) + HexChar(*buffer++);
    }
    else {
        uuid->longs[0] = 0;
        uuid->longs[1] = 0;
        uuid->longs[2] = 0;
        uuid->longs[3] = 0;
    }
}
