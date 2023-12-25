#ifndef LUKIP_H
#define LUKIP_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/lukip_asserts.h"

#define LUKIP_INIT() init_lukip()
#define LUKIP_END() end_lukip()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)
#define MAKE_TEST_SUITE(setUpFunc, tearDownFunc) \
    make_test_suite(setUpFunc, tearDownFunc)

#define LINE_INFO \
    (LineInfo){.fileName=__FILE__, .funcName=(char *)__func__, .line=__LINE__,}

#define TEST(FuncToTest) test_func(FuncToTest, LINE_INFO)

#define LUKIP_INT_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%d Does not equal %d.", (val1), (val2))

#define LUKIP_INT8_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRId8 " Does not equal " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRId16 " Does not equal " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRId32 " Does not equal " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRId64 " Does not equal " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%u Does not equal %u.", (val1), (val2))

#define LUKIP_UINT8_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRIu8 " Does not equal " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRIu16 " Does not equal " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRIu32 " Does not equal " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, PRIu64 " Does not equal " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%li Does not equal %li.", (val1), (val2))

#define LUKIP_ULONG_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%lu Does not equal %lu.", (val1), (val2))

#define LUKIP_SIZE_T_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%zu Does not equal %zu.", (val1), (val2))

#define LUKIP_FLOAT_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%f Does not equal %f.", (val1), (val2))

#define LUKIP_DOUBLE_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%lf Does not equal %lf.", (val1), (val2))

#define LUKIP_HEX_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X Does not equal 0x%X. (" LUKIP_INT_FMT " Does not equal " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "0x%X Does not equal 0x%X. (" \
        LUKIP_UINT_FMT " Does not equal " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_EQUAL(val1, val2) \
    verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%s Does not equal %s. (" LUKIP_INT_FMT " Does not equal " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_EQUAL(val1, val2) \
    verify_binary( \
        (val1) == (val2), LINE_INFO, \
        "%s Does not equal %s. (" LUKIP_UINT_FMT " Does not equal " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_INT_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%d Is not greater than %d.", (val1), (val2) \
    )

#define LUKIP_INT8_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRId8 " Is not greater than " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRId16 " Is not greater than " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRId32 " Is not greater than " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRId64 " Is not greater than " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%u Is not greater than %u.", (val1), (val2) \
    )

#define LUKIP_UINT8_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRIu8 " Is not greater than " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRIu16 " Is not greater than " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRIu32 " Is not greater than " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        PRIu64 " Is not greater than " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%li Is not greater than %li.", (val1), (val2) \
    )

#define LUKIP_ULONG_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%lu Is not greater than %lu.", (val1), (val2) \
    )

#define LUKIP_SIZE_T_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%zu Is not greater than %zu.", (val1), (val2) \
    )

#define LUKIP_FLOAT_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%f Is not greater than %f.", (val1), (val2) \
    )

#define LUKIP_DOUBLE_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, "%lf Is not greater than %lf.", (val1), (val2) \
    )

#define LUKIP_HEX_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "0x%X Is not greater than 0x%X. (" \
        LUKIP_INT_FMT " Is not greater than " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_GREATER(val1, val2) \
    verify_condition( \
        (val1) > (val2), LINE_INFO, \
        "0x%X Is not greater than 0x%X. (" \
        LUKIP_UINT_FMT " Is not greater than " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_GREATER(val1, val2) \
    verify_binary( \
        (val1) > (val2), LINE_INFO, \
        "%s Is not greater than %s. (" \
        LUKIP_INT_FMT " Is not greater than " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_GREATER(val1, val2) \
    verify_binary( \
        (val1) > (val2), LINE_INFO, \
        "%s Is not greater than %s. (" \
        LUKIP_UINT_FMT " Is not greater than " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_INT_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, "%d Is not greater or equal to %d.", (val1), (val2) \
    )

#define LUKIP_INT8_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRId8 " Is not greater or equal to " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRId16 " Is not greater or equal to " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRId32 " Is not greater or equal to " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRId64 " Is not greater or equal to " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%u Is not greater or equal to %u.", (val1), (val2) \
    )

#define LUKIP_UINT8_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRIu8 " Is not greater or equal to " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRIu16 " Is not greater or equal to " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRIu32 " Is not greater or equal to " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        PRIu64 " Is not greater or equal to " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%li Is not greater or equal to %li.", (val1), (val2) \
    )

