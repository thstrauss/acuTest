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

static void printHelp(void) {
    acu_eprintf("Shall provide file name for test.");
} 

static int executeEntry(const char* cupName, const ACU_Summary* summary) {
    int returnValue;
    ACU_Entry* entry = cup_load(cupName);

    if (entry == NULL) {
        acu_eprintf("Could not load: %s", cupName);
        return 2;
    }

    returnValue = acu_suiteExecute(entry->suite, acu_progress, NULL);
    fprintf(stdout, "\n\r");
    acu_suiteAccept(entry->suite,acu_report, NULL);
    acu_suiteAccept(entry->suite, acu_reportSummary, (void*)summary);

    cup_unload(entry);
    return returnValue;
}

static void execute(const ACU_FileEntry* file, void* context) {
	executeEntry(file->fileName, (ACU_Summary*) context);
}

static int executeEntries(const char** testFiles, const ACU_Summary* summary) {
    int returnValue = ACU_TEST_PASSED;
    int fileIndex = 0;
    while (testFiles[fileIndex] != NULL) {
        int retValue = executeEntry(testFiles[fileIndex++], summary);
        returnValue = returnValue | retValue;
    }
    return returnValue;
}

int main(int argc, const char* argv[]) {
    int returnValue = 0;
    ACU_Summary summary = { 0,0 };

    ACU_Files* files;

    if (argc < 2) {
        printHelp();
    }

    files = acu_filesMalloc();
    acu_filesInit(files);

    acu_filesCollect(files, argv[1]);

	acu_filesAccept(files, execute, &summary);

    acu_filesDestroy(files);

    fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);

    return returnValue;
}