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
} MessageBuffer;

static char buffer[BUFFER_LENGTH];
static LukipUnit lukip;

void init_lukip() {
    lukip.testsCapacity = 0;
    lukip.testsLength = 0;
    lukip.tests = NULL;

    lukip.setUp = NULL;
    lukip.tearDown = NULL;
    lukip.startTime = clock();
    lukip.successful = true;
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

static void init_message_buffer(MessageBuffer *message) {
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

static char *vstrf_alloc(const char *format, va_list args) {
    vsnprintf(buffer, BUFFER_LENGTH, format, args);
    const size_t length = strlen(buffer) + 1; // account for NUL.
    char *message = allocate((int)length, sizeof(char));
    strncpy(message, buffer, length);
    return message;
}

char *strf_alloc(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = vstrf_alloc(format, args);
    va_end(args);
    return message;
}

static void concatenate_buffer(MessageBuffer *dest, char *src) {
    int srcLength = strlen(src);

    if (dest->length + srcLength + 1 >= dest->capacity) {
        dest->capacity += srcLength * 2;
        dest->buffer = reallocate(dest->buffer, dest->capacity, sizeof(char));
    }
    dest->buffer[dest->length] = '\0';
    strncat(dest->buffer, src, srcLength);
    dest->length += srcLength;
}

void reverse_string(char *string) {
    const int length = strlen(string);
    char tmp;
    for (int i = 0; i < length / 2; i++) {
        tmp = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = tmp;
    }
}

static void sprint_int_as_bin(char *string, LukipInt value) {
    const int BYTE_SIZE = 8;
    int bitsCopied = 0, length = 0;

    while (value != 0) {
        // add white space every byte
        if (bitsCopied % BYTE_SIZE == 0 && bitsCopied != 0) {
            string[length++] = ' ';
        }
        // add bit from the very right
        string[length++] = value & 1 ? '1' : '0';
        bitsCopied++;
        value >>= 1; // read next bit
    }
    // pad remaining bits
    while (bitsCopied % BYTE_SIZE != 0) {
        string[length++] = '0';
        bitsCopied++;
    }
    // we were copying bits from right, but strings go from left to right
    // so we have to reverse the result in order to have bytes in the correct order.
    string[length] = '\0';
    reverse_string(string);
}

static void free_failure_messages(TestFunc *failedFunc) {
    for (int i = 0; i < failedFunc->failsLength; i++) {
        free(failedFunc->failures[i].message);
    }
}

void end_lukip() {
    show_results(lukip);
    for (int i = 0; i < lukip.testsLength; i++) {
        if (lukip.tests[i].info.status == FAILURE) {
            free_failure_messages(&lukip.tests[i]);
            free(lukip.tests[i].failures);
        }
    }
    free(lukip.tests);
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

static void append_failure(const Failure failure) {
    TestFunc *testFunc = &lukip.tests[lukip.testsLength - 1];

    if (testFunc->failsCapacity < testFunc->failsLength + 1) {
        testFunc->failsCapacity = GROW_CAPACITY(testFunc->failsCapacity);
        testFunc->failures = reallocate(
            testFunc->failures, testFunc->failsCapacity, sizeof(Failure)
        );
    }
    testFunc->failures[testFunc->failsLength++] = failure;
}

void test_func(const EmptyFunc funcToTest, LineInfo caller) {
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

static void assert_success(const TestInfo newInfo) {
    TestInfo *info = &lukip.tests[lukip.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = SUCCESS;
    }
}

static void assert_failure(const TestInfo newInfo, const int line, char *message) {
    TestInfo *info = &lukip.tests[lukip.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
    }
    lukip.successful = false;
    info->status = FAILURE;
    Failure failure = {.line=line, .message=message};
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

static void vbin_str_sprintf(MessageBuffer *message, const char *format, va_list args) {
    int idx = 0;
    while (format[idx] != '\0') {
        if (format[idx] != '%') {
            // + 2 to always have space for NUL.
            if (message->length + 2 >= message->capacity) {
                message->capacity = GROW_CAPACITY(message->capacity);
                message->buffer = reallocate(
                    message->buffer, message->capacity, sizeof(char)
                );
            }
            message->buffer[message->length++] = format[idx++];
            continue;
        }
        idx++;
        switch (format[idx]) {
        case 'l':
            if (format[idx + 1] == 'd' || format[idx + 1] == 'i') {
                idx += 2;
                snprintf(buffer, BUFFER_LENGTH, LUKIP_INT_STR, va_arg(args, LukipInt));
                concatenate_buffer(message, buffer);
            }
            else if (format[idx + 1] == 'u') {
                idx += 2;
                snprintf(buffer, BUFFER_LENGTH, LUKIP_UINT_STR, va_arg(args, LukipUnsigned));
                concatenate_buffer(message, buffer);
            }
            break;
        case 'd':
            idx++;
            snprintf(buffer, BUFFER_LENGTH, LUKIP_INT_STR, va_arg(args, LukipInt));
            concatenate_buffer(message, buffer);
            break;
        case 'u':
            idx++;
            snprintf(buffer, BUFFER_LENGTH, LUKIP_UINT_STR, va_arg(args, LukipUnsigned));
            concatenate_buffer(message, buffer);
            break;
        case 's':
            idx++;
            sprint_int_as_bin(buffer, va_arg(args, LukipInt));
            concatenate_buffer(message, buffer);
            break;
        }
    }
    message->buffer[message->length] = '\0';
}

void verify_binary(bool condition, LineInfo lineInfo, const char *format, ...) {
    TestInfo testInfo = {
        .fileName=lineInfo.fileName, .funcName=lineInfo.funcName, .status=UNKNOWN
    };
    if (condition) {
        assert_success(testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    MessageBuffer message;
    init_message_buffer(&message);

    vbin_str_sprintf(&message, format, args);
    assert_failure(testInfo, lineInfo.line, message.buffer);
    va_end(args);
}

void assert_bytes_equal(
    void *array1, void *array2, const int length, LineInfo lineInfo
) {
    TestInfo testInfo = {
        .fileName=lineInfo.fileName, .funcName=lineInfo.funcName, .status=UNKNOWN
    };
    uint8_t arr1Byte, arr2Byte;

    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte == arr2Byte) {
            continue;
        }
        char *message = strf_alloc(
            "Failed at index %i: %u != %u. (Expected equal).", i, arr1Byte, arr2Byte
        );
        assert_failure(testInfo, lineInfo.line, message);
        return;
    }
    assert_success(testInfo);
}

void assert_strings_equal(char *string1, char *string2, LineInfo lineInfo) {
    TestInfo testInfo = {
        .fileName=lineInfo.fileName, .funcName=lineInfo.funcName, .status=UNKNOWN
    };
    size_t length1 = strlen(string1);
    size_t length2 = strlen(string2);
    if (length1 != length2) {
        char *message = strf_alloc(
            "Different lengths: %zu != %zu. (Expected same strings).", length1, length2
        );
        assert_failure(testInfo, lineInfo.line, message);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) != 0) {
        char *message = strf_alloc(
            "%s != %s. (Expected same strings).", string1, string2
        );
        assert_failure(testInfo, lineInfo.line, message);
        return;
    }
    assert_success(testInfo);
}

void verify_condition(bool condition, LineInfo lineInfo, const char *format, ...) {
    TestInfo testInfo = {
        .fileName=lineInfo.fileName, .funcName=lineInfo.funcName, .status=UNKNOWN
    };
    if (condition) {
        assert_success(testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    char *message = vstrf_alloc(format, args);
    assert_failure(testInfo, lineInfo.line, message);
    va_end(args);
}
