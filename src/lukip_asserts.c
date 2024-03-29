/**
 * @file lukip_asserts.c
 * @brief Implementation of Lukip asserts.
 * 
 * @author Larmix
 */

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "lukip_asserts.h"
#include "lukip_output.h"

/** Increase the capacity of a dynamically growable array. */
#define GROW_CAPACITY(capacity) ((capacity) < 16 ? 16 : (capacity) * 2)

/** Length of our temporary, global buffer. */
#define BUFFER_LENGTH 256

/** Dynamically growable string. */
typedef struct {
    int capacity;
    int length;
    char *buffer;
} DynamicMessage;

static char buffer[BUFFER_LENGTH]; /** Temporary buffer. */
static LukipUnit lukip; /** The unit which stores the unit-test's info. */

/** frees all failure messages of a test function which had failures. */
static void free_failure_messages(TestFunc *failedFunc) {
    for (int i = 0; i < failedFunc->failsLength; i++) {
        free(failedFunc->failures[i].message);
    }
}

/** Initializes the Lukip unit to start the program. */
void init_lukip() {
    lukip.testsCapacity = 0;
    lukip.testsLength = 0;
    lukip.tests = NULL;

    lukip.setup = NULL;
    lukip.teardown = NULL;
    lukip.startTime = clock();
    lukip.successful = true;
}

/** Ends the Lukip unit, which is by displaying the results and freeing resources. */
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

/** Initializes a FuncInfo struct. */
static void init_func_info(FuncInfo *info) {
    info->status = UNKNOWN;
    info->fileName = NULL;
    info->funcName = NULL;
}

/** Initializes a line info struct. */
static void init_line_info(LineInfo *info) {
    info->line = 0;
    init_func_info(&info->testInfo);
}

/** Initializes a function which has tests. */
static void init_test(TestFunc *test) {
    test->failsCapacity = 0;
    test->failsLength = 0;
    test->failures = NULL;
    test->testFunc = NULL;
    init_func_info(&test->info);
    init_line_info(&test->caller);
}

/** Initializes a DynamicMessage which is a heap growable string. */
static void init_dynamic_message(DynamicMessage *message) {
    message->length = 0;
    message->capacity = 0;
    message->buffer = NULL;
}

/**
 * @brief Allocates from the heap and checks for NULL itself. 
 * 
 * @param size The amount of elements to be allocated.
 * @param elementSize The size of each element.
 * 
 * @return Allocated pointer.
 */
