#ifndef LAR_UNIT_IMPL_H
#define LAR_UNIT_IMPL_H

#include <stdbool.h>

#include "lar_unit.h"

typedef void (*EmptyFunc)();

typedef enum {
    UNKNOWN,
    SUCCESS,
    FAILURE
} TestStatus;

typedef struct {
    int line;
    char *message;
} Failure;

typedef struct {
    TestStatus status;
    char *fileName;
    char *funcName;
} TestInfo;

typedef struct {
    int failsCapacity;
    int failsLength;
    Failure *failures;

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

void assert_success(const TestInfo newInfo);
void assert_failure(const TestInfo newInfo, const int line, char *message);

void test_func(const EmptyFunc funcToTest);
void check_condition(
    const bool conditionResult, char *message,
    char *fileName, char *funcName, const int line
);

#endif
