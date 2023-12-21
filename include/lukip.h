#ifndef LUKIP_H
#define LUKIP_H

#include <stdio.h>
#include <stdlib.h>

#include "../src/lukip_asserts.h"

#define LUKIP_INIT() init_lukip()
#define LUKIP_END() end_lukip()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)
#define MAKE_TEST_SUITE(setUpFunc, tearDownFunc) \
    make_test_suite(setUpFunc, tearDownFunc)

#define LINE_INFO (LineInfo) \
    {.fileName=__FILE__, .funcName=(char *)__func__, .line=__LINE__,}

#define TEST(FuncToTest) test_func(FuncToTest, LINE_INFO)

#define LUKIP_INT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%d != %d. (Expected same int).", (val1), (val2) \
    )

#define LUKIP_UINT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%u != %u. (Expected same unsigned int).", (val1), (val2) \
    )

#define LUKIP_LONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%li != %li. (Expected same long).", (val1), (val2) \
    )

#define LUKIP_ULONG_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%lu != %lu. (Expected same unsigned long).", (val1), (val2) \
    )

#define LUKIP_CHAR_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%c != %c. (Expected same char).", (val1), (val2) \
    )

#define LUKIP_SIZE_T_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%zu != %zu. (Expected same size_t).", (val1), (val2) \
    )

#define LUKIP_FLOAT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%f != %f. (Expected same float).", (val1), (val2) \
    )

#define LUKIP_DOUBLE_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%lf != %lf. (Expected same double).", (val1), (val2) \
    )

#define LUKIP_BOOL_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%s != %s. (Expected same booleans).", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LUKIP_ADDRESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%p != %p. (Expected same addresses).", (val1), (val2) \
    )

#define LUKIP_HEX_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X != 0x%X. (" LUKIP_INT_STR " != " LUKIP_INT_STR ", expected same hexes).", \
        (val1), (val2), (val1), (val2) \
    )

#define LUKIP_UHEX_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X != 0x%X. (" LUKIP_UINT_STR " != " LUKIP_UINT_STR ", expected same hexes).", \
        (val1), (val2), (val1), (val2) \
    )

#define LUKIP_BINARY_EQUAL(val1, val2) \
    verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%s != %s. (" LUKIP_INT_STR " != " LUKIP_INT_STR ", Expected same binary).", \
         (val1), (val2), (val1), (val2) \
    )

#define LUKIP_UBINARY_EQUAL(val1, val2) \
    verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%s != %s. (" LUKIP_UINT_STR " != " LUKIP_UINT_STR ", Expected same binary).", \
         (val1), (val2), (val1), (val2) \
    )

// TODO: make it a "verify_bytes" function or create an assert_bytes_not_equal function.
#define LUKIP_BYTES_EQUAL(val1, val2, length) \
    assert_bytes_equal(val1, val2, length, LINE_INFO)

// TODO: make it a "verify_string" function or create an assert_strings_not_equal function.
#define LUKIP_STRING_EQUAL(val1, val2) \
    assert_strings_equal(val1, val2, LINE_INFO)

#define LUKIP_IS_TRUE(val) \
    verify_condition( \
        (val) == true, LINE_INFO, \
        "%s != true. (Expected true).", (val) == true ? "true" : "false" \
    )

#define LUKIP_IS_FALSE(val) \
    verify_condition( \
        (val) == false, LINE_INFO, \
        "%s != false. (Expected false).", (val) == true ? "true" : "false" \
    )

#define LUKIP_IS_NULL(val) \
    verify_condition( \
        (val) == NULL, LINE_INFO, \
        "%p != NULL. (Expected NULL).", (val) \
    )

#define LUKIP_IS_CONDITION(val) \
    verify_condition( \
        (val) == true, LINE_INFO, \
        "%s doesn't evaluate to true. (Expected true).", val \
    )

#endif
