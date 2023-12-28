#ifndef LUKIP_ASSERTS_H
#define LUKIP_ASSERTS_H

/**
 * @file lukip_asserts.h
 * @brief Header for Lukip functionality and asserts implementation.
 * 
 * @author Larmix
 */

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "lukip.h"

/** Pastes all information before function call (file name, function name, and line.). */
#define LINE_INFO \
    (LineInfo){ \
        (FuncInfo){.status=UNKNOWN, .fileName=__FILE__, .funcName=(char *)__func__}, \
        .line=__LINE__, \
    }

#define LUKIP_INT_FMT "%" PRId64 /** Lukip integer format for strings. */
#define LUKIP_UINT_FMT "%" PRIu64 /** Lukip unsigned integer format for strings. */
#define LUKIP_FLOAT_FMT "%lf" /** Lukip float format for strings. */

/** Integer assumed to be compatible with what the user passes. */
typedef int64_t LukipInt;

/** Unsigned integer assumed to be compatible with what the user passes. */
typedef uint64_t LukipUnsigned;

/** Float assumed to be compatible with what the user passes. */
typedef double LukipFloat;

/** Pointer to function with no parameters or return value. */
typedef void (*EmptyFunc)();

/** An enum to differentiate between equal and unequal without an ambiguous bool. */
typedef enum {
    ASSERT_EQUAL,
    ASSERT_NOT_EQUAL
} AssertOp;

/** The current status of a given test. */
typedef enum {
    UNKNOWN,
    SUCCESS,
    FAILURE
} TestStatus;

/**
 * @brief Information of a test function.
 * 
 * The file and function names are only set after macro asserts are called.
 * This is because we need certain variables to get the needed names which are:
 * "__LINE__", "__func__", and "__FILE__" which only give information based off of
 * the function they were called on.
 */
typedef struct {
    TestStatus status;
    const char *fileName;
    const char *funcName;
} FuncInfo;

/** Has a test function's information + a line where a macro was called. */
typedef struct {
    FuncInfo testInfo;
    int line;
} LineInfo;

/** Stores a failed assert's message and line where it was called. */
typedef struct {
    char *message;
    int line;
} Failure;

/** Information of a function used for testing as a whole. */
typedef struct {
    int failsCapacity;
    int failsLength;
    Failure *failures;

    LineInfo caller;
    FuncInfo info;
    EmptyFunc testFunc;
} TestFunc;

/** The main struct which stores the fields used for unit-testing. */
typedef struct {
    int testsCapacity;
    int testsLength;
    TestFunc *tests;

    EmptyFunc setUp;
    EmptyFunc tearDown;
    clock_t startTime;
    bool successful;
} LukipUnit;

/** Initializes the Lukip unit. */
void init_lukip();

/** Ends the Lukip unit*/
void end_lukip();

/** 
 * @brief Allocates a formatted string.
 * 
 * @param format The formatted string.
 * @param ... Arguments for the format.
 * 
 * @return The allocated string.
 */
char *strf_alloc(const char *format, ...);

/** Makes both a new set up and a new tear down. */
void make_test_suite(const EmptyFunc newSetUp, const EmptyFunc newTearDown);

/** Calls passed set up function before every test. */
void make_set_up(const EmptyFunc newSetUp);

/** Calls passed tear down function after every test. */
void make_tear_down(const EmptyFunc newTearDown);

/**
 * @brief Performs a unit test on a function.
 * 
 * @param funcToTest The function to be tested.
 * @param caller Information about the place where the TEST() call was made.
 */
void test_func(const EmptyFunc funcToTest, const LineInfo caller);

/**
 * @brief Verifies that a condition is true.
 * 
 * @param condition The boolean result of the passed condition.
 * @param info The line information of the assert.
 * @param format The formatted error message in the case of failing.
 * @param ... Arguments for the format.
 */
void verify_condition(const bool condition, const LineInfo info, const char *format, ...);

/**
 * @brief Verify a condition and allow %b to format ints as a binary string.
 * 
 * @param condition The boolean result of the passed condition.
 * @param info The line information of the assert.
 * @param format The formatted error message in the case of failing.
 * @param ... Arguments for the format.
 */
void verify_binary(const bool condition, const LineInfo info, const char *format, ...);

/**
 * @brief Checks if 2 strings evaluate to a passed condition.
 * 
 * @param string1 First string.
 * @param string2 Second string.
 * @param info The line information of the assert.
 * @param op The operation to be done on the strings.
 */
void verify_strings(
    const char *string1, const char *string2, const LineInfo info, const AssertOp op
);

/**
 * @brief Checks if 2 byte arrays evaluate to a passed condition.
 * 
 * @param array1 First byte array.
 * @param array2 Second byte array.
 * @param length How many bytes to compare.
 * @param info The line information of the assert.
 * @param op The operation to be done on the strings.
 */
void verify_bytes_array(
    const void *array1, const void *array2, const int length,
    const LineInfo info, const AssertOp op
);

/**
 * @brief Checks if 2 floating values within decimal places evaluate to a condition.
 * 
 * @param float1 First float.
 * @param float2 Second float.
 * @param digitPrecision Precision in floating places.
 * @param info The line information of the assert.
 * @param op The operation to be done on the floats
 */
void verify_precision(
    const LukipFloat float1, const LukipFloat float2, const int digitPrecision,
    const LineInfo info, const AssertOp op
);

#endif
