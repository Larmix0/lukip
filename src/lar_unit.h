#ifndef LAR_UNIT_H
#define LAR_UNIT_H

#include <stdio.h>
#include <stdlib.h>

#include "lar_unit_impl.h"
#include "lar_asserts.h"

#define LAR_INIT() init_lar_unit()
#define LAR_END() end_lar_unit()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)

#define TEST(FuncToTest) test_func(FuncToTest)

// memory leak because FORMAT_ALLOC'd message is abandoned by check_condition if successful.
#define VERIFY_2_VALS(formatted_error, condition, val1Format, val2Format) \
    do { \
        char *message; \
        FORMAT_ALLOC(message, formatted_error, val1Format, val2Format); \
        check_condition(condition, message, __FILE__, (char *)__func__, __LINE__); \
    } while (0)

#define LAR_INT_EQUAL(val1, val2) \
    VERIFY_2_VALS("%d != %d. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_UINT_EQUAL(val1, val2) \
    VERIFY_2_VALS("%u != %u. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_LONG_EQUAL(val1, val2) \
    VERIFY_2_VALS("%li != %li. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_ULONG_EQUAL(val1, val2) \
    VERIFY_2_VALS("%lu != %lu. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_SIZE_T_EQUAL(val1, val2) \
    VERIFY_2_VALS("%zu != %zu. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_FLOAT_EQUAL(val1, val2) \
    VERIFY_2_VALS("%f != %f. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_DOUBLE_EQUAL(val1, val2) \
    VERIFY_2_VALS("%lf != %lf. (Expected equal)", (val1) == (val2), (val1), (val2))

#define LAR_BOOL_EQUAL(val1, val2) \
    VERIFY_2_VALS( \
        "%s != %s. (Expected equal)", (val1) == (val2), \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LAR_BYTES_EQUAL(val1, val2, length) \
    assert_bytes_equal(val1, val2, length, __FILE__, (char *)__func__, __LINE__)

#define LAR_STRING_EQUAL(val1, val2) \
    assert_strings_equal(val1, val2, __FILE__, (char *)__func__, __LINE__)

#endif
