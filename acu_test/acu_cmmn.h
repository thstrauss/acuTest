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
#ifndef _ACU_COMMON_H_
#define _ACU_COMMON_H_

#ifdef __TOS__
#define __EXPORT 
#else
#define __EXPORT __declspec(dllexport) 
#endif

#ifdef __TOS__
#define __IMPORT 
#else
#define __IMPORT __declspec(dllimport) 
#endif

typedef enum ACU_TestResult_ {
    ACU_TEST_FAILED = 0,
    ACU_TEST_PASSED = 1,
    ACU_TEST_UNDEFINED = -1,
    ACU_TEST_ERROR = -2
} ACU_TestResult;

typedef enum ACU_Exception_ {
    ACU_TEST_ABORTED = 3
} ACU_Exception;

/*
* Can be used to suppress unused variable warnings.
*/
#define UNUSED(x) (void)(x);

#endif