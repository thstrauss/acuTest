#ifndef __entry__
#define __entry__

#include <acu_rprt.h>

typedef int ACU_Execute(ACU_ProgressFunc progress);
typedef void ACU_Report(void* context, ACU_ReportFunc report);
typedef void ACU_Fini(void);

typedef struct ACU_Entry_ {
	ACU_Execute* execute;
	ACU_Report* report;
	ACU_Fini* fini;
} ACU_Entry;

ACU_Entry* acu_init(void);
void acu_fini(void);

#endif