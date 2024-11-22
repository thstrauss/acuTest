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

#include <stddef.h>
#include <stdio.h>

#include <acu_ldr.h>
#include <acu_fxtr.h>
#include <acu_rprt.h>
#include <acu_tryc.h>
#include <acu_stck.h>
#include <acu_util.h>

int main(void) {
	ACU_Entry* entry;
	ACU_Summary summary = {0,0};
	
	ACU_ReportVisitor report = {acu_report, NULL};
	ACU_ReportHelper reportHelper = {NULL, NULL};

	ACU_ReportVisitor counter = {acu_countTestCases, NULL};
	
	ACU_ReportVisitor reportSummary = {acu_reportSummary, NULL};
	
	int count=0;
	
	acu_enabledTrackMemory(1);
	entry = acu_init();
	counter.context = (void*) &count;
	report.context = &reportHelper;
	reportSummary.context = &summary;
	
	acu_acceptFixture(entry->fixture, &counter);
	fprintf(stdout, "count = %d \n\r", count);
	acu_executeFixture(entry->fixture, (ACU_Progress*) NULL);
	fprintf(stdout, "\n\r");
	acu_acceptFixture(entry->fixture, &report);
	acu_acceptFixture(entry->fixture, &reportSummary);
	acu_destroyEntry(entry);
	acu_free(entry);
	fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);
    acu_freeFrameStack();
    acu_reportTrackMemory();
    acu_enabledTrackMemory(0);
	return 0;
}
