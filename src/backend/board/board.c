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
 * @file    board.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 23:02
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "random_generator.h"

#include "pieces/piece_actions.h"

// This file
#include "board.h"

/// @privatesection
// === Constants and Macro definitions ===
// Board Height for the matrix. The additional 2 rows are hidden to the user
#define MBOARD_H     BOARD_H + 2 
// Board Width for the matrix.
#define MBOARD_W     BOARD_W

// === Enumerations, structures and typedefs ===

// === Global variables ===
char cgame[MBOARD_H][MBOARD_W];

piece_t piece;

stats_t stats;

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

void *
board (void)
{
    int pos;
    char bag[NUM_PIECES], tmp_bag[NUM_PIECES];

    board_init(bag, NUM_PIECES);

    while ( 1 )
    {
        //

        // Empty bag? New set of pieces


        // Change stats

    }
}

const stats_t *
ask_stats (void)
{
    return &stats;
}

const char *
ask_board (void)
{
    return &cgame[2][0];
}

/// @privatesection
// === Local function definitions ===

static char
board_init (char * bag, int size)
{
    if ( !init_random_generator() )
    {
        return 1;
    }

    // Generate first set of pieces
    random_generator(bag, size);

    piece.type = PIECE_NONE;

    stats.score.actual = 0;
    stats.next = PIECE_NONE;
    stats.pieces.I = 0;
    stats.pieces.J = 0;
    stats.pieces.L = 0;
    stats.pieces.O = 0;
    stats.pieces.S = 0;
    stats.pieces.T = 0;
    stats.pieces.Z = 0;

    stats.status = 0;

    /// FIXME
    stats.score.top = 1000;
    stats.level = 0;
    stats.lines = 0;

    return 0;
}

static int
update_board (char * bag, int position)
{
    if ( piece.type == PIECE_NONE )
    {
        piece_init(bag, position);
    }

    if ( piece.rotation.status == true )
    {
        piece.rotate();
    }

    if ( piece.drop == true )
    {
        piece.coord.old.b1[0] = INACTIVE_COORD;
        piece.update_board();

        // FIXME Add some timer here (FPG dependent)

        piece.drop = false;
    }

    if ( piece.shift != NONE )
    {
        piece.shift();
    }

    piece.update_board();

}

