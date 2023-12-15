#ifndef LAR_UNIT_H
#define LAR_UNIT_H

#include "lar_unit_impl.h"

#define LAR_INIT() init_lar_unit()
#define LAR_END() end_lar_unit()

#define MAKE_SET_UP(setUpFunc) make_set_up(setUpFunc)
#define MAKE_TEAR_DOWN(tearDownFunc) make_tear_down(tearDownFunc)

#define TEST(FuncToTest) test_func(FuncToTest)

#define LAR_EQUAL(val1, val2) \
    check_condition( \
        val1 == val2, "Expected something but got something instead.", \
        __FILE__, (char*)__func__, __LINE__ \
    ); \

#endif
