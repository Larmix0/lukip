#ifndef LUKIP_ASSERTS_H
#define LUKIP_ASSERTS_H

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "lukip.h"

// format specifiers for Lukip numbers.
#define LUKIP_INT_FMT "%" PRId64
#define LUKIP_UINT_FMT "%" PRIu64
#define LUKIP_FLOAT_FMT "%lf"

#define LINE_INFO \
    (LineInfo){ \
        (FuncInfo){.status=UNKNOWN, .fileName=__FILE__, .funcName=(char *)__func__}, \
        .line=__LINE__, \
    }

typedef int64_t LukipInt;
typedef uint64_t LukipUnsigned;
typedef double LukipFloat;

typedef void (*EmptyFunc)();

typedef enum {
    ASSERT_EQUAL,
    ASSERT_NOT_EQUAL
} AssertOp;

typedef enum {
    UNKNOWN,
    SUCCESS,
    FAILURE
} TestStatus;

typedef struct {
    TestStatus status;
    const char *fileName;
    const char *funcName;
} FuncInfo;

typedef struct {
    FuncInfo testInfo;
    int line;
} LineInfo;

typedef struct {
    char *message;
    int line;
} Failure;

typedef struct {
    int failsCapacity;
    int failsLength;
    Failure *failures;

    LineInfo caller;
    FuncInfo info;
    EmptyFunc testFunc;
} TestFunc;

typedef struct {
    int testsCapacity;
    int testsLength;
    TestFunc *tests;

    EmptyFunc setUp;
    EmptyFunc tearDown;
    clock_t startTime;
    bool successful;
} LukipUnit;

void init_lukip();
void end_lukip();

char *strf_alloc(const char *format, ...);

void make_test_suite(const EmptyFunc newSetUp, const EmptyFunc newTearDown);
void make_set_up(const EmptyFunc newSetUp);
void make_tear_down(const EmptyFunc newTearDown);

void test_func(const EmptyFunc funcToTest, const LineInfo caller);

void verify_condition(const bool condition, const LineInfo info, const char *format, ...);
void verify_binary(const bool condition, const LineInfo info, const char *format, ...);
void verify_strings(
    const char *string1, const char *string2, const LineInfo info, const AssertOp op
);
void verify_bytes_array(
    const void *array1, const void *array2, const int length,
    const LineInfo info, const AssertOp op
);
void verify_precision(
    const LukipFloat float1, const LukipFloat float2, const int digitPrecision,
    const LineInfo info, const AssertOp op, const char *format, ...
);

#endif