#define LUKIP_ULONG_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%lu Is not greater or equal to %lu.", (val1), (val2) \
    )

#define LUKIP_SIZE_T_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%zu Is not greater or equal to %zu.", (val1), (val2) \
    )

#define LUKIP_FLOAT_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%f Is not greater or equal to %f.", (val1), (val2) \
    )

#define LUKIP_DOUBLE_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "%lf Is not greater or equal to %lf.", (val1), (val2) \
    )

#define LUKIP_HEX_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "0x%X Is not greater or equal to 0x%X. (" \
        LUKIP_INT_FMT " Is not greater or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_GREATER_EQUAL(val1, val2) \
    verify_condition( \
        (val1) >= (val2), LINE_INFO, \
        "0x%X Is not greater or equal to 0x%X. (" \
        LUKIP_UINT_FMT " Is not greater or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_GREATER_EQUAL(val1, val2) \
    verify_binary( \
        (val1) >= (val2), LINE_INFO, \
        "%s Is not greater or equal to %s. (" \
        LUKIP_INT_FMT " Is not greater or equal to " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_GREATER_EQUAL(val1, val2) \
    verify_binary( \
        (val1) >= (val2), LINE_INFO, \
        "%s Is not greater or equal to %s. (" \
        LUKIP_UINT_FMT " Is not greater or equal to " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_INT_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%d Is not less than %d.", (val1), (val2))

#define LUKIP_INT8_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRId8 " Is not less than " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRId16 " Is not less than " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRId32 " Is not less than " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRId64 " Is not less than " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%u Is not less than %u.", (val1), (val2))

#define LUKIP_UINT8_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRIu8 " Is not less than " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRIu16 " Is not less than " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRIu32 " Is not less than " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, PRIu64 " Is not less than " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%li Is not less than %li.", (val1), (val2))

#define LUKIP_ULONG_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%lu Is not less than %lu.", (val1), (val2))

#define LUKIP_SIZE_T_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%zu Is not less than %zu.", (val1), (val2))

#define LUKIP_FLOAT_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%f Is not less than %f.", (val1), (val2))

#define LUKIP_DOUBLE_LESS(val1, val2) \
    verify_condition((val1) < (val2), LINE_INFO, "%lf Is not less than %lf.", (val1), (val2))

#define LUKIP_HEX_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, \
        "0x%X Is not less than 0x%X. (" \
        LUKIP_INT_FMT " Is not less than " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_LESS(val1, val2) \
    verify_condition( \
        (val1) < (val2), LINE_INFO, \
        "0x%X Is not less than 0x%X. (" \
        LUKIP_UINT_FMT " Is not less than " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_LESS(val1, val2) \
    verify_binary( \
        (val1) < (val2), LINE_INFO, \
        "%s Is not less than %s. (" \
        LUKIP_INT_FMT " Is not less than " LUKIP_INT_FMT ").", \
         (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_LESS(val1, val2) \
    verify_binary( \
        (val1) < (val2), LINE_INFO, \
        "%s Is not less than %s. (" \
        LUKIP_UINT_FMT " Is not less than " LUKIP_UINT_FMT ").", \
         (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

// ============================== LESS OR EQUAL ================================

#define LUKIP_INT_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%d Is not less or equal to %d.", (val1), (val2) \
    )

#define LUKIP_INT8_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRId8 " Is not less or equal to " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRId16 " Is not less or equal to " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRId32 " Is not less or equal to " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRId64 " Is not less or equal to " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%u Is not less or equal to %u.", (val1), (val2) \
        )

#define LUKIP_UINT8_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRIu8 " Is not less or equal to " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRIu16 " Is not less or equal to " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRIu32 " Is not less or equal to " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        PRIu64 " Is not less or equal to " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%li Is not less or equal to %li.", (val1), (val2) \
        )

#define LUKIP_ULONG_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%lu Is not less or equal to %lu.", (val1), (val2) \
        )

#define LUKIP_SIZE_T_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%zu Is not less or equal to %zu.", (val1), (val2) \
        )

#define LUKIP_FLOAT_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%f Is not less or equal to %f.", (val1), (val2) \
        )

#define LUKIP_DOUBLE_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, "%lf Is not less or equal to %lf.", (val1), (val2) \
    )

#define LUKIP_HEX_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "0x%X Is not less or equal to 0x%X. (" \
        LUKIP_INT_FMT " Is not less or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_LESS_EQUAL(val1, val2) \
    verify_condition( \
        (val1) <= (val2), LINE_INFO, \
        "0x%X Is not less or equal to 0x%X. (" \
        LUKIP_UINT_FMT " Is not less or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_LESS_EQUAL(val1, val2) \
    verify_binary( \
        (val1) <= (val2), LINE_INFO, \
        "%s Is not less or equal to %s. (" \
        LUKIP_INT_FMT " Is not less or equal to " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_LESS_EQUAL(val1, val2) \
    verify_binary( \
        (val1) <= (val2), LINE_INFO, \
        "%s Is not less or equal to %s. (" \
        LUKIP_UINT_FMT " Is not less or equal to " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_INT_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%d Is not different from %d.", (val1), (val2) \
    )

#define LUKIP_INT8_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRId8 " Is not different from " PRId8 ".", (val1), (val2) \
    )

#define LUKIP_INT16_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRId16 " Is not different from " PRId16 ".", (val1), (val2) \
    )

#define LUKIP_INT32_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRId32 " Is not different from " PRId32 ".", (val1), (val2) \
    )

#define LUKIP_INT64_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRId64 " Is not different from " PRId64 ".", (val1), (val2) \
    )

#define LUKIP_UINT_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%u Is not different from %u.", (val1), (val2) \
    )

#define LUKIP_UINT8_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRIu8 " Is not different from " PRIu8 ".", (val1), (val2) \
    )

#define LUKIP_UINT16_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRIu16 " Is not different from " PRIu16 ".", (val1), (val2) \
    )

#define LUKIP_UINT32_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRIu32 " Is not different from " PRIu32 ".", (val1), (val2) \
    )

#define LUKIP_UINT64_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        PRIu64 " Is not different from " PRIu64 ".", (val1), (val2) \
    )

#define LUKIP_LONG_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%li Is not different from %li.", (val1), (val2) \
    )

#define LUKIP_ULONG_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%lu Is not different from %lu.", (val1), (val2) \
    )

#define LUKIP_DOUBLE_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%lf Is not different from %lf.", (val1), (val2) \
    )

#define LUKIP_HEX_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "0x%X Is not different from 0x%X. (" \
        LUKIP_INT_FMT " Is not different from " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UHEX_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "0x%X Is not different from 0x%X. (" \
        LUKIP_UINT_FMT " Is not different from " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_BINARY_NOT_EQUAL(val1, val2) \
    verify_binary( \
        (val1) != (val2), LINE_INFO, \
        "%s Is not different from %s. (" \
        LUKIP_INT_FMT " Is not different from " LUKIP_INT_FMT ").", \
        (val1), (val2), (LukipInt)(val1), (LukipInt)(val2) \
    )

