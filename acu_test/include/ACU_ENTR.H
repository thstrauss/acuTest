#ifndef __entry__
#define __entry__

#include <acu_suit.h>

struct ACU_Suite_;


typedef struct ACU_Entry_ {
	ACU_SuiteExecuteFunc* execute;
	ACU_SuiteReportFunc* report;
	ACU_SuiteDestroyFunc* destroy;
	struct ACU_Suite_* suite;
} ACU_Entry;

ACU_Entry* acu_init(void);
void __exit(int status);

typedef ACU_Entry* ACU_init(void);

#endif