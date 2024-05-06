#ifndef LUKIP_H
#define ASSERT_H

/**
 * @file lukip.h
 * @brief Header for the Lukip unit testing framework for users.
 * 
 * @author Larmix
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/lukip_asserts.h"

// ================================= GENERAL ===================================

/** Must be called first before other Lukip macros. */
#define LUKIP_INIT() (init_lukip())

/** Declares a function that acts like a test case (and should be later ran). */
#define TEST_CASE(name) void name()

/** Same as TEST_CASE, but ensures the function is only visible in current translation unit. */
#define PRIVATE_TEST_CASE(name) static TEST_CASE(name)

/** Currently same as TEST_CASE, but it's made for fixtures in case the implementation changes. */
#define TEST_FIXTURE(name) void name()

/**
 * @brief Makes a new setup.
 * 
 * The passed setup gets called before every test function is called.
 * 
 * @param setupFunc The function to be called before every test.
 */
#define MAKE_SETUP(setupFunc) (make_setup(setupFunc))

/**
 * @brief Makes a new teardown.
 * 
 * The passed teardown gets called after every test function has executed.
 * 
 * @param teardownFunc The function to be called after every test.
 */
#define MAKE_TEARDOWN(teardownFunc) (make_teardown(teardownFunc))

/** Makes both a new setup and teardown at once. */
#define MAKE_TEST_FIXTURE(setupFunc, teardownFunc) (make_test_fixture(setupFunc, teardownFunc))

/** Makes setup nothing. */
#define RESET_SETUP() (make_setup(NULL))

/** Makes teardown nothing. */
#define RESET_TEARDOWN() (make_teardown(NULL))

/** Makes both setup and teardown nothing. */
#define RESET_TEST_FIXTURE() (make_test_fixture(NULL, NULL))

/**
 * @brief Tests the passed function
 * 
 * @param funcToTest The function to unit-test.
 * 
 * @note The passed function should have no parameters or return value.
 */
#define TEST(funcToTest) (test_func(funcToTest, LINE_INFO))

// ============================ NUMBER EQUAL =======================================================

#define ASSERT_INT_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%d Does not equal %d.", (val1), (val2)))

#define ASSERT_INT8_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRId8 " Does not equal %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRId16 " Does not equal %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRId32 " Does not equal %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRId64 " Does not equal %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%u Does not equal %u.", (val1), (val2)))

#define ASSERT_UINT8_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRIu8 " Does not equal %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRIu16 " Does not equal %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRIu32 " Does not equal %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, "%" PRIu64 " Does not equal %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%li Does not equal %li.", (val1), (val2)))

#define ASSERT_ULONG_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%lu Does not equal %lu.", (val1), (val2)))

#define ASSERT_SIZE_T_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%zu Does not equal %zu.", (val1), (val2)))

#define ASSERT_FLOAT_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%f Does not equal %f.", (val1), (val2)))

#define ASSERT_DOUBLE_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%lf Does not equal %lf.", (val1), (val2)))

#define ASSERT_HEX_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X Does not equal 0x%X. (" LUKIP_INT_FMT " Does not equal " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X Does not equal 0x%X. (" \
        LUKIP_UINT_FMT " Does not equal " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%b Does not equal %b. (" LUKIP_INT_FMT " Does not equal " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%b Does not equal %b. (" LUKIP_UINT_FMT " Does not equal " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

// ======================== NUMBER GREATER =========================================================

#define ASSERT_INT_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%d Is not greater than %d.", (val1), (val2) \
    ))

#define ASSERT_INT8_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRId8 " Is not greater than %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRId16 " Is not greater than %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRId32 " Is not greater than %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRId64 " Is not greater than %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%u Is not greater than %u.", (val1), (val2) \
    ))

#define ASSERT_UINT8_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRIu8 " Is not greater than %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRIu16 " Is not greater than %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRIu32 " Is not greater than %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "%" PRIu64 " Is not greater than %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%li Is not greater than %li.", (val1), (val2) \
    ))

#define ASSERT_ULONG_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%lu Is not greater than %lu.", (val1), (val2) \
    ))

#define ASSERT_SIZE_T_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%zu Is not greater than %zu.", (val1), (val2) \
    ))

#define ASSERT_FLOAT_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%f Is not greater than %f.", (val1), (val2) \
    ))

#define ASSERT_DOUBLE_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, "%lf Is not greater than %lf.", (val1), (val2) \
    ))

#define ASSERT_HEX_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "0x%X Is not greater than 0x%X. (" \
        LUKIP_INT_FMT " Is not greater than " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_GREATER(val1, val2) \
    (verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "0x%X Is not greater than 0x%X. (" \
        LUKIP_UINT_FMT " Is not greater than " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_GREATER(val1, val2) \
    (verify_binary( \
        (val1) > (val2), LINE_INFO, \
        "%b Is not greater than %b. (" \
        LUKIP_INT_FMT " Is not greater than " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_GREATER(val1, val2) \
    (verify_binary( \
        (val1) > (val2), LINE_INFO, \
        "%b Is not greater than %b. (" \
        LUKIP_UINT_FMT " Is not greater than " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

// ====================== NUMBER GREATER OR EQUAL ==================================================

#define ASSERT_INT_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, "%d Is not greater or equal to %d.", (val1), (val2) \
    ))

#define ASSERT_INT8_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRId8 " Is not greater or equal to %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRId16 " Is not greater or equal to %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRId32 " Is not greater or equal to %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRId64 " Is not greater or equal to %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%u Is not greater or equal to %u.", (val1), (val2) \
    ))

#define ASSERT_UINT8_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRIu8 " Is not greater or equal to %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRIu16 " Is not greater or equal to %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRIu32 " Is not greater or equal to %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%" PRIu64 " Is not greater or equal to %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%li Is not greater or equal to %li.", (val1), (val2) \
    ))

#define ASSERT_ULONG_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%lu Is not greater or equal to %lu.", (val1), (val2) \
    ))

#define ASSERT_SIZE_T_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%zu Is not greater or equal to %zu.", (val1), (val2) \
    ))

#define ASSERT_FLOAT_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%f Is not greater or equal to %f.", (val1), (val2) \
    ))

#define ASSERT_DOUBLE_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%lf Is not greater or equal to %lf.", (val1), (val2) \
    ))

#define ASSERT_HEX_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "0x%X Is not greater or equal to 0x%X. (" \
        LUKIP_INT_FMT " Is not greater or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_GREATER_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "0x%X Is not greater or equal to 0x%X. (" \
        LUKIP_UINT_FMT " Is not greater or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_GREATER_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) >= (val2), LINE_INFO, \
        "%b Is not greater or equal to %b. (" \
        LUKIP_INT_FMT " Is not greater or equal to " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_GREATER_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) >= (val2), LINE_INFO, \
        "%b Is not greater or equal to %b. (" \
        LUKIP_UINT_FMT " Is not greater or equal to " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

// ========================== NUMBER LESS ==========================================================

#define ASSERT_INT_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%d Is not less than %d.", (val1), (val2)))

#define ASSERT_INT8_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRId8 " Is not less than %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRId16 " Is not less than %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRId32 " Is not less than %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRId64 " Is not less than %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%u Is not less than %u.", (val1), (val2)))

#define ASSERT_UINT8_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRIu8 " Is not less than %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRIu16 " Is not less than %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRIu32 " Is not less than %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, "%" PRIu64 " Is not less than %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%li Is not less than %li.", (val1), (val2)))

#define ASSERT_ULONG_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%lu Is not less than %lu.", (val1), (val2)))

#define ASSERT_SIZE_T_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%zu Is not less than %zu.", (val1), (val2)))

#define ASSERT_FLOAT_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%f Is not less than %f.", (val1), (val2)))

#define ASSERT_DOUBLE_LESS(val1, val2) \
    (verify_condition((val1) < (val2), LINE_INFO, "%lf Is not less than %lf.", (val1), (val2)))

#define ASSERT_HEX_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, \
        "0x%X Is not less than 0x%X. (" \
        LUKIP_INT_FMT " Is not less than " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_LESS(val1, val2) \
    (verify_condition( \
        (val1) < (val2), LINE_INFO, \
        "0x%X Is not less than 0x%X. (" \
        LUKIP_UINT_FMT " Is not less than " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_LESS(val1, val2) \
    (verify_binary( \
        (val1) < (val2), LINE_INFO, \
        "%b Is not less than %b. (" \
        LUKIP_INT_FMT " Is not less than " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_LESS(val1, val2) \
    (verify_binary( \
        (val1) < (val2), LINE_INFO, \
        "%b Is not less than %b. (" \
        LUKIP_UINT_FMT " Is not less than " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

// ============================ LESS OR EQUAL ======================================================

#define ASSERT_INT_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%d Is not less or equal to %d.", (val1), (val2) \
    ))

#define ASSERT_INT8_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRId8 " Is not less or equal to %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRId16 " Is not less or equal to %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRId32 " Is not less or equal to %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRId64 " Is not less or equal to %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%u Is not less or equal to %u.", (val1), (val2) \
        ))

#define ASSERT_UINT8_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRIu8 " Is not less or equal to %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRIu16 " Is not less or equal to %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRIu32 " Is not less or equal to %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "%" PRIu64 " Is not less or equal to %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%li Is not less or equal to %li.", (val1), (val2) \
        ))

#define ASSERT_ULONG_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%lu Is not less or equal to %lu.", (val1), (val2) \
        ))

#define ASSERT_SIZE_T_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%zu Is not less or equal to %zu.", (val1), (val2) \
        ))

#define ASSERT_FLOAT_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%f Is not less or equal to %f.", (val1), (val2) \
        ))

#define ASSERT_DOUBLE_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, "%lf Is not less or equal to %lf.", (val1), (val2) \
    ))

#define ASSERT_HEX_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "0x%X Is not less or equal to 0x%X. (" \
        LUKIP_INT_FMT " Is not less or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_LESS_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "0x%X Is not less or equal to 0x%X. (" \
        LUKIP_UINT_FMT " Is not less or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_LESS_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) <= (val2), LINE_INFO, \
        "%b Is not less or equal to %b. (" \
        LUKIP_INT_FMT " Is not less or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_LESS_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) <= (val2), LINE_INFO, \
        "%b Is not less or equal to %b. (" \
        LUKIP_UINT_FMT " Is not less or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

// ========================= NUMBER NOT EQUAL ======================================================

#define ASSERT_INT_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%d Is not different from %d.", (val1), (val2) \
    ))

#define ASSERT_INT8_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRId8 " Is not different from %" PRId8 ".", (val1), (val2) \
    ))

#define ASSERT_INT16_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRId16 " Is not different from %" PRId16 ".", (val1), (val2) \
    ))

#define ASSERT_INT32_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRId32 " Is not different from %" PRId32 ".", (val1), (val2) \
    ))

#define ASSERT_INT64_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRId64 " Is not different from %" PRId64 ".", (val1), (val2) \
    ))

#define ASSERT_UINT_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%u Is not different from %u.", (val1), (val2) \
    ))

#define ASSERT_UINT8_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRIu8 " Is not different from %" PRIu8 ".", (val1), (val2) \
    ))

#define ASSERT_UINT16_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRIu16 " Is not different from %" PRIu16 ".", (val1), (val2) \
    ))

#define ASSERT_UINT32_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRIu32 " Is not different from %" PRIu32 ".", (val1), (val2) \
    ))

#define ASSERT_UINT64_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%" PRIu64 " Is not different from %" PRIu64 ".", (val1), (val2) \
    ))

#define ASSERT_LONG_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%li Is not different from %li.", (val1), (val2) \
    ))

#define ASSERT_ULONG_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%lu Is not different from %lu.", (val1), (val2) \
    ))

#define ASSERT_DOUBLE_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%lf Is not different from %lf.", (val1), (val2) \
    ))

#define ASSERT_HEX_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "0x%X Is not different from 0x%X. (" \
        LUKIP_INT_FMT " Is not different from " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UHEX_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "0x%X Is not different from 0x%X. (" \
        LUKIP_UINT_FMT " Is not different from " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_BINARY_NOT_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) != (val2), LINE_INFO, \
        "%s Is not different from %s. (" \
        LUKIP_INT_FMT " Is not different from " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    ))

#define ASSERT_UBINARY_NOT_EQUAL(val1, val2) \
    (verify_binary( \
        (val1) != (val2), LINE_INFO, \
        "%s Is not different from %s. (" \
        LUKIP_UINT_FMT " Is not different from " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    ))

#define ASSERT_SIZE_T_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%zu Is not different from %zu.", (val1), (val2) \
    ))

#define ASSERT_FLOAT_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%f Is not different from %f.", (val1), (val2) \
    ))

// ======================= TRUE (without comparisons) ==============================================

#define ASSERT_WITHIN_EQUAL(val1, val2, precision) \
    (verify_precision(val1, val2, precision, LINE_INFO, ASSERT_EQUAL))

#define ASSERT_CHAR_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "'%c' Does not equal '%c'.", (val1), (val2)))

#define ASSERT_BOOL_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%s Does not equal %s.", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    ))

#define ASSERT_ADDRESS_EQUAL(val1, val2) \
    (verify_condition((val1) == (val2), LINE_INFO, "%p Does not equal %p.", (val1), (val2)))

#define ASSERT_BYTES_EQUAL(val1, val2, length) \
    (verify_bytes_array(val1, val2, length, LINE_INFO, ASSERT_EQUAL))

#define ASSERT_STRING_EQUAL(val1, val2) \
    (verify_strings(val1, val2, LINE_INFO, ASSERT_EQUAL))

#define ASSERT_NULL(val) \
    (verify_condition((val) == NULL, LINE_INFO, "%p Does not equal NULL.", (val)))

// ======================= FALSE (without comparisons) =============================================

#define ASSERT_WITHIN_NOT_EQUAL(val1, val2, precision) \
    (verify_precision(val1, val2, precision, LINE_INFO, ASSERT_NOT_EQUAL))

#define ASSERT_CHAR_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "'%c' Is not different from '%c'.", (val1), (val2) \
    ))

#define ASSERT_BOOL_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%s == %s.", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    ))

#define ASSERT_ADDRESS_NOT_EQUAL(val1, val2) \
    (verify_condition( \
        (val1) != (val2), LINE_INFO, "%p Is not different from %p.", (val1), (val2) \
    ))

#define ASSERT_BYTES_NOT_EQUAL(val1, val2, length) \
    (verify_bytes_array(val1, val2, length, LINE_INFO, ASSERT_NOT_EQUAL))

#define ASSERT_STRING_NOT_EQUAL(val1, val2) \
    (verify_strings(val1, val2, LINE_INFO, ASSERT_NOT_EQUAL))

#define ASSERT_NOT_NULL(val) \
    (verify_condition((val) != NULL, LINE_INFO, "%p Is not different from NULL.", (val)))

// =================================== RAISE =======================================================

#define ASSERT_RAISE_FAIL() (raise_assert(RAISE_FAIL, LINE_INFO, "Failure raised."))

#define ASSERT_RAISE_FAIL_MESSAGE(...) (raise_assert(RAISE_FAIL, LINE_INFO, __VA_ARGS__))

#define ASSERT_RAISE_WARN() (raise_assert(RAISE_WARN, LINE_INFO, "Warning raised."))

#define ASSERT_RAISE_WARN_MESSAGE(...) (raise_assert(RAISE_WARN, LINE_INFO, __VA_ARGS__))

// =============================== MISCELLANEOUS ===================================================

#define ASSERT_TRUE(val) \
    (verify_condition((val) == true, LINE_INFO, "false Does not equal true."))

#define ASSERT_FALSE(val) \
    (verify_condition((val) == false, LINE_INFO, "true Does not equal false."))

#define ASSERT_CONDITION(condition) \
    (verify_condition((condition) == true, LINE_INFO, "Condition failed."))

#define ASSERT_CUSTOM(condition, ...) \
    (verify_condition((condition) == true, LINE_INFO, __VA_ARGS__))

#endif
