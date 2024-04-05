#include <acu_fxtr.h>
#include <acu_asrt.h>
#include <acu_suit.h>
#include <acu_util.h>
#include <acu_rslt.h>

#include <tryctch.h>

#include "tryc_tst.h"

static void tryETryTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    TRY
        visited = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block not visited");
    UNUSED(context);
}

static void tryCatchNotVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    TRY
        visited = 1;
    CATCH(1)
        catched = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block not visited");
    ACU_assert(environment, int, Equal, catched, 0, "catch not visited");
    UNUSED(context);
}

static void tryCatchVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    TRY
        visited++;
        THROW(1);
        visited++;
    CATCH(1)
        catched = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    UNUSED(context);
}

static void tryCatchVisitedExpandedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    do {
        jmp_buf _exception_Buf; switch (_setjmp(_exception_Buf)) {
        case 0: while (1) {
            visited++;
            longjmp(_exception_Buf, (1));
            visited++;
        break; case (1):
            catched = 1;
            break;
        }
        }
    } while (0);
    ACU_assert(environment, int, Equal, visited, 1, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    UNUSED(context);
}


static void tryCatchFinallyVisitedTests(ACU_ExecuteEnv* environment, const void* context) {
    int visited = 0;
    int catched = 0;
    int finally = 0;
    TRY
        THROW(1);
        visited = 1;
    CATCH(1)
        catched = 1;
    FINALLY
        finally = 1;
    ETRY;
    ACU_assert(environment, int, Equal, visited, 0, "block visited");
    ACU_assert(environment, int, Equal, catched, 1, "catch not visited");
    ACU_assert(environment, int, Equal, finally, 1, "finally not visited");
    UNUSED(context);
}

void tryCatchFixture(ACU_Suite* suite)
{
    ACU_Fixture* fixture = acu_emalloc(sizeof(ACU_Fixture));
    acu_suiteAddFixture(suite, fixture);
    acu_fixtureInit(fixture, "try catch finally");

    acu_fixtureAddTestCase(fixture, "try etry", tryETryTests);
    acu_fixtureAddTestCase(fixture, "try catch", tryCatchNotVisitedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch", tryCatchVisitedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch_ expanded", tryCatchVisitedExpandedTests);
    acu_fixtureAddTestCase(fixture, "try throw catch finally", tryCatchFinallyVisitedTests);
}
