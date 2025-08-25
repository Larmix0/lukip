/**
 * @file lukip_asserts.h
 * @brief Header for displaying output of a Lukip unit-testing program.
 * 
 * @author Larmix
 */

#ifndef LUKIP_OUTPUT_H
#define LUKIP_OUTPUT_H

#include "lukip_assert.h"

/**
 * @brief Show results of a Lukip unit program.
 * 
 * @param lukip The lukip unit to show the results of.
 */
void lkp_show_results(const LukipUnit *lukip);

#endif
