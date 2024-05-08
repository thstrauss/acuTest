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

#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "acu_vers.h"
#include "acu_util.h"

static ACU_Version acuLibraryVersion = { 0,0,1,0 };

ACU_Version* acu_getVersion(void)
{
    return &acuLibraryVersion;
}

int acu_compareVersion(const ACU_Version* version1, const ACU_Version* version2)
{
    return version1->version[0] == version2->version[0] 
    	&& version1->version[1] == version2->version[1] 
    	&& version1->version[2] == version2->version[2];
}

char* acu_formatVersion(const ACU_Version* version) {
	char* buffer = acu_emalloc(8);
	acu_sprintf_s(buffer, 8, "%u.%u.%u.%u", 
		version->version[0], 
		version->version[1], 
		version->version[2], 
		version->version[3]);
    return buffer;
}
