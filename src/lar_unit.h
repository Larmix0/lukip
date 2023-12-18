#ifndef LAR_UNIT_H
#define LAR_UNIT_H

#include <stdio.h>
#include <stdlib.h>

#include "lar_asserts.h"

#define LAR_INIT() init_lar_unit()
#define LAR_END() end_lar_unit()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)

#define TEST(FuncToTest) \
    test_func( \
        FuncToTest, \
        (LineInfo){.fileName=__FILE__, .funcName=(char *)__func__, .line=__LINE__,} \
    )
    
// add other tests for things like: Hexadecimals, comparisons (<=, < >= >)
#define LAR_INT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%d != %d. (Expected equal).", (val1), (val2) \
    )

#define LAR_UINT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%u != %u. (Expected equal).", (val1), (val2) \
    )

#define LAR_LONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%li != %li. (Expected equal).", (val1), (val2) \
    )

#define LAR_ULONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%lu != %lu. (Expected equal).", (val1), (val2) \
    )

#define LAR_CHAR_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%c != %c. (Expected equal).", (val1), (val2) \
    )

#define LAR_SIZE_T_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%zu != %zu. (Expected equal).", (val1), (val2) \
    )

#define LAR_FLOAT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%f != %f. (Expected equal).", (val1), (val2) \
    )

#define LAR_DOUBLE_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%lf != %lf. (Expected equal).", (val1), (val2) \
    )

#define LAR_BOOL_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%s != %s. (Expected equal).", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LAR_BYTES_EQUAL(val1, val2, length) \
    assert_bytes_equal(val1, val2, length, __FILE__, (char *)__func__, __LINE__)

#define LAR_STRING_EQUAL(val1, val2) \
    assert_strings_equal(val1, val2, __FILE__, (char *)__func__, __LINE__)

#endif
