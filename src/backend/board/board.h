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

/// @def PIECE_NONE
/// @brief Definition to set a PIECE structure as not initialized or cleared.
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

// === Enumerations, structures and typedefs ===

/**
 * @brief Board cell definitions
 */
enum board_cell
{
    /// The cell has a block that is part of a moving piece
    CELL_MOVING = -1,
    /// The cell is clear
    CELL_CLEAR = 0,
    /// The cell has a fixed piece
    CELL_FIXED
};

/**
 * @brief Coordinates definitions
 */
enum coords
{
    /// X Coordinate
    COORD_X,
    /// Y Coordinate
    COORD_Y,
    /// Number of coordinates
    COORD_NUM
};

/**
 * @brief Current game's board object.
 * 
 * This structure gives access to different board actions to display it on the
 * front end of the program.
 * 
 * @warning board_init() must be called before using this structure.
 */
typedef struct GAMEBOARD
{
    /// Destroy current board. All structure information is deleted.
    void (* destroy) (void);

    /// Get board information

    struct
    {
        /// How fast is the board being updated in Frames Per Grid
        int (* FPG) (void);
        /// Get coordinate (0,0) of the board (top-left)
        int * (* board) (void);
    } ask;

    /// Clear a specific part of the board

    struct
    {
        /// Clear all filled lines. Intended to be called after an animation
        void (* line) (void);
    } clear;

    /// Manage the current piece

    struct
    {
        /// Clear pieces that are...

        struct
        {
            /// Clear pieces that are moving (should be only one)
            void (* moving) (void);
        } clear;

        struct
        {
            /// Set the piece's coordinates as CELL_MOVING
            void (* moving) (void);
            /// Set the piece's coordinates as CELL_FIXED
            void (* fixed) (void);
        } set;
    } piece;

} board_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize a GAMEBOARD structure
void
board_init (void * gameBoardStruct);

#endif /* BOARD_H */
