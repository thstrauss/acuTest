#include "misc_tst.h"

#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_perf.h>
#include <acu_cach.h>

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static float func2(float f1, ...) {
    va_list list;
    float f2;
    float f3;
    char* s;
    size_t len;
    va_start(list, f1);

    f2 = (float) va_arg(list, double);
    f3 = (float)va_arg(list, double);
    s = (char*)va_arg(list, char*);

    len = strlen(s);

    va_end(list);

    UNUSED(len);
    UNUSED(f3);
    UNUSED(f1);
    UNUSED(s);

    return f2;
}

static void floatInVaArgsTest(ACU_ExecuteEnv* environment, const void* context) {
    ACU_assert(environment, float, Equal, func2(1.0, 42.f, 43.f, "123"), 42.f, "float argument not handled correctly.");
    UNUSED(context);
}

void* p;

static void orphanedAlloc(ACU_ExecuteEnv* environment, const void* context) {
    p = acu_emalloc(1);
    UNUSED(environment);
    UNUSED(context);
}

static int zbyter(unsigned long x) {
    unsigned long y;

    y = (x & 0x7F7F7F7FL) + 0x7F7F7F7FL;
    y = ~(y | x | 0x7F7F7F7FL);        

    if (y == 0) {
        return 4;
    } 
    else if (y & 0x0000FFFFL) {
        return (int) ((y >> 7) & 1) ^ 1;
    }
    else {
        return (int) ((y >> 23) & 1) ^ 3;
    }
}


static int zbytel(unsigned long x) {
    unsigned long y;
    y = (x & 0x7F7F7F7FL) + 0x7F7F7F7FL;
    y = ~(y | x | 0x7F7F7F7FL);
    
    if (y == 0) return 4; 
    else if (y > 0x0000FFFFL) 
        return (int) (y >> 31) ^ 1; 
    else 
        return (int) (y >> 15) ^ 3;
}


static size_t fstrlen_C(const char* s) {
    const char* sPtr = s;
    unsigned long  magic = 0x7F7F7F7FL;

    unsigned long  l = *((const unsigned long *)sPtr);
    while (!~(((l & magic) + magic) | l | magic)) {
        sPtr += sizeof(unsigned long );
        l = *((const unsigned long *)sPtr);
        if (~(((l & magic) + magic) | l | magic)) {
            break;
        }
        sPtr += sizeof(unsigned long);
        l = *((const unsigned long*)sPtr);
        if (~(((l & magic) + magic) | l | magic)) {
            break;
        }
        sPtr += sizeof(unsigned long);
        l = *((const unsigned long*)sPtr);
    }
    while (*sPtr) {
        if (!*(++sPtr)) break;
        if (!*(++sPtr)) break;
        sPtr++;
    }
    return sPtr - s;
}

static void voidFunc(void) {
	int i;
	for (i=0; i<100; i++) {
	}
}

static void strLenFunc(void) {
	int i;
	for (i=0; i<100; i++) {
    	strlen("The quick brown fox jumps over the lazy dog");
    }
}

#ifdef __TOS__
extern size_t fstrlen(const char*);

static void fstrLenFunc(void) {
	int i;
	for (i=0; i<100; i++) {
		fstrlen("The quick brown fox jumps over the lazy dog");
    }
}
#endif

static void fstrlen_CFunc(void) {
    int i;
	for (i=0; i<100; i++) {
		fstrlen_C("The quick brown fox jumps over the lazy dog");
    }
}


static void strlenTest(ACU_ExecuteEnv* environment, const void* context) {

    #define DIVISOR 2
    printf("voidFunc\t%ld\n\r", (acu_measureLoop(voidFunc, CLK_TCK / DIVISOR))*DIVISOR);
    printf("strLenFunc\t%ld\n\r", (acu_measureLoop(strLenFunc, CLK_TCK / DIVISOR))*DIVISOR);
#ifdef __TOS__
    printf("fstrLenFunc\t%ld\n\r", (acu_measureLoop(fstrLenFunc, CLK_TCK / DIVISOR))*DIVISOR);
#endif
    printf("fstrlen_CFunc\t%ld\n\r", (acu_measureLoop(fstrlen_CFunc, CLK_TCK / DIVISOR))*DIVISOR);

    printf("%ld\n\r", (long) fstrlen_C("The quick brown fox jumps over the lazy dog"));

#ifdef __TOS__
    printf("%ld\n\r", fstrlen("The quick brown fox jumps over the lazy dog"));
#endif
    UNUSED(environment);
    UNUSED(context);
}

ACU_Fixture* miscFixture(void)
{
    ACU_Fixture* fixture = acu_mallocFixture();

    acu_initFixture(fixture, "mixed bag of tests");

    acu_addTestCase(fixture, "float In VaArgs are implicit converted to double", floatInVaArgsTest);
    acu_addTestCase(fixture, "float In VaArgs are implicit converted to double", orphanedAlloc);
    acu_addTestCase(fixture, "strlenTest", strlenTest);

    return fixture;
}
