#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "lukip_asserts.h"
#include "lukip_output.h"

#define GROW_CAPACITY(capacity) ((capacity) < 16 ? 16 : (capacity) * 2)
#define BUFFER_LENGTH 256

typedef struct {
    int capacity;
    int length;
    char *buffer;
} DynamicMessage;

static char buffer[BUFFER_LENGTH];
static LukipUnit lukip;

static void free_failure_messages(TestFunc *failedFunc) {
    for (int i = 0; i < failedFunc->failsLength; i++) {
        free(failedFunc->failures[i].message);
    }
}

void init_lukip() {
    lukip.testsCapacity = 0;
    lukip.testsLength = 0;
    lukip.tests = NULL;

    lukip.setUp = NULL;
    lukip.tearDown = NULL;
    lukip.startTime = clock();
    lukip.successful = true;
}

void end_lukip() {
    show_results(&lukip);
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status == FAILURE) {
            free_failure_messages(&lukip.tests[i]);
            free(lukip.tests[i].failures);
        }
    }
    free(lukip.tests);
}

static void init_test_info(FuncInfo *info) {
    info->status = UNKNOWN;
    info->fileName = NULL;
    info->funcName = NULL;
}

static void init_line_info(LineInfo *info) {
    info->line = 0;
    init_test_info(&info->testInfo);
}

static void init_test(TestFunc *test) {
    test->failsCapacity = 0;
    test->failsLength = 0;
    test->failures = NULL;
    test->testFunc = NULL;
    init_test_info(&test->info);
    init_line_info(&test->caller);
}

static void init_dynamic_message(DynamicMessage *message) {
    message->length = 0;
    message->capacity = 0;
    message->buffer = NULL;
}

