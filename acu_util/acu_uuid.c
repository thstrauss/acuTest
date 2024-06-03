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

#include <time.h>

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

__EXPORT void acu_formatUuid(char* buffer, const ACU_UUID* uuid)
{
#define format(ii) \
    *buffer++ = nibbles[uuid->bytes[(ii)] >> 4]; \
    *buffer++ = nibbles[uuid->bytes[(ii++)] & 0x0f];

    static const char nibbles[] = "0123456789ABCDEF";
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

    *buffer++ = 0;
#undef format
}
