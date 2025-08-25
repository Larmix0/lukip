/**
 * @file lukip_asserts.c
 * @brief Implementation of Lukip asserts.
 * 
 * @author Larmix
 */

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "lukip_dynamic_array.h"
#include "lukip_assert.h"
#include "lukip_output.h"

/** Increase the capacity of a dynamically growable array. */
#define GROW_CAPACITY(capacity) ((capacity) < 16 ? 16 : (capacity) * 2)

/** Length of our temporary, global buffer. */
#define BUFFER_LENGTH 256

/** Dynamically growable string. */
LKP_DECLARE_DA_STRUCT(DynamicMessage, char);

static char buffer[BUFFER_LENGTH]; /** Temporary buffer. */
static LukipUnit lukip; /** The unit which stores the unit-test's info. */

/** Initializes the passed dynamic message with a NUL terminator. Use this over LP_INIT_DA. */
static void init_message(DynamicMessage *message) {
    LKP_INIT_DA(message);
    LKP_APPEND_DA(message, '\0');
}

/** frees all failure messages of a test function which had failures. */
static void free_failure_messages(LkpTestFunc *failedFunc) {
    for (int i = 0; i < failedFunc->failures.length; i++) {
        free(failedFunc->failures.data[i].message);
    }
}

/** Initializes the Lukip unit to start the program, and sets end_lukip to run at exit. */
void init_lukip() {
    LKP_INIT_DA(&lukip.tests);
    LKP_INIT_DA(&lukip.warnings);

    lukip.setup = NULL;
    lukip.teardown = NULL;
    lukip.startTime = clock();
    lukip.hasFailed = false;
    if (atexit(end_lukip) != 0) {
        fprintf(
            stderr, "Lukip failed to set a clean up at exit: %s (Errno %d)", strerror(errno), errno
        );
        exit(EXIT_FAILURE);
    }
}

/** Ends the Lukip unit, which is by displaying the results and freeing resources. */
void end_lukip() {
    lkp_show_results(&lukip);
    for (int i = 0; i < lukip.warnings.length; i++) {
        free(lukip.warnings.data[i].message);
    }
    LKP_FREE_DA(&lukip.warnings);

    for (int i = 0; i < lukip.tests.length; i++) {
        if (lukip.tests.data[i].info.status == LKP_TEST_FAILURE) {
            free_failure_messages(&lukip.tests.data[i]);
            LKP_FREE_DA(&lukip.tests.data[i].failures);
        }
    }
    LKP_FREE_DA(&lukip.tests);
}

/** Initializes a LkpFuncInfo struct. */
static void init_func_info(LkpFuncInfo *info) {
    info->status = LKP_TEST_UNKNOWN;
    info->fileName = NULL;
    info->funcName = NULL;
}

/** Initializes a line info struct. */
static void init_line_info(LkpLineInfo *info) {
    info->line = 0;
    init_func_info(&info->testInfo);
}

/** Initializes a function which has tests. */
static void init_test(LkpTestFunc *test) {
    LKP_INIT_DA(&test->failures);
    test->testFunc = NULL;
    init_func_info(&test->info);
    init_line_info(&test->caller);
}

/** 
 * Returns the current status code for lukip testing.
 * 
 * @return an integer which is 1 if a unit has failed, or 0 if none have failed so far.
 */
int lkp_status() {
    return lukip.hasFailed ? 1 : 0;
}

/**
 * @brief Allocates a formatted string from a va_list.
 * 
 * @param format The formatted string to be allocated.
 * @param args The va_list of arguments for the format.
 * 
 * @return The allocated string.
 */
static char *lkp_vstrf_alloc(const char *format, va_list *args) {
    vsnprintf(buffer, BUFFER_LENGTH, format, *args);
    const size_t length = strlen(buffer) + 1; // +1 to account for NUL.
    char *message = lkp_allocate((int)length, sizeof(char));
    strncpy(message, buffer, length);
    return message;
}

