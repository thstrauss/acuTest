#pragma once
#ifndef _ACU_FIXTURE_H_
#define _ACU_FIXTURE_H_

#include <stdio.h>

#include <acu_cmmn.h>
#include <acu_eenv.h>
#include <acu_rslt.h>
#include <acu_tcse.h>
#include <acu_rprt.h>

struct ACU_List_;
struct ACU_Suite_;

typedef struct ACU_Fixture_ {
    const char* name;
    const void* context;
    struct ACU_List_* testCases;
    struct ACU_Suite_* suite;
} ACU_Fixture;

void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char *name, ACU_TestFunc testFunc);

void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context);

void acu_fixtureInit(ACU_Fixture* fixture, const char* name);

int acu_fixtureExecute(ACU_Fixture* fixture, ACU_ProgressFunc progress);

void* acu_fixtureReport(ACU_Fixture* fixture, void* context, ACU_ReportFunc report);

ACU_Fixture* acu_fixtureMalloc(void);

void acu_fixtureDestroy(void* fixture);

#endif
