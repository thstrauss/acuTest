#include <acu_ldr.h>
#include <acu_suit.h>
#include <acu_rprt.h>

int main() {
	ACU_Entry* entry = acu_init();
	ACU_Summary summary = {0,0};
	
	acu_suiteExecute(entry->suite, acu_progress, NULL);
	acu_suiteReport(entry->suite, acu_report, NULL);
	acu_suiteReport(entry->suite, acu_reportSummary, &summary);
	acu_entryDestroy(entry);
	fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);
	return 0;
}