/******************************************************************************
 * Copyright (C) 2019 Martín E. Zahnd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

/**
 * 
 * @file    random_generator.h
 * 
 * @brief   Functions used to generate a bag of random pieces.
 * 
 * @details Generates an array (called "bag of pieces") of size NUM_PIECES 
 * (defined in header board.h) with random pieces following the set of rules
 * taken from tetris.wiki webpage:
 *  - No more than 4 S or Z pieces togheter.
 *  - No more than 12 pieces between two different I pieces.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    28/11/2019, 10:58
 * 
 * @see https://tetris.wiki/Random_Generator
 *  
 * @copyright GNU General Public License v3
 */

#ifndef RANDOM_GENERATOR_H
#    define RANDOM_GENERATOR_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Generates a NUM_PIECES size bag with a pseudo-random list of pieces.
void
random_generator (int * rnd_bag, int size);

#endif /* RANDOM_GENERATOR_H */
