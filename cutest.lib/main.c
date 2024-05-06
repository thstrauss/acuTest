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

#include <acu_ldr.h>
#include <acu_suit.h>
#include <acu_rprt.h>
#include <acu_tryc.h>

int main(void) {
	ACU_Entry* entry = acu_init();
	ACU_Summary summary = {0,0};
	int count=0;
	
	acu_suiteAccept(entry->suite, acu_countTestCases, (void*) &count);
	fprintf(stdout, "count = %d \n\r", count);
	acu_suiteExecute(entry->suite, (ACU_VisitorFunc*) NULL, NULL);
	fprintf(stdout, "\n\r");
	acu_suiteAccept(entry->suite, acu_report, NULL);
	acu_suiteAccept(entry->suite, acu_reportSummary, &summary);
	acu_entryDestroy(entry);
	fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);
	return 0;
}
