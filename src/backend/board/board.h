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
// For LEFT and RIGHT directions
#    include "pieces/piece_actions.h"

// === Constants and Macro definitions ===
/// @def BOARD_HEIGHT
/// @brief Board Height
#    define BOARD_HEIGHT    20

/// @def BOARD_WIDTH
/// @brief Board Width
#    define BOARD_WIDTH     10

#    define ORIENTATION     4

// === Enumerations, structures and typedefs ===

enum block
{
    b1 = 0,
    b2,
    b3,
    b4,
    BLOCKS
};

enum pieces
{
    TETROMINO_NONE = -1,
    TETROMINO_I = 0,
    TETROMINO_J,
    TETROMINO_L,
    TETROMINO_O,
    TETROMINO_S,
    TETROMINO_T,
    TETROMINO_Z,
    TETROMINOS
};

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
    CELL_I,
    CELL_J,
    CELL_L,
    CELL_O,
    CELL_S,
    CELL_T,
    CELL_Z,
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
        /// How fast is the board being updated in Frames Per Gridcell
        int (* FPG) (void);
        /// Get coordinate (0,0) of the board (top-left)
        int * (* board) (void);

        int (* filledLines) (int arr[BOARD_HEIGHT]);

        int (* endGame) (void);
    } ask;

    /// Clear a specific part of the board

    struct
    {
        /// Clear all filled lines. Intended to be called after an animation
        void (* line) (int line);
    } clear;

    /// Manage the current piece

    struct
    {

        /*/// Clear pieces that are...

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
            void (* fixed) (int cellType);
        } set;*/

        struct
        {
            void (* rotate) (int direction);
            void (* shift) (int direction);
            void (* softDrop) (void);
        } action;

    } piece;

    void (* update) (void);

} board_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize a GAMEBOARD structure
void
board_init (void * gameBoardStruct);

#endif /* BOARD_H */