static void *allocate(const int size, const size_t elementSize) {
    void *result = malloc(size * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

/**
 * @brief Reallocates a certain heap pointer and checks for NULL itself.
 * 
 * @param pointer The pointer to be reallocated.
 * @param newSize The new amount of elements it should be able to hold.
 * @param elementSize The size of each element it holds.
 * 
 * @return The new reallocated pointer.
 */
static void *reallocate(void *pointer, const int newSize, const size_t elementSize) {
    void *result = realloc(pointer, newSize * elementSize);
    if (result == NULL) {
        exit(1);
    }
    return result;
}

/**
 * @brief Allocates a formatted string from a va_list.
 * 
 * @param format The formatted string to be allocated.
 * @param args The va_list of arguments for the format.
 * 
 * @return The allocated string.
 */
static char *vstrf_alloc(const char *format, va_list *args) {
    vsnprintf(buffer, BUFFER_LENGTH, format, *args);
    const size_t length = strlen(buffer) + 1; // +1 to account for NUL.
    char *message = allocate((int)length, sizeof(char));
    strncpy(message, buffer, length);
    return message;
}

/** Manually create a va_list to call the already made vstrf_alloc. */
char *strf_alloc(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = vstrf_alloc(format, &args);
    va_end(args);
    return message;
}

/**
 * @brief Appends a string to a DynamicMessage.
 * 
 * @param message The DynamicMessage to append to.
 * @param string The string to append to message.
 */
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

/** Reverses a string in place. */
static void reverse_string(char *string) {
    const int length = strlen(string);
    char tmp;
    for (int i = 0; i < length / 2; i++) {
        tmp = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = tmp;
    }
}

/**
 * @brief Pastes an int on a string as a binary number.
 * 
 * Iterates over the LukipInt and grabs its bits right to left,
 * and ads a padding space every byte for readability as well as
 * padding the last byte with zeroes so all bytes are 8 zeroes.
 * 
 * @param[out] string The string which stores the binary output.
 * @param[in] value The LukipInt to extract the binary from.
 */
static void sprint_int_as_bin(char *string, LukipInt value) {
    const int byte_size = 8;
    int bitsCopied = 0, length = 0;

    // Copy bits from right to left.
    while (value != 0) {
        if (bitsCopied % byte_size == 0 && bitsCopied != 0) {
            string[length++] = ' ';
        }
        string[length++] = value & 1 ? '1' : '0';
        bitsCopied++;
        value >>= 1;
    }
    // Pad remaining bits
    while (bitsCopied % byte_size != 0) {
        string[length++] = '0';
        bitsCopied++;
    }
    // We were copying right to left, but strings go from left to right, so reverse.
    string[length] = '\0';
    reverse_string(string);
}

/** Appends a testing function to Lukip */
static void append_test(const TestFunc newTest) {
    if (lukip.testsCapacity < lukip.testsLength + 1) {
        lukip.testsCapacity = GROW_CAPACITY(lukip.testsCapacity);
        lukip.tests = reallocate(
            lukip.tests, lukip.testsCapacity, sizeof(TestFunc)
        );
    }
    lukip.tests[lukip.testsLength++] = newTest;
}

/**
 * Calls a testing function (so its macros can be used),
 * then appends it. Also uses setup and teardown if they're set.
 */
void test_func(const EmptyFunc funcToTest, const LineInfo caller) {
    if (lukip.setup != NULL) {
        lukip.setup();
    }
    TestFunc testFunc;
    init_test(&testFunc);
    testFunc.caller = caller;
    append_test(testFunc);

    funcToTest();

    if (lukip.teardown != NULL) {
        lukip.teardown();
    }
}

/** Append a failure to the failure array of the function we're currently testing. */
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

/** Sets information to success if it hasn't already failed or succeeded. */
static void assert_success(const FuncInfo newInfo) {
    lukip.asserts++;

    FuncInfo *info = &lukip.tests[lukip.testsLength - 1].info;
    if (info->status == UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = SUCCESS;
    }
}

/** Sets the function's status to fail and appends the failed assert. */
static void assert_failure(const LineInfo newInfo, char *message) {
    lukip.asserts++;
    lukip.failedAsserts++;

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

/** Sets both the new setup and teardown to be called between each test. */
void make_test_fixture(const EmptyFunc newSetup, const EmptyFunc newTeardown) {
    lukip.setup = newSetup;
    lukip.teardown = newTeardown;
}

/** Makes a new setup to be called between each test. */
void make_setup(const EmptyFunc newSetup) {
    lukip.setup = newSetup;
}

/** Makes a new teardown to be called between each test. */
void make_teardown(const EmptyFunc newTeardown) {
    lukip.teardown = newTeardown;
}

/** Appends a single character to a DynamicMessage. */
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

/**
 * @brief vsprintf(), but on a DynamicMessage and it allows %b binary format.
 * 
 * @param[out] message The message to be appended the formatted string.
 * @param[in] format The formatted message.
 * @param[in] args Arguments for the format.
 */
static void bin_str_vsprintf(DynamicMessage *message, const char *format, va_list *args) {
    int idx = 0;
    while (format[idx] != '\0') {
        // Not a format.
        if (format[idx] != '%') {
            append_message_char(message, format[idx++]);
            continue;
        }
        idx++;
        switch (format[idx]) {
        case 'l':
            if (format[idx + 1] == 'l') {
                idx++; // Handle lld, lli, and llu.
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
        case 'b':
            idx++;
            sprint_int_as_bin(buffer, va_arg(*args, LukipInt));
            append_message_string(message, buffer);
            break;
        }
    }
    message->buffer[message->length] = '\0';
}

/** 
 * Tries to verify the passed condition.
 * Otherwise, it creates a va_list for the passed error message to assert failure. 
 */
void verify_condition(const bool condition, const LineInfo info, const char *format, ...) {
    if (condition) {
        assert_success(info.testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    char *message = vstrf_alloc(format, &args);
    assert_failure(info, message);
    va_end(args);
}

/** 
 * Tries to verify the binary condition, upon failure it creates a DynamicMessage
 * to put the formatted error inside and asserts failure.
 */
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

/**
 * @brief Asserts that 2 strings have the same characters and length.
 * 
 * It tries to ensure that they have the same length first.
 * If it succeeded, then it strncmp's them. Both checks have different error messages. 
 * 
 * @param string1 The first string.
 * @param string2 The second string.
 * @param info The line information of the assert.
 */
static void assert_strings_equal(
    const char *string1, const char *string2, const LineInfo info
) {
    const size_t length1 = strlen(string1);
    const size_t length2 = strlen(string2);
    if (length1 != length2) {
        char *message = strf_alloc(
            "Different string lengths: %zu Does not equal %zu.", length1, length2
        );
        assert_failure(info, message);
        return;
    }
    if (strncmp(string1, string2, length1) != 0) {
        char *message = strf_alloc(
            "\"%s\" Does not equal \"%s\".", string1, string2
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

/**
 * @brief Asserts that 2 strings aren't equal.
 * 
 * @param string1 The first string.
 * @param string2 The second string.
 * @param info The line information of the assert.
 */
static void assert_strings_not_equal(
    const char *string1, const char *string2, const LineInfo info
) {
    if (strlen(string1) != strlen(string2)) {
        assert_success(info.testInfo);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) == 0) {
        char *message = strf_alloc(
            "\"%s\" Is not different from \"%s\".", string1, string2
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

/** Calls a string operation function based off of op for assertion. */
void verify_strings(
    const char *string1, const char *string2, const LineInfo info, const AssertOp op
) {
    if (op == ASSERT_EQUAL) {
        assert_strings_equal(string1, string2, info);
    } else if (op == ASSERT_NOT_EQUAL) {
        assert_strings_not_equal(string1, string2, info);
    }
}

/**
 * @brief Asserts that 2 byte arrays are not equal.
 * 
 * @param array1 The first byte array.
 * @param array2 The second byte array.
 * @param length The amount of bytes to be compared.
 * @param info The line information of the assert.
 */
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
        "Failed because byte arrays are not different."
    );
    assert_failure(info, message);
}

/**
 * @brief Asserts that 2 byte arrays are equal.
 * 
 * @param array1 The first byte array.
 * @param array2 The second byte array.
 * @param length The amount of bytes to be compared.
 * @param info The line information of the assert.
 */
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
            "Index %i of byte arrays: %u Does not equal %u.",
            i, arr1Byte, arr2Byte
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

/** Calls a byte array operation function based off of op for assertion. */
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

/**
 * We decrase the acceptableDifference by an extra place for every number of
 * digitPrecision. We also ensure that the real difference of
 * the passed floats is positive before checking if they're WithinPrecision.
 * Then, call verify_condition() depending on the operation to be done.
 */
void verify_precision(
    const LukipFloat float1, const LukipFloat float2, const int digitPrecision,
    const LineInfo info, const AssertOp op
) {
    double acceptableDifference = 0.1;
    double realDifference = float1 - float2;

    // Manual fabs and pow so we don't have to link -lm just for 2 calls.
    if (realDifference < 0) {
        realDifference = -realDifference;
    }
    for (int i = 0; i < digitPrecision - 1; i++) {
        acceptableDifference *= 0.1;
    }
    const bool withinPrecision = realDifference <= acceptableDifference ? true : false;

    if (op == ASSERT_EQUAL) {
        verify_condition(
            withinPrecision, info,
            LUKIP_FLOAT_FMT " Does not equal " LUKIP_FLOAT_FMT " within %d places.",
            float1, float2, digitPrecision
        );
    } else if (op == ASSERT_NOT_EQUAL) {
        verify_condition(
            !withinPrecision, info, 
            LUKIP_FLOAT_FMT " Is not different from " LUKIP_FLOAT_FMT " within %d places.",
            float1, float2, digitPrecision
        );
    }
}
