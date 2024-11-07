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

#pragma once
#ifndef _ACU_FIXTURE_H_
#define _ACU_FIXTURE_H_

#include <time.h>

#include "acu_eenv.h"
#include "acu_rslt.h"
#include "acu_tcse.h"
#include "acu_rprt.h"
#include <acu_util.h>

struct ACU_List_;

/* The ACU_Fixture allows to group ACU_TestCase's. */
typedef struct ACU_Fixture_ {
    struct ACU_List_* testCases;
    struct ACU_List_* childFixtures;
    char* name;
    const void* context;
    const struct ACU_Fixture_* parentFixture;
    clock_t duration;
} ACU_Fixture;

/* Allocates memory for an ACU_Fixture. */
#define acu_mallocFixture() ((ACU_Fixture*) acu_emalloc(sizeof(ACU_Fixture)))

/* Initializes an ACU_Fixture.*/
__EXPORT void acu_initFixture(ACU_Fixture* fixture, const char* name);

__EXPORT void acu_fixtureDestroy(ACU_Fixture* fixture);

__EXPORT void acu_addTestCase(ACU_Fixture* fixture, const char *name, ACU_TestFunc testFunc);

__EXPORT void acu_addChildFixture(ACU_Fixture* fixture, ACU_Fixture* childFixture);

__EXPORT void acu_setFixtureContext(ACU_Fixture* fixture, const void* context);

__EXPORT enum ACU_TestResult acu_executeFixture(ACU_Fixture* fixture, ACU_Progress* progress);

__EXPORT void acu_acceptFixture(const ACU_Fixture* fixture, ACU_ReportVisitor* visitor);

#endif
