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
#include <string.h>

#include <acu_rprt.h>
#include <acu_cmmn.h>
#include <acu_ldr.h>
#include <acu_util.h>
#include <acu_dir.h>
#include <acu_tryc.h>
#include <acu_stck.h>

static void printHelp(void) {
    acu_eprintf("Shall provide file name for test.");
} 

typedef struct Summary_ {
    ACU_Summary summary;
    enum ACU_TestResult returnValue;
} Summary;

static void executeEntry(const char* cupName, Summary* summary) {
    static char buffer[] = "\n\r";
    ACU_Progress progress = { acu_progress , NULL };
    ACU_ReportHelper reportHelper = { NULL, NULL };
    ACU_ReportVisitor report = { acu_report , NULL };
    ACU_ReportVisitor reportSummary = { acu_reportSummary , NULL };
    ACU_Entry* entry;
    
    entry = cup_load(cupName);
    
    report.context = (void*) &reportHelper;
    
    reportSummary.context = (void*) &summary->summary;

    if (!entry) {
        acu_eprintf("Could not load: %s", cupName);
        summary->returnValue = ACU_TEST_UNDEFINED;
        return;
    }

    summary->returnValue = acu_fixtureExecute(entry->fixture, &progress);
    acu_printf_s(buffer, sizeof(buffer), "\n\r");
    acu_fixtureAccept(entry->fixture, &report);
    acu_fixtureAccept(entry->fixture, &reportSummary);

    cup_unload(entry);
}

static void execute(const ACU_FileEntry* file, void* context) {
    executeEntry(file->fileName, (Summary*) context);
}

int main(int argc, const char* argv[]) {
    Summary result = { {0,0}, 0 };
    static char buffer[256];
    ACU_Files* files;
    int allocs;

    ACU_FilesVisitor testExecuteVisitor = { execute , NULL };
    testExecuteVisitor.context = (void*) &result;

    if (argc < 2) {
        printHelp();
    }

    acu_enabledTrackMemory(1);

    files = acu_filesMalloc();
    acu_filesInit(files);

    acu_filesCollect(files, argv[1]);

    acu_filesAccept(files, &testExecuteVisitor);

    acu_filesDestroy(files);
    acu_free(files);

    acu_printf_s(buffer, sizeof(buffer), "%d of %d failed.\n\r", result.summary.failedTestCases, result.summary.totalTestCases);
    
    acu_freeFrameStack();

    acu_reportTrackMemory();

    acu_enabledTrackMemory(0);

    allocs = acu_getAllocCount();
    printf("\n\rallocs = %d", allocs);
    return result.returnValue == ACU_TEST_PASSED ? 0 : 2;
}