/** Manually create a va_list to call the already made lkp_vstrf_alloc. */
char *lkp_strf_alloc(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = lkp_vstrf_alloc(format, &args);
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

    LKP_DROP_DA(message); // Pop NUL temporarily.
    for (int i = 0; i < srcLength; i++) {
        LKP_APPEND_DA(message, string[i]);
    }
    LKP_APPEND_DA(message, '\0'); // Put it back on.
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
 * Calls a testing function (so its macros can be used),
 * then appends it. Also uses setup and teardown if they're set.
 */
void lkp_test_func(const LkpEmptyFunc funcToTest, const LkpLineInfo caller) {
    if (lukip.setup != NULL) {
        lukip.setup();
    }
    LkpTestFunc testFunc;
    init_test(&testFunc);
    testFunc.caller = caller;
    LKP_APPEND_DA(&lukip.tests, testFunc);

    funcToTest();

    if (lukip.teardown != NULL) {
        lukip.teardown();
    }
}

/** Sets information to success if it hasn't already failed or succeeded. */
static void assert_success(const LkpFuncInfo newInfo) {
    lukip.asserts++;

    LkpFuncInfo *info = &lukip.tests.data[lukip.tests.length - 1].info;
    if (info->status == LKP_TEST_UNKNOWN) {
        info->fileName = newInfo.fileName;
        info->funcName = newInfo.funcName;
        info->status = LKP_TEST_SUCCESS;
    }
}

/** Sets the function's status to fail and appends the failed assert. */
static void assert_failure(const LkpLineInfo newInfo, char *message) {
    lukip.asserts++;
    lukip.failedAsserts++;

    LkpFuncInfo *info = &lukip.tests.data[lukip.tests.length - 1].info;
    if (info->status == LKP_TEST_UNKNOWN) {
        info->fileName = newInfo.testInfo.fileName;
        info->funcName = newInfo.testInfo.funcName;
    }
    lukip.hasFailed = true;
    info->status = LKP_TEST_FAILURE;
    LkpFailure failure = {.line=newInfo.line, .message=message};
    LKP_APPEND_DA(&lukip.tests.data[lukip.tests.length - 1].failures, failure);
}

/** Sets both the new setup and teardown to be called between each test. */
void lkp_make_fixture(const LkpEmptyFunc newSetup, const LkpEmptyFunc newTeardown) {
    lukip.setup = newSetup;
    lukip.teardown = newTeardown;
}

/** Makes a new setup to be called between each test. */
void lkp_make_setup(const LkpEmptyFunc newSetup) {
    lukip.setup = newSetup;
}

/** Makes a new teardown to be called between each test. */
void lkp_make_teardown(const LkpEmptyFunc newTeardown) {
    lukip.teardown = newTeardown;
}

/** Appends a single character to a DynamicMessage while keeping it NUL terminated. */
static void append_message_char(DynamicMessage *message, char ch) {
    // Temporarily pop NUL and put it back on.
    LKP_DROP_DA(message);
    LKP_APPEND_DA(message, ch);
    LKP_APPEND_DA(message, '\0');
}

/**
 * @brief Pastes an int on a string as a binary number.
 * 
 * Iterates over the LkpInt and grabs its bits right to left,
 * and ads a padding space every byte for readability as well as
 * padding the last byte with zeroes so all bytes are 8 zeroes.
 * 
 * @param[out] string The string which stores the binary output.
 * @param[in] value The LkpInt to extract the binary from.
 */
static void append_int_as_binary(char *string, LkpInt value) {
    const int byteSize = 8;
    int bitsCopied = 0, length = 0;

    // Copy bits from right to left.
    while (value != 0) {
        if (bitsCopied % byteSize == 0 && bitsCopied != 0) {
            string[length++] = ' ';
        }
        string[length++] = value & 1 ? '1' : '0';
        bitsCopied++;
        value >>= 1;
    }
    // Pad remaining bits
    while (bitsCopied % byteSize != 0) {
        string[length++] = '0';
        bitsCopied++;
    }
    // We were copying right to left, but strings go from left to right, so reverse.
    string[length] = '\0';
    reverse_string(string);
}

/**
 * @brief Appends a binary formatted string, where each "%b" expects an integer argument.
 * 
 * @note This only handles %b format, not other formats you'd expect from printf for example.
 * 
 * @param[out] message The message to be appended the formatted string.
 * @param[in] format The formatted message.
 * @param[in] args Arguments for the format.
 */
static void binary_str_sprint(DynamicMessage *message, const char *format, va_list *args) {
    int idx = 0;
    while (format[idx] != '\0') {
        // Not a format.
        if (format[idx] != '%') {
            append_message_char(message, format[idx++]);
            continue;
        }
        idx++;
        if (format[idx] == 'b') {
            idx++;
            append_int_as_binary(buffer, va_arg(*args, LkpInt));
            append_message_string(message, buffer);
            break;
        }
    }
    message->data[message->length] = '\0';
}

/** 
 * Tries to verify the passed condition.
 * Otherwise, it creates a va_list for the passed error message to assert failure. 
 */
void lkp_verify_condition(const bool condition, const LkpLineInfo info, const char *format, ...) {
    if (condition) {
        assert_success(info.testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    char *message = lkp_vstrf_alloc(format, &args);
    assert_failure(info, message);
    va_end(args);
}

/** 
 * Tries to verify the binary condition, upon failure it creates a DynamicMessage
 * to put the formatted error inside and asserts failure.
 */
void lkp_verify_binary(const bool condition, const LkpLineInfo info, const char *format, ...) {
    if (condition) {
        assert_success(info.testInfo);
        return;
    }
    va_list args;
    va_start(args, format);
    DynamicMessage message;
    init_message(&message);

    binary_str_sprint(&message, format, &args);
    assert_failure(info, message.data);
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
    const char *string1, const char *string2, const LkpLineInfo info
) {
    const size_t length1 = strlen(string1);
    const size_t length2 = strlen(string2);
    if (length1 != length2) {
        char *message = lkp_strf_alloc(
            "Different string lengths: %zu Does not equal %zu.", length1, length2
        );
        assert_failure(info, message);
        return;
    }
    if (strncmp(string1, string2, length1) != 0) {
        char *message = lkp_strf_alloc(
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
    const char *string1, const char *string2, const LkpLineInfo info
) {
    if (strlen(string1) != strlen(string2)) {
        assert_success(info.testInfo);
        return;
    }
    if (strncmp(string1, string2, strlen(string1)) == 0) {
        char *message = lkp_strf_alloc(
            "\"%s\" Is not different from \"%s\".", string1, string2
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

/** Calls a string operation function based off of op for assertion. */
void lkp_verify_strings(
    const char *string1, const char *string2, const LkpLineInfo info, const LkpAssertOp op
) {
    if (op == LKP_ASSERT_EQUAL) {
        assert_strings_equal(string1, string2, info);
    } else if (op == LKP_ASSERT_NOT_EQUAL) {
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
    const void *array1, const void *array2, const int length, const LkpLineInfo info
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
    char *message = lkp_strf_alloc(
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
    const void *array1, const void *array2, const int length, const LkpLineInfo info
) {
    uint8_t arr1Byte, arr2Byte;
    for (int i = 0; i < length; i++) {
        arr1Byte = ((uint8_t *)array1)[i]; 
        arr2Byte = ((uint8_t *)array2)[i];
        if (arr1Byte == arr2Byte) {
            continue;
        }
        char *message = lkp_strf_alloc(
            "Index %i of byte arrays: %u Does not equal %u.",
            i, arr1Byte, arr2Byte
        );
        assert_failure(info, message);
        return;
    }
    assert_success(info.testInfo);
}

/** Calls a byte array operation function based off of op for assertion. */
void lkp_verify_bytes_array(
    const void *array1, const void *array2, const int length,
    const LkpLineInfo info, const LkpAssertOp op
) {
    if (op == LKP_ASSERT_EQUAL) {
        assert_bytes_equal(array1, array2, length, info);
    } else if (op == LKP_ASSERT_NOT_EQUAL) {
        assert_bytes_not_equal(array1, array2, length, info);
    }
}

/**
 * We decrase the acceptableDifference by an extra place for every number of
 * digitPrecision. We also ensure that the real difference of
 * the passed floats is positive before checking if they're WithinPrecision.
 * Then, call lkp_verify_condition() depending on the operation to be done.
 */
void lkp_verify_precision(
    const LkpFloat float1, const LkpFloat float2, const int digitPrecision,
    const LkpLineInfo info, const LkpAssertOp op
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

    if (op == LKP_ASSERT_EQUAL) {
        lkp_verify_condition(
            withinPrecision, info,
            LKP_FLOAT_FMT " Does not equal " LKP_FLOAT_FMT " within %d places.",
            float1, float2, digitPrecision
        );
    } else if (op == LKP_ASSERT_NOT_EQUAL) {
        lkp_verify_condition(
            !withinPrecision, info, 
            LKP_FLOAT_FMT " Is not different from " LKP_FLOAT_FMT " within %d places.",
            float1, float2, digitPrecision
        );
    }
}

/** Raises some form of assert type immediately without any conditions. */
void lkp_raise_assert(const LkpRaiseType type, const LkpLineInfo info, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = lkp_vstrf_alloc(format, &args);

    if (type == LKP_RAISE_FAIL) {
        assert_failure(info, message);
    } else if (type == LKP_RAISE_WARN) {
        LkpWarning warning = {.location=info, .message=message};
        LKP_APPEND_DA(&lukip.warnings, warning);
    }
    va_end(args);
}
