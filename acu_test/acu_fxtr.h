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

#include <stdio.h>
#include <time.h>

#include "acu_cmmn.h"
#include "acu_eenv.h"
#include "acu_rslt.h"
#include "acu_tcse.h"
#include "acu_rprt.h"

struct ACU_List_;
struct ACU_Suite_;

typedef struct ACU_Fixture_ {
    const char* name;
    const void* context;
    struct ACU_List_* testCases;
    struct ACU_Suite_* suite;
    clock_t start;
    clock_t end;
} ACU_Fixture;

__EXPORT void acu_fixtureAddTestCase(ACU_Fixture* fixture, const char *name, ACU_TestFunc testFunc);

__EXPORT void acu_fixtureSetContext(ACU_Fixture* fixture, const void* context);

__EXPORT void acu_fixtureInit(ACU_Fixture* fixture, const char* name);

__EXPORT enum ACU_TestResult acu_fixtureExecute(ACU_Fixture* fixture, ACU_ProgressFunc progress, void* progressContext);

__EXPORT void acu_fixtureAccept(const ACU_Fixture* fixture, ACU_VisitorFunc visitor, void* visitorContext);

__EXPORT ACU_Fixture* acu_fixtureMalloc(void);

__EXPORT void acu_fixtureDestroy(void* fixture);

#endif
