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
 * @file    board.h
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
 */

#ifndef BOARD_H
#    define BOARD_H

// === Libraries and header files ===

#    include "../stats/stats_mgmt.h"

// === Constants and Macro definitions ===
/// @def NUM_PIECES
/// @brief How many pieces are in the game
#    define NUM_PIECES 7

#    define PIECE_NONE  '\0'

/// @def PIECE_I
/// @brief Piece I definition used to fill a bag.
#    define PIECE_I     'I'

/// @def PIECE_J
/// @brief Piece J definition used to fill a bag.
#    define PIECE_J     'J'

/// @def PIECE_L
/// @brief Piece L definition used to fill a bag.
#    define PIECE_L     'L'

/// @def PIECE_O
/// @brief Piece O definition used to fill a bag.
#    define PIECE_O     'O'

/// @def PIECE_S
/// @brief Piece S definition used to fill a bag.
#    define PIECE_S     'S'

/// @def PIECE_T
/// @brief Piece T definition used to fill a bag.
#    define PIECE_T     'T'

/// @def PIECE_Z
/// @brief Piece Z definition used to fill a bag.
#    define PIECE_Z     'Z'

/// @def BOARD_HEIGHT
/// @brief Board Height
#    define BOARD_HEIGHT    20

/// @def BOARD_WIDTH
/// @brief Board Width
#    define BOARD_WIDTH     10

#    define INACTIVE_COORD  -1

// === Enumerations, structures and typedefs ===
enum board_cell
{
    MOVING = -1,
    CLEAR = 0,
    FIXED
};

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

const stats_t *
ask_stats (void);

const char *
ask_board (void);
#endif /* BOARD_H */
