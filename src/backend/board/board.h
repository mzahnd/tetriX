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

// === Constants and Macro definitions ===
/// @def BOARD_HEIGHT
/// @brief Board Height
#    define BOARD_HEIGHT    20

/// @def BOARD_WIDTH
/// @brief Board Width
#    define BOARD_WIDTH     10

/**
 * @def ORIENTATION
 * @brief How many different orientations a piece can have
 */
#    define ORIENTATION     4

// === Enumerations, structures and typedefs ===

/**
 * @brief Tetromino's block number and how many blocks a tetromino has
 */
enum block
{
    b1 = 0,
    b2,
    b3,
    b4,
    BLOCKS
};

/**
 * @brief Tetrominos definitions and number of tetrominos
 */
enum piecesType
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
 * @brief Parameters to shift a piece.
 * 
 * One of this is passed to the shifting variable in PIECE structure.
 */
enum shiftingTypes
{
    /// Don't shift piece
    NONE,
    /// Shift piece one position to the left
    LEFT,
    /// Shift piece one position to the right
    RIGHT
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
    CELL_Z
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

/// Type of every grid in the board.
typedef int grid_t;

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
        grid_t * (* board) (void);

        /**
         * @brief How many rows are complete and which are those.
         * 
         * @param lines Array with the number of the rows that are filled.
         * 
         * @return Number of filled rows
         */
        int (* filledRows) (int lines[BOARD_HEIGHT]);

        /** 
         * @brief Ask if game has to finish
         * 
         * @return True: 1
         * @return False: 0
         */
        int (* endGame) (void);
        
        /**
         * @brief Get the current game stats.
         * 
         * @note This pointer must be casted to stats_t or STATS structure.
         */
        const void * (* stats) (void);
    } ask;

    /// Clear a specific part of the board

    struct
    {
        /**
         * @brief Clear a filled row.
         * 
         * Clears the given row and drops the board "as it is" once. This 
         * function it's intended to be called after an animation.
         * 
         * @param lines Array with all the filled rows.
         * @param position Position of the row to be cleared in @p lines array.
         * 
         * @return Nothing
         */
        void (* line) (int lines[BOARD_HEIGHT], int position);
    } clear;

    /// Manage the current piece

    struct
    {
        /**
         * @brief Rotate the piece in the given direction
         * 
         * Automatically performs a board update but does not fixes the piece
         * if the ratation cannot be performed.
         * 
         * @param direction LEFT or RIGHT, according to shiftingTypes enum
         * 
         * @return Nothing
         */
        void (* rotate) (int direction);

        /**
         * @brief Shift the piece in the given direction
         *
         * Automatically performs a board update but does not fixes the piece
         * if the shifting cannot be performed.
         * 
         * @param direction LEFT or RIGHT, according to shiftingTypes enum
         * 
         * @return Nothing
         */
        void (* shift) (int direction);

        /**
         * @brief Perform a soft drop of the piece
         * 
         * Automatically performs a board update.
         * 
         * @param None
         * 
         * @return Nothing
         */
        void (* softDrop) (void);
    } piece;

    /**
     * @brief Update the board
     * 
     * Updates the piece position in the board, dropping it once or fixing it
     * if necessary.
     * 
     * @param None
     * 
     * @return Nothing
     */
    void (* update) (void);
    
} board_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize a GAMEBOARD structure
void
board_init (void * gameBoardStruct);

#endif /* BOARD_H */
