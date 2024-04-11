#include <acu_entr.h>
#include <acu_fxtr.h>
#include <acu_suit.h>

#include <fxtr_tst.h>
#include <flt_test.h>
#include <tryc_tst.h>

ACU_Entry entry;
ACU_Suite* suite;

static int execute(ACU_ProgressFunc progress) {
	return acu_suiteExecute(suite, progress);
}

static void report(void* context, ACU_ReportFunc report) {
	acu_suiteReport(suite, context, report);
}

static void fini(void) {
	acu_suiteDestroy(suite);
}

ACU_Entry* acu_init(void) {
    suite = acu_suiteMalloc();        acu_suiteInit(suite, "Suite");    acu_suiteAddFixture(suite, fixtureFixture());    acu_suiteAddFixture(suite, floatFixture());    acu_suiteAddFixture(suite, tryCatchFixture());
    
    entry.execute = execute;
    entry.report = report;
    entry.fini = fini;
    
	return &entry;
} 

