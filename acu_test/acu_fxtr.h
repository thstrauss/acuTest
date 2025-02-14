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

/* The ACU_Fixture structure allows to group ACU_TestCase's and to hold a hierarchy of fixtures. */
typedef struct ACU_Fixture_ {
    struct ACU_List_* testCases;
    struct ACU_List_* childFixtures;
    char* name;
    const void* context;
    const struct ACU_Fixture_* parentFixture;
    clock_t duration;
} ACU_Fixture;

/* Allocates memory for an ACU_Fixture. 
* @return The allocated memory.
* @remark The memory must be freed with acu_freeFixture.
*/
#define acu_mallocFixture() ((ACU_Fixture*) acu_emalloc(sizeof(ACU_Fixture)))

/* Initializes an ACU_Fixture.
* @param fixture The fixture to initialize.
* @param name The name of the fixture.
*/
__EXPORT void acu_initFixture(ACU_Fixture* fixture, const char* name);

/* Destroys an ACU_Fixture. 
* @param fixture The fixture to destroy.
*/
__EXPORT void acu_fixtureDestroy(ACU_Fixture* fixture);

/* Adds a test case to the fixture. 
 * @param fixture The fixture to add the test case to.
 * @param name The name of the test case.
 * @param testFunc The function to execute the test case.
 */ 
__EXPORT void acu_addTestCase(ACU_Fixture* fixture, const char *name, ACU_TestFunc testFunc);

/* Adds a child fixture to the fixture.
 * @param fixture The fixture to add the child fixture to.
 * @param childFixture The child fixture to add.
 */ 
__EXPORT void acu_addChildFixture(ACU_Fixture* fixture, ACU_Fixture* childFixture);

/* Sets the context for the fixture. The context is passed to the test cases when executed.
 * @param fixture The fixture to set the context for.
 * @param context The context to set.
 */
__EXPORT void acu_setFixtureContext(ACU_Fixture* fixture, const void* context);

/* Executes the fixture. 
 * @param fixture The fixture to execute.
 * @param progress The progress to report to.
 */
__EXPORT enum ACU_TestResult acu_executeFixture(ACU_Fixture* fixture, ACU_Progress* progress);

/* Accepts a fixture. All Nodes of the fixture are visited.
 * @param fixture The fixture to accept.
 * @param visitor The visitor to accept the fixture.
 */ 
__EXPORT void acu_acceptFixture(const ACU_Fixture* fixture, ACU_ReportVisitor* visitor);

#endif
