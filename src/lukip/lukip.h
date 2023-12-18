#ifndef LUKIP_H
#define LUKIP_H

#include <stdio.h>
#include <stdlib.h>

#include "lukip_asserts.h"

#define LUKIP_INIT() init_lukip()
#define LUKIP_END() end_lukip()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)

#define TEST(FuncToTest) \
    test_func( \
        FuncToTest, \
        (LineInfo){.fileName=__FILE__, .funcName=(char *)__func__, .line=__LINE__,} \
    )
    
// add other tests for things like: Hexadecimals, comparisons (<=, < >= >)
#define LUKIP_INT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%d != %d. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_UINT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%u != %u. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_LONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%li != %li. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_ULONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%lu != %lu. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_CHAR_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%c != %c. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_SIZE_T_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%zu != %zu. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_FLOAT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%f != %f. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_DOUBLE_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%lf != %lf. (Expected equal).", (val1), (val2) \
    )

#define LUKIP_BOOL_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), __FILE__, (char *)__func__, __LINE__, \
        "%s != %s. (Expected equal).", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LUKIP_BYTES_EQUAL(val1, val2, length) \
    assert_bytes_equal(val1, val2, length, __FILE__, (char *)__func__, __LINE__)

#define LUKIP_STRING_EQUAL(val1, val2) \
    assert_strings_equal(val1, val2, __FILE__, (char *)__func__, __LINE__)

#endif