#define LUKIP_UBINARY_NOT_EQUAL(val1, val2) \
    verify_binary( \
        (val1) != (val2), LINE_INFO, \
        "%s Is not different from %s. (" \
        LUKIP_UINT_FMT " Is not different from " LUKIP_UINT_FMT ").", \
        (val1), (val2), (LukipUnsigned)(val1), (LukipUnsigned)(val2) \
    )

#define LUKIP_SIZE_T_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%zu Is not different from %zu.", (val1), (val2) \
    )

#define LUKIP_FLOAT_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%f Is not different from %f.", (val1), (val2) \
    )

// Things that don't have comparisons (only equal and not equal).
#define LUKIP_EQUAL_WITHIN(val1, val2, precision) \
    verify_precision( \
        val1, val2, precision, LINE_INFO, ASSERT_EQUAL, \
        LUKIP_FLOAT_FMT " Does not equal " LUKIP_FLOAT_FMT " within %d places.", \
        (LukipFloat)(val1), (LukipFloat)(val2), (precision), ASSERT_EQUAL \
    )

#define LUKIP_CHAR_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%c Does not equal %c.", (val1), (val2))

#define LUKIP_BOOL_EQUAL(val1, val2) \
    verify_condition( \
        (val1) == (val2), LINE_INFO, \
        "%s Does not equal %s.", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LUKIP_ADDRESS_EQUAL(val1, val2) \
    verify_condition((val1) == (val2), LINE_INFO, "%p Does not equal %p.", (val1), (val2))

#define LUKIP_BYTES_EQUAL(val1, val2, length) \
    verify_bytes_array(val1, val2, length, LINE_INFO, ASSERT_EQUAL)

#define LUKIP_STRING_EQUAL(val1, val2) \
    verify_strings(val1, val2, LINE_INFO, ASSERT_EQUAL)

#define LUKIP_IS_TRUE(val) \
    verify_condition((val) == true, LINE_INFO, "false Does not equal true.")

#define LUKIP_IS_FALSE(val) \
    verify_condition((val) == false, LINE_INFO, "true Does not equal false.")

#define LUKIP_IS_NULL(val) \
    verify_condition((val) == NULL, LINE_INFO, "%p Does not equal NULL.", (val))

#define LUKIP_NOT_EQUAL_WITHIN(val1, val2, precision) \
    verify_precision( \
        val1, val2, precision, LINE_INFO, ASSERT_NOT_EQUAL, \
        LUKIP_FLOAT_FMT " Is not different from " LUKIP_FLOAT_FMT " within %d places.", \
        (LukipFloat)(val1), (LukipFloat)(val2), (precision) \
    )

#define LUKIP_CHAR_NOT_EQUAL(val1, val2) \
    verify_condition((val1) != (val2), LINE_INFO, "%c == %c.", (val1), (val2))

#define LUKIP_BOOL_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, \
        "%s == %s.", \
        (val1) == true ? "true" : "false", \
        (val2) == true ? "true" : "false" \
    )

#define LUKIP_ADDRESS_NOT_EQUAL(val1, val2) \
    verify_condition( \
        (val1) != (val2), LINE_INFO, "%p Is not different from %p.", (val1), (val2) \
    )

#define LUKIP_BYTES_NOT_EQUAL(val1, val2, length) \
    verify_bytes_array(val1, val2, length, LINE_INFO, ASSERT_NOT_EQUAL)

#define LUKIP_STRING_NOT_EQUAL(val1, val2) \
    verify_strings(val1, val2, LINE_INFO, ASSERT_NOT_EQUAL)

#define LUKIP_IS_NOT_NULL(val) \
    verify_condition((val) != NULL, LINE_INFO, "%p Is not different from NULL.", (val))

#define LUKIP_IS_CONDITION(condition) \
    verify_condition((condition) == true, LINE_INFO, "Condition failed.")

#define LUKIP_CUSTOM(condition, format, ...) \
    verify_condition((condition) == true, LINE_INFO, format, __VA_ARGS__)

#endif
