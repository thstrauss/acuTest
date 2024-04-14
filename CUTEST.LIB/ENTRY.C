#include <acu_entr.h>
#include <acu_fxtr.h>
#include <acu_suit.h>

#include "fxtr_tst.h"
#include "flt_test.h"
#include "tryc_tst.h"

ACU_Entry entry;

ACU_Entry* acu_init(void) {
	ACU_Suite* suite;
    suite = acu_suiteMalloc();        acu_suiteInit(suite, "Suite");    acu_suiteAddFixture(suite, fixtureFixture());     acu_suiteAddFixture(suite, floatFixture());    acu_suiteAddFixture(suite, tryCatchFixture());
    
    entry.execute = acu_suiteExecute;
    entry.report = acu_suiteReport;
    entry.destroy = acu_suiteDestroy;
    entry.suite = suite;
    
	return &entry;
}

void __exit(int status) {
	exit(status);
}