static void *allocate(const int size, const size_t elementSize) {
    void *result = malloc(size * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

static void *reallocate(void *pointer, const int newSize, const size_t elementSize) {
    void *result = realloc(pointer, newSize * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

static char *vstrf_alloc(const char *format, va_list *args) {
    vsnprintf(buffer, BUFFER_LENGTH, format, *args);
    const size_t length = strlen(buffer) + 1; // account for NUL.
    char *message = allocate((int)length, sizeof(char));
    strncpy(message, buffer, length);
    return message;
}

char *strf_alloc(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = vstrf_alloc(format, &args);
    va_end(args);
    return message;
}

static void append_message_string(DynamicMessage *message, const char *string) {
    const int srcLength = strlen(string);

    if (message->length + srcLength + 1 >= message->capacity) {
        message->capacity += srcLength * 2;
        message->buffer = reallocate(message->buffer, message->capacity, sizeof(char));
    }
    message->buffer[message->length] = '\0';
    strncat(message->buffer, string, srcLength);
    message->length += srcLength;
}

static void reverse_string(char *string) {
    const int length = strlen(string);
    char tmp;
    for (int i = 0; i < length / 2; i++) {
        tmp = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = tmp;
    }
}

static void sprint_int_as_bin(char *string, LukipInt value) {
    const int byte_size = 8;
    int bitsCopied = 0, length = 0;

    while (value != 0) {
        // add white space every byte
        if (bitsCopied % byte_size == 0 && bitsCopied != 0) {
            string[length++] = ' ';
        }
        // add bit from the very right
        string[length++] = value & 1 ? '1' : '0';
        bitsCopied++;
        value >>= 1; // read next bit
    }
    // pad remaining bits
    while (bitsCopied % byte_size != 0) {
        string[length++] = '0';
        bitsCopied++;
    }
    // we were copying right to left, but strings go from left to right, so reverse.
    string[length] = '\0';
    reverse_string(string);
}

static void append_test(const TestFunc newTest) {
    if (lukip.testsCapacity < lukip.testsLength + 1) {
        lukip.testsCapacity = GROW_CAPACITY(lukip.testsCapacity);
        lukip.tests = reallocate(
            lukip.tests, lukip.testsCapacity, sizeof(TestFunc)
        );
    }
    lukip.tests[lukip.testsLength++] = newTest;
}

void test_func(const EmptyFunc funcToTest, const LineInfo caller) {
    if (lukip.setUp != NULL) {
        lukip.setUp();
    }
    TestFunc testFunc;
    init_test(&testFunc);
    testFunc.caller = caller;

    append_test(testFunc);
    funcToTest();
    if (lukip.tearDown != NULL) {
        lukip.tearDown();
    }
}

static void append_failure(Failure failure) {
    TestFunc *testFunc = &lukip.tests[lukip.testsLength - 1];

    if (testFunc->failsCapacity < testFunc->failsLength + 1) {
        testFunc->failsCapacity = GROW_CAPACITY(testFunc->failsCapacity);
        testFunc->failures = reallocate(
            testFunc->failures, testFunc->failsCapacity, sizeof(Failure)
        );
    }
    testFunc->failures[testFunc->failsLength++] = failure;
}

static void assert_success(const FuncInfo newInfo) {
    FuncInfo *info = &lukip.tests[lukip.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = SUCCESS;
    }
}

static void assert_failure(const LineInfo newInfo, char *message) {
    FuncInfo *info = &lukip.tests[lukip.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.testInfo.fileName;
        info->funcName = newInfo.testInfo.funcName;
    }
    lukip.successful = false;
    info->status = FAILURE;
    Failure failure = {.line=newInfo.line, .message=message};
    append_failure(failure);
}

void make_test_suite(const EmptyFunc newSetUp, const EmptyFunc newTearDown) {
    lukip.setUp = newSetUp;
    lukip.tearDown = newTearDown;
}

void make_set_up(const EmptyFunc newSetUp) {
    lukip.setUp = newSetUp;
}

void make_tear_down(const EmptyFunc newTearDown) {
    lukip.tearDown = newTearDown;
}

static void variadic_verify_condition(
    const bool condition, const LineInfo info, const char *format, va_list *args
) {
    if (condition) {
        assert_success(info.testInfo);
        return;
    }
    char *message = vstrf_alloc(format, args);
    assert_failure(info, message);
}

void verify_condition(const bool condition, const LineInfo info, const char *format, ...) {
    va_list args;
    va_start(args, format);
    variadic_verify_condition(condition, info, format, &args);
    va_end(args);
}

static void append_message_char(DynamicMessage *message, char ch) {
    // + 2 to always have space for NUL.
    if (message->length + 2 >= message->capacity) {
        message->capacity = GROW_CAPACITY(message->capacity);
        message->buffer = reallocate(
            message->buffer, message->capacity, sizeof(char)
        );
    }
    message->buffer[message->length++] = ch;
}

static void bin_str_vsprintf(DynamicMessage *message, const char *format, va_list *args) {
    int idx = 0;
    while (format[idx] != '\0') {
        if (format[idx] != '%') {
            append_message_char(message, format[idx++]);
            continue;
        }
        idx++;
        switch (format[idx]) {
        case 'l':
            if (format[idx + 1] == 'l') {
                idx++; // handle lld, lli, and llu.
            }
            if (format[idx + 1] == 'd' || format[idx + 1] == 'i') {
                idx += 2;
                snprintf(buffer, BUFFER_LENGTH, LUKIP_INT_FMT, va_arg(*args, LukipInt));
                append_message_string(message, buffer);
            } else if (format[idx + 1] == 'u') {
                idx += 2;
                snprintf(buffer, BUFFER_LENGTH, LUKIP_UINT_FMT, va_arg(*args, LukipUnsigned));
                append_message_string(message, buffer);
            }
            break;
        case 'd':
            idx++;
            snprintf(buffer, BUFFER_LENGTH, LUKIP_INT_FMT, va_arg(*args, LukipInt));
            append_message_string(message, buffer);
            break;
        case 'u':
            idx++;
            snprintf(buffer, BUFFER_LENGTH, LUKIP_UINT_FMT, va_arg(*args, LukipUnsigned));
            append_message_string(message, buffer);
            break;
        case 's':
            idx++;
            sprint_int_as_bin(buffer, va_arg(*args, LukipInt));
            append_message_string(message, buffer);
            break;
        }
    }
    message->buffer[message->length] = '\0';
}

void verify_binary(const bool condition, const LineInfo info, const char *format, ...) {
    if (condition) {
        assert_success(info.testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    DynamicMessage message;
    init_dynamic_message(&message);

    bin_str_vsprintf(&message, format, &args);
    assert_failure(info, message.buffer);
    va_end(args);
}

static void assert_strings_equal(
    const char *string1, const char *string2, const LineInfo info
) {
    const size_t length1 = strlen(string1);
    const size_t length2 = strlen(string2);
    if (length1 != length2) {
        char *message = strf_alloc(
            "Different lengths: %zu != %zu. (Expected same strings).", length1, length2
        );
        assert_failure(info, message);
        return;
    }
    if (strncmp(string1, string2, length1) != 0) {
        char *message = strf_alloc(
            "%s != %s. (Expected same strings).", string1, string2
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

static void assert_strings_not_equal(
    const char *string1, const char *string2, const LineInfo info
) {
    if (strlen(string1) != strlen(string2)) {
        assert_success(info.testInfo);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) == 0) {
        char *message = strf_alloc(
            "%s == %s. (Expected different strings).", string1, string2
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

void verify_strings(
    const char *string1, const char *string2, const LineInfo info, const AssertOp op
) {
    if (op == ASSERT_EQUAL) {
        assert_strings_equal(string1, string2, info);
    } else if (op == ASSERT_NOT_EQUAL) {
        assert_strings_not_equal(string1, string2, info);
    }
}

static void assert_bytes_not_equal(
    const void *array1, const void *array2, const int length, const LineInfo info
) {
    uint8_t arr1Byte, arr2Byte;
    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte != arr2Byte) {
            assert_success(info.testInfo);
            return;
        }
    }
    char *message = strf_alloc(
        "Failed because byte arrays are equal. (Expected not equal)."
    );
    assert_failure(info, message);
}

static void assert_bytes_equal(
    const void *array1, const void *array2, const int length, const LineInfo info
) {
    uint8_t arr1Byte, arr2Byte;
    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte == arr2Byte) {
            continue;
        }
        char *message = strf_alloc(
            "Index %i: %u != %u. (Expected equal byte arrays).",
            i, arr1Byte, arr2Byte
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

void verify_bytes_array(
    const void *array1, const void *array2, const int length,
    const LineInfo info, const AssertOp op
) {
    if (op == ASSERT_EQUAL) {
        assert_bytes_equal(array1, array2, length, info);
    } else if (op == ASSERT_NOT_EQUAL) {
        assert_bytes_not_equal(array1, array2, length, info);
    }
}

void verify_precision(
    const LukipFloat float1, const LukipFloat float2, const int digitPrecision,
    const LineInfo info, const AssertOp op, const char *format, ...
) {
    double acceptableDifference = 0.1;
    double realDifference = float1 - float2;

    // manual fabs() and pow() so we don't have to link -lm just for 2 simple calls.
    if (realDifference < 0) {
        realDifference = -realDifference;
    }
    for (int i = 0; i < digitPrecision - 1; i++) {
        acceptableDifference *= 0.1;
    }
    const bool withinPrecision = realDifference <= acceptableDifference ? true : false;
    va_list args;
    va_start(args, format);

    if (op == ASSERT_EQUAL) {
        variadic_verify_condition(withinPrecision, info, format, &args);
    } else if (op == ASSERT_NOT_EQUAL) {
        variadic_verify_condition(!withinPrecision, info, format, &args);
    }
    va_end(args);
}
