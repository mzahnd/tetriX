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

/******************************************************************************
 * 
 * @file    random_generator.c
 * 
 * @brief   Functions used to generate a bag of random pieces.
 * 
 * @details Generates an array (called "bag of pieces") of size TETROMINOS 
 * (defined in @header board.h) with random pieces following the set of rules
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
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <time.h>       // for srand()

// For pieces, board_cell and coords enum
#include "board.h"

// This file
#include "random_generator.h"

/// @privatesection
// === Constants and Macro definitions ===

// If modified, change test_random_gen.c and @details in random_generator()
#define MAX_SZ      4
#define MAX_I       12

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Verify if random_generator() rules are being accomplished 
static char
chk_rnd (int * rnd_bag, int pos, int * old_bag);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize random generator
 * 
 * Must be called once before random_generator().
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
int
init_random_generator (void)
{
    srand(time(NULL));
    return 0;
}

/**
 * @brief Generates a TETROMINOS size bag with a pseudo-random list of pieces.
 * 
 * Bags are filled with random pieces with the following rules:
 *  - No more than 4 S or Z pieces togheter.
 *  - No more than 12 pieces between two different I pieces.
 * 
 * @param rnd_bag Array of size TETROMINOS
 * @param size Size of rnd_bag array. Used to verify if it's actually 
 * TETROMINOS
 * 
 * @return Nothing
 * 
 * @note init_random_generator() must be called before using this function for
 * the first time.
 */
void
random_generator (int * rnd_bag, int size)
{
    int i, piece;

    int old_bag[TETROMINOS];

    if ( size != TETROMINOS )
    {
        fputs("Error on bag size for random_generator()", stderr);
        return;
    }

    // Copy the last used bag.
    for ( i = 0; i < TETROMINOS; i++ )
    {
        old_bag[i] = rnd_bag[i];
    }

    // Generate pieces to fill the bag
    i = 0;
    while ( i < TETROMINOS )
    {
        piece = rand() % TETROMINOS;
        switch ( piece )
        {
            case 0:
                rnd_bag[i] = TETROMINO_I;
                break;
            case 1:
                rnd_bag[i] = TETROMINO_J;
                break;
            case 2:
                rnd_bag[i] = TETROMINO_L;
                break;
            case 3:
                rnd_bag[i] = TETROMINO_O;
                break;
            case 4:
                rnd_bag[i] = TETROMINO_S;
                break;
            case 5:
                rnd_bag[i] = TETROMINO_T;
                break;
            case 6:
                rnd_bag[i] = TETROMINO_Z;
                break;

            default:
                break;
        }

        // Verify if rules are being accomplished
        if ( !chk_rnd(rnd_bag, i, old_bag) )
        {
            i++;
        }
    }
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Verify if random_generator() rules are being accomplished 
 * 
 * First counts how many S / Z pieces are togheter and the distance between the 
 * two nearest I pieces, then, compares this values with the generator rules
 * and returns 0 if both are being accomplished.
 * 
 * @param rnd_bag Array of size TETROMINOS with the bag being generated.
 * @param pos Position in rnd_bag array with the piece to check.
 * @param old_bag Array of size TETROMINOS with the old bag with random pieces.
 * @return Success: 0 (All rules are being accomplished)
 * @return Fail: Non 0 (At least one of the rules is not accomplished)
 */
static char
chk_rnd (int * rnd_bag, int pos, int * old_bag)
{
    // Return
    int ans = 0;

    // Counters
    int i = 0, count_sz = 0;
    static int pieces_since_i = 0;

    // Count S and Z pieces in the old bag and the number of pieces between
    // the last I and the end of the bag.
    i = 0;
    while ( i < TETROMINOS )
    {
        // If an S or Z is found, increment counter
        if ( old_bag[i] == TETROMINO_S || old_bag[i] == TETROMINO_Z )
        {
            count_sz++;
        }

            // With any other piece, reset it
        else
        {
            count_sz = 0;
        }

        // If an I is found, (re)start counting 
        if ( old_bag[i] == TETROMINO_I )
        {
            pieces_since_i = 0;
        }

            // Keep counting while not finding another I
        else
        {
            pieces_since_i++;
        }

        i++;
    }

    // Verify the bag that's being generated
    i = 0;
    while ( i < pos )
    {
        // If an S or Z is found, increment counter
        if ( rnd_bag[i] == TETROMINO_S || rnd_bag[i] == TETROMINO_Z )
        {
            count_sz++;
        }

            // With any other piece, reset it
        else
        {
            count_sz = 0;
        }

        // If an I is found, (re)start counting 
        if ( rnd_bag[i] == TETROMINO_I )
        {
            pieces_since_i = 0;
        }

            // Keep counting while not finding another I
        else
        {
            pieces_since_i++;
        }

        i++;
    }

    // If there are more than MAX_I pieces between two I, modify the generated
    // piece to follow the generator rule.
    if ( pieces_since_i >= MAX_I )
    {
        rnd_bag[pos] = TETROMINO_I;
        pieces_since_i = 0;
    }

        // If an extra S or Z has been generated, ask for another
    else if ( count_sz >= MAX_SZ &&
              (rnd_bag[pos] == TETROMINO_S || rnd_bag[pos] == TETROMINO_Z) )
    {
        ans = 1;
    }

    return ans;
}
