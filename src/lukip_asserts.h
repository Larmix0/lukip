#ifndef LUKIP_ASSERTS_H
#define LUKIP_ASSERTS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lukip.h"

typedef void (*EmptyFunc)();

typedef enum {
    UNKNOWN,
    SUCCESS,
    FAILURE
} TestStatus;

typedef struct {
    char *message;
    int line;
} Failure;

typedef struct {
    char *fileName;
    char *funcName;
    int line;
} LineInfo;

typedef struct {
    TestStatus status;
    char *fileName;
    char *funcName;
} TestInfo;

typedef struct {
    int failsCapacity;
    int failsLength;
    Failure *failures;

    LineInfo caller;
    TestInfo info;
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

void make_set_up(const EmptyFunc newSetUp);
void make_tear_down(const EmptyFunc newTearDown);

void test_func(const EmptyFunc funcToTest, LineInfo caller);

void assert_bytes_equal(void *array1, void *array2, const int length, LineInfo lineInfo);
void assert_strings_equal(char *string1, char *string2, LineInfo lineInfo);

void verify_condition(bool condition, LineInfo lineInfo, const char *format, ...);

#endif
