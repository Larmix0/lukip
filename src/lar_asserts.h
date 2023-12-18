#ifndef LAR_ASSERTS_H
#define LAR_ASSERTS_H

#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lar_unit.h"

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
    bool successful;
} LarUnit;

void init_lar_unit();
void end_lar_unit();

void make_set_up(const EmptyFunc newSetUp);
void make_tear_down(const EmptyFunc newTearDown);

void test_func(const EmptyFunc funcToTest, LineInfo caller);

void assert_bytes_equal(
    void *array1, void *array2, const int length,
    char *fileName, char *funcName, const int line
);
void assert_strings_equal(
    char *string1, char *string2,
    char *fileName, char *funcName, const int line
);
void verify_condition(
    bool condition, char *fileName, char *funcName, const int line,
    const char *format, ...
);

#endif
