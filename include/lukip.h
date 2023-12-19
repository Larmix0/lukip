#ifndef LUKIP_H
#define LUKIP_H

#include <stdio.h>
#include <stdlib.h>

#include "../src/lukip_asserts.h"

#define LUKIP_INIT() init_lukip()
#define LUKIP_END() end_lukip()

#define MAKE_TEST_SUITE(setUpFunc, tearDownFunc) make_test_suite(setUpFunc, tearDownFunc)
#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)

#define LINE_INFO (LineInfo) \
    {.fileName=__FILE__, .funcName=(char *)__func__, .line=__LINE__,}

#define TEST(FuncToTest) test_func(FuncToTest, LINE_INFO)

#define LUKIP_INT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%d != %d. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_UINT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%u != %u. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_LONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%li != %li. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_ULONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%lu != %lu. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_CHAR_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%c != %c. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_SIZE_T_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%zu != %zu. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_FLOAT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%f != %f. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_DOUBLE_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%lf != %lf. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_BOOL_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%s != %s. (Expected equal).", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LUKIP_BYTES_EQUAL(val1, val2, length) \
    assert_bytes_equal(val1, val2, length, LINE_INFO)

#define LUKIP_STRING_EQUAL(val1, val2) \
    assert_strings_equal(val1, val2, LINE_INFO)

#endif
