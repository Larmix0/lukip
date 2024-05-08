#ifndef LUKIP_ASSERT_H
#define LUKIP_ASSERT_H

/**
 * @file lukip_assert.h
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
#include "lukip_dynamic_array.h"

/** Pastes all information before function call (file name, function name, and line.). */
#define LKP_LINE_INFO \
    (LkpLineInfo){ \
        (LkpFuncInfo){.status = UNKNOWN, .fileName = __FILE__, .funcName = (char *)__func__}, \
        .line = __LINE__, \
    }

#define LKP_INT_FMT "%" PRId64 /** Lukip integer format for strings. */
#define LKP_UINT_FMT "%" PRIu64 /** Lukip unsigned integer format for strings. */
#define LKP_FLOAT_FMT "%lf" /** Lukip float format for strings. */

/** Integer assumed to be compatible with what the user passes. */
typedef int64_t LkpInt;

/** Unsigned integer assumed to be compatible with what the user passes. */
typedef uint64_t LkpUnsigned;

/** Float assumed to be compatible with what the user passes. */
typedef double LkpFloat;

/** Pointer to function with no parameters or return value. */
typedef void (*LkpEmptyFunc)();

/** An enum to differentiate between equal and unequal without an ambiguous bool. */
typedef enum {
    ASSERT_EQUAL,
    ASSERT_NOT_EQUAL
} LkpAssertOp;

typedef enum {
    RAISE_WARN,
    RAISE_FAIL
} LkpRaiseType;

/** The current status of a given test. */
typedef enum {
    UNKNOWN,
    SUCCESS,
    FAILURE
} LkpTestStatus;

/**
 * @brief Information of a test function.
 * 
 * The file and function names are only set after macro asserts are called.
 * This is because we need certain variables to get the needed names which are:
 * "__LINE__", "__func__", and "__FILE__" which only give information based off of
 * the function they were called on.
 */
typedef struct {
    LkpTestStatus status;
    const char *fileName;
    const char *funcName;
} LkpFuncInfo;

/** Has a test function's information + a line where a macro was called. */
typedef struct {
    LkpFuncInfo testInfo;
    int line;
} LkpLineInfo;

/** Stores a failed assert's message and line where it was called. */
typedef struct {
    char *message;
    int line;
} LkpFailure;

/** Array of failure asserts in test functions. */
LKP_DECLARE_DA_STRUCT(LkpFailureArray, LkpFailure);

typedef struct {
    char *message;
    LkpLineInfo location;
} LkpWarning;

/** Array of warnings during testing. */
LKP_DECLARE_DA_STRUCT(WarningArray, LkpWarning);

/** Information of a function used for testing as a whole. */
typedef struct {
    LkpFailureArray failures;
    LkpLineInfo caller;
    LkpFuncInfo info;
    LkpEmptyFunc testFunc;
} LkpTestFunc;

/** An array of tested functions. */
LKP_DECLARE_DA_STRUCT(LkpTestFuncArray, LkpTestFunc);

/** The main struct which stores the fields used for unit-testing. */
typedef struct {
    LkpTestFuncArray tests;
    WarningArray warnings;

    LkpEmptyFunc setup;
    LkpEmptyFunc teardown;
    clock_t startTime;
    int asserts;
    int failedAsserts;
    bool hasFailed;
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
char *lkp_strf_alloc(const char *format, ...);

/** Makes both a new setup and a new teardown. */
void lkp_make_fixture(const LkpEmptyFunc newSetup, const LkpEmptyFunc newTeardown);

/** Calls passed setup function before every test. */
void lkp_make_setup(const LkpEmptyFunc newSetup);

/** Calls passed teardown function after every test. */
void lkp_make_teardown(const LkpEmptyFunc newTeardown);

/**
 * @brief Performs a unit test on a function.
 * 
 * @param funcToTest The function to be tested.
 * @param caller Information about the place where the TEST() call was made.
 */
void lkp_test_func(const LkpEmptyFunc funcToTest, const LkpLineInfo caller);

/**
 * @brief Verifies that a condition is true.
 * 
 * @param condition The boolean result of the passed condition.
 * @param info The line information of the assert.
 * @param format The formatted error message in the case of failing.
 * @param ... Arguments for the format.
 */
void lkp_verify_condition(const bool condition, const LkpLineInfo info, const char *format, ...);

/**
 * @brief Verify a condition and allow %b to format ints as a binary string.
 * 
 * @param condition The boolean result of the passed condition.
 * @param info The line information of the assert.
 * @param format The formatted error message in the case of failing.
 * @param ... Arguments for the format.
 */
void lkp_verify_binary(const bool condition, const LkpLineInfo info, const char *format, ...);

/**
 * @brief Checks if 2 strings evaluate to a passed condition.
 * 
 * @param string1 First string.
 * @param string2 Second string.
 * @param info The line information of the assert.
 * @param op The operation to be done on the strings.
 */
void lkp_verify_strings(
    const char *string1, const char *string2, const LkpLineInfo info, const LkpAssertOp op
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
void lkp_verify_bytes_array(
    const void *array1, const void *array2, const int length,
    const LkpLineInfo info, const LkpAssertOp op
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
void lkp_verify_precision(
    const LkpFloat float1, const LkpFloat float2, const int digitPrecision,
    const LkpLineInfo info, const LkpAssertOp op
);

/**
 * @brief Raises some form of assert type immediately without any conditions.
 * 
 * @param type The type of raise being done.
 * @param info The line information of the assert.
 * @param format The formatted message to be written in the raise.
 * @param ... Arguments for the format.
 */
void lkp_raise_assert(const LkpRaiseType type, const LkpLineInfo info, const char *format, ...);

#endif
