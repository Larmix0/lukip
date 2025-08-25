/**
 * @file included_tests.h
 * @brief Header file of tests that are included (to test that including other files works).
 * 
 * @author Larmix
 */

#ifndef LUKIP_INCLUDED_TESTS_H
#define LUKIP_INCLUDED_TESTS_H

#include "lukip.h"

/** Setup from a different file. */
DECLARE_SETUP(set_up2);

/** Teardown from a different file. */
DECLARE_TEARDOWN(tear_down2);

/** A random test from a different file. A string comparison test in this case. */
TEST_CASE(string_test2);

#endif
