#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "lar_asserts.h"
#include "output.h"

#define GROW_CAPACITY(capacity) ((capacity) < 16 ? 16 : (capacity) * 2)
#define BUFFER_LENGTH 256

static char buffer[BUFFER_LENGTH];
static LarUnit larUnit;

static void *reallocate(void *pointer, const int newSize, const size_t elementSize) {
    void *result = realloc(pointer, newSize * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

static void *allocate(const int newSize, const size_t elementSize) {
    void *result = malloc(newSize * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

static char *vfstr_alloc(const char *format, va_list args) {
    vsnprintf(buffer, BUFFER_LENGTH, format, args);
    const int len = strlen(buffer) + 1; // account for NUL.
    char *message = allocate(len, sizeof(char));
    strncpy(message, buffer, len);
    return message;
}

static char *fstr_alloc(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = vfstr_alloc(format, args);
    va_end(args);
    return message;
}

void init_lar_unit() {
    larUnit.testsCapacity = 0;
    larUnit.testsLength = 0;
    larUnit.tests = NULL;

    larUnit.setUp = NULL;
    larUnit.tearDown = NULL;
    larUnit.successful = false;
}

static void init_test_info(TestInfo *info) {
    info->status = UNKNOWN;
    info->fileName = NULL;
    info->funcName = NULL;
}

static void init_line_info(LineInfo *info) {
    info->line = 0;
    info->fileName = NULL;
    info->funcName = NULL;
}

static void init_test(TestFunc *test) {
    test->failsCapacity = 0;
    test->failsLength = 0;
    test->failures = NULL;
    test->testFunc = NULL;
    init_test_info(&test->info);
    init_line_info(&test->caller);
}

static void free_failure_messages(TestFunc *failedFunc) {
    for (int i = 0; i < failedFunc->failsLength; i++) {
        free(failedFunc->failures[i].message);
    }
}

void end_lar_unit() {
    show_results(larUnit);
    for (int i = 0; i < larUnit.testsLength; i++) {
        if (larUnit.tests[i].info.status == FAILURE) {
            free_failure_messages(&larUnit.tests[i]);
            free(larUnit.tests[i].failures);
        }
    }
    free(larUnit.tests);
}

static void append_test(const TestFunc newTest) {
    if (larUnit.testsCapacity < larUnit.testsLength + 1) {
        larUnit.testsCapacity = GROW_CAPACITY(larUnit.testsCapacity);
        larUnit.tests = reallocate(
            larUnit.tests, larUnit.testsCapacity, sizeof(TestFunc)
        );
    }
    larUnit.tests[larUnit.testsLength++] = newTest;
}

static void append_failure(const Failure failure) {
    TestFunc *testFunc = &larUnit.tests[larUnit.testsLength - 1];

    if (testFunc->failsCapacity < testFunc->failsLength + 1) {
        testFunc->failsCapacity = GROW_CAPACITY(testFunc->failsCapacity);
        testFunc->failures = reallocate(
            testFunc->failures, testFunc->failsCapacity, sizeof(Failure)
        );
    }
    testFunc->failures[testFunc->failsLength++] = failure;
}

void test_func(const EmptyFunc funcToTest, LineInfo caller) {
    if (larUnit.setUp != NULL) {
        larUnit.setUp();
    }
    TestFunc testFunc;
    init_test(&testFunc);
    testFunc.caller = caller;

    append_test(testFunc);
    funcToTest();
    if (larUnit.tearDown != NULL) {
        larUnit.tearDown();
    }
}

static void assert_success(const TestInfo newInfo) {
    TestInfo *info = &larUnit.tests[larUnit.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = SUCCESS;
    }
}

static void assert_failure(const TestInfo newInfo, const int line, char *message) {
    TestInfo *info = &larUnit.tests[larUnit.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
    }
    larUnit.successful = false;
    info->status = FAILURE;
    Failure failure = {.line=line, .message=message};
    append_failure(failure);
}

void make_set_up(const EmptyFunc newSetUp) {
    larUnit.setUp = newSetUp;
}

void make_tear_down(const EmptyFunc newTearDown) {
    larUnit.tearDown = newTearDown;
}

void assert_bytes_equal(
    void *array1, void *array2, const int length,
    char *fileName, char *funcName, const int line
) {
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};
    uint8_t arr1Byte, arr2Byte;

    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte == arr2Byte) {
            continue;
        }
        char *message = fstr_alloc(
            "Failed at index %i: %u != %u. (Expected equal).", i, arr1Byte, arr2Byte
        );
        assert_failure(info, line, message);
        return;
    }
    assert_success(info);
}

void assert_strings_equal(
    char *string1, char *string2,
    char *fileName, char *funcName, const int line
) {
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};
    size_t length1 = strlen(string1);
    size_t length2 = strlen(string2);
    if (length1 != length2) {
        char *message = fstr_alloc(
            "Different lengths: %zu != %zu. (Expected same strings)", length1, length2
        );
        assert_failure(info, line, message);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) != 0) {
        char *message = fstr_alloc(
            "%s != %s. (Expected same strings)", string1, string2
        );
        assert_failure(info, line, message);
        return;
    }
    assert_success(info);
}

void verify_condition(
    bool condition, char *fileName, char *funcName, const int line,
    const char *format, ...
) {
    TestInfo info = {.fileName=fileName, .funcName=funcName, .status=UNKNOWN};
    if (condition) {
        assert_success(info);
        return;
    }
    va_list args;
    va_start(args, format);
    char *message = vfstr_alloc(format, args);
    assert_failure(info, line, message);
    va_end(args);
}
