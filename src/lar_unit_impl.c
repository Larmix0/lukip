#include <stdio.h>
#include <stdlib.h>

#include "lar_unit.h"
#include "lar_unit_impl.h"

#define GROW_CAPACITY(capacity) ((capacity) < 16 ? 16 : (capacity) * 2)

LarUnit larUnit;

static void *allocate(void *pointer, const int newSize, const size_t elementSize) {
    void *result = realloc(pointer, newSize * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

void init_lar_unit() {
    larUnit.testsCapacity = 0;
    larUnit.testsLength = 0;
    larUnit.tests = NULL;

    larUnit.setUp = NULL;
    larUnit.tearDown = NULL;
}

static void init_info(TestInfo *info) {
    info->status = UNKNOWN;
    info->fileName = NULL;
    info->funcName = NULL;
}

static void init_test(TestFunc *test) {
    test->failsCapacity = 0;
    test->failsLength = 0;
    test->failures = NULL;
    test->testFunc = NULL;
    init_info(&test->info);
}

static void show_results() {
    for (int i = 0; i < larUnit.testsLength; i++) {
        printf("%i\n", larUnit.tests[i].info.status);
    }
}

void end_lar_unit() {
    show_results();
    free(larUnit.tests);
}

static void append_test(const TestFunc newTest) {
    if (larUnit.testsCapacity < larUnit.testsLength + 1) {
        larUnit.testsCapacity = GROW_CAPACITY(larUnit.testsCapacity);
        larUnit.tests = allocate(
            larUnit.tests, larUnit.testsCapacity, sizeof(TestFunc)
        );
    }
    larUnit.tests[larUnit.testsLength++] = newTest;
}

static void append_failure(const Failure failure) {
    TestFunc *testFunc = &larUnit.tests[larUnit.testsLength - 1];

    if (testFunc->failsCapacity < testFunc->failsLength + 1) {
        testFunc->failsCapacity = GROW_CAPACITY(testFunc->failsCapacity);
        testFunc->failures = allocate(
            testFunc->failures, testFunc->failsCapacity, sizeof(Failure)
        );
    }
    testFunc->failures[testFunc->failsLength] = failure;
}

void test_func(const EmptyFunc funcToTest) {
    TestFunc testFunc;
    init_test(&testFunc);
    append_test(testFunc);
    funcToTest();
    // add warning about the tested function having no assertion
    // or iterate in "end_lar_unit" over "unknown" status results and show warning for them.
}

static void assert_success(const TestInfo newInfo) {
    TestInfo *info = &larUnit.tests[larUnit.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = SUCCESS;
    }
}

static void assert_failure(const TestInfo newInfo, const int line, const char *message) {
    TestInfo *info = &larUnit.tests[larUnit.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
    }
    info->status = FAILURE;
    Failure failure = {.line=line, .message=message};
    append_failure(failure);
}

void check_condition(
    const bool conditionResult, char *message,
    char *fileName, char *funcName, int line
) {
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};
    if (conditionResult) {
        assert_success(info);
    } else {
        assert_failure(info, line, message);
    }
}

void make_set_up(const EmptyFunc newSetUp) {
    larUnit.setUp = newSetUp;
}

void make_tear_down(const EmptyFunc newTearDown) {
    larUnit.tearDown = newTearDown;
}
