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
#ifndef _ACU_SUITE_H_
#define _ACU_SUITE_H_

#include <time.h>

#include "acu_fxtr.h"
#include "acu_list.h"

 /* The ACU_Suite struct allows to group ACU_TestFixtures. */
typedef struct ACU_Suite_ {
    ACU_List* testFixtures;
    char* name;
    clock_t start;
    clock_t end;
} ACU_Suite;

/* Allocates an ACU_Suite. */
__EXPORT ACU_Suite* acu_suiteMalloc(void);

/* Initializes an ACU_Suite struct and sets the suite name. */
__EXPORT void acu_suiteInit(ACU_Suite* suite, const char* name);

/* Adds an ACU_Fixture to an ACU_Suite. */
__EXPORT void acu_suiteAddFixture(ACU_Suite* suite, ACU_Fixture* fixture);

/* Executes an ACU_Suite and reports progress. */
__EXPORT enum ACU_TestResult acu_suiteExecute(ACU_Suite* suite, ACU_Progress* progress);

/* Traverses an ACU_Suite with its added ACU_Fixture's. For each node (ACU_TestCase) the visitor is handled. */
__EXPORT void acu_suiteAccept(const ACU_Suite* suite, ACU_Visitor* visitor);

/* Releases all memory allocated by the ACU_Suite. */
__EXPORT void acu_suiteDestroy(ACU_Suite* suite);

#endif