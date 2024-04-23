#include <acu_ldr.h>
#include <acu_suit.h>
#include <acu_rprt.h>

int main() {
	ACU_Entry* entry = acu_init();
	ACU_Summary summary = {0,0};
	
	acu_suiteExecute(entry->suite, acu_progress);
	acu_suiteReport(entry->suite, NULL, acu_report);
	acu_suiteReport(entry->suite, &summary, acu_reportSummary);
	acu_entryDestroy(entry);
	fprintf(stdout, "%d of %d failed.\n\r", summary.failedTestCases, summary.totalTestCases);
	return 0;
}