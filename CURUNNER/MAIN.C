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

#include <stdio.h>

#include <acu_rprt.h>
#include <acu_cmmn.h>
#include <acu_ldr.h>
#include <acu_util.h>

#ifdef __TOS__
#define TEST_FILE "cutest.cup"
#else
#define TEST_FILE "cutest.dll"
#endif

int main() {
    int returnValue;
    ACU_Summary summary = { 0,0 };

    ACU_Entry* entry = cup_load(TEST_FILE);

    if (entry == NULL) {
        acu_eprintf("Could not load: %s", TEST_FILE);
        return;
    }

    returnValue = entry->execute(entry->suite, acu_progress) == ACU_TEST_PASSED ? 0 : 2;
    fprintf(stdout, "\n\r");

    entry->report(entry->suite, NULL, acu_report);
    entry->report(entry->suite, &summary, acu_reportSummary);

    cup_unload(entry);

    fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);

    return returnValue;
}