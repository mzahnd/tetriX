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

#include "../stats/stats_mgmt.h"

// This file
#include "board.h"

/// @privatesection
// === Constants and Macro definitions ===


// === Enumerations, structures and typedefs ===

// === Global variables ===
char rnd_bag[NUM_PIECES];

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===


// === Global function definitions ===
/// @publicsection



/// @privatesection
// === Local function definitions ===

static char
board_init (stats_t * stats)
{
    stats -> score.actual = 0;
    stats -> next = '\0';
    stats -> pieces.I = 0;
    stats -> pieces.J = 0;
    stats -> pieces.L = 0;
    stats -> pieces.O = 0;
    stats -> pieces.S = 0;
    stats -> pieces.T = 0;
    stats -> pieces.Z = 0;

    /// FIXME
    stats -> score.top = 1000;
    stats -> level = 0;
    stats -> lines = 0;

    return 0;
}

