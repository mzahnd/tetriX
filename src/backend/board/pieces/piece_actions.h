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
 * @file    piece_actions.h
 * 
 * @brief   Manage a piece actions like rotation, shifting and dropping.
 * 
 * Using PIECE structure a given piece can be rotated, shifted and droped using
 * soft drop. Also it's coordinates can be retrived for "real time"
 * representation of it in the board.
 * 
 * @note piece_init() <b>must</b> be called before accessing the structure.
 * If not done so, the structure behaviour is undefined.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    28/11/2019, 18:31
 * 
 * @copyright GNU General Public License v3
 */

#ifndef PIECE_ACTIONS_H
#    define PIECE_ACTIONS_H

// === Libraries and header files ===
// For BLOCKS, COORD_NUM
#include "../board.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

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
 * @brief Piece object.
 * 
 * Contains a piece's type, actions it has to perform, and it's current 
 * position in the board.
 * 
 * @warning Function piece_init() must be called before using this object
 * 
 * @headerfile piece_actions.h
 */
typedef struct PIECE
{
    /// Type of piece. Set using definitions in board.h file.
    int type;

    /// Board coordinates of the piece. To access, use coords enum in board.h 

    struct
    {
        /// Board coordinates of the piece. To access, use coords enum in board.h    
        int coordinates[4][2];

    } get;

    /**
     * @brief Perform piece rotation
     * 
     * @param direction Direction to rotate the piece. Use shiftingTypes enum.
     * 
     * @return Nothing
     */
    void (* rotate) (int direction);

    /**
     * @brief Shift piece in the given direction
     * 
     * @param direction Direction to shift the piece. Use shiftingTypes enum.
     * 
     * @return Nothing
     */
    void (* shift) (int direction);

    /**
     * @brief Perform a soft drop
     * 
     * @param None
     * 
     * @return Nothing
     */
    void (* softDrop) (void);
    
    /**
     * @brief Update the board with this piece's information
     * 
     * @param None
     * 
     * @return Nothing
     */
    void (* update) (int * cellType);
    
    void (* destroy) (void);
} piece_t;

// === Global variables ===
extern struct GAMEBOARD * boardStr;

// === ROM Constant variables ===

// === Global function definitions ===
// Piece initialization
int
piece_init (struct PIECE * pstruct, struct GAMEBOARD * boardStr,
            int * board, int boardHeight, int boardWidth,
            const int piece);

#endif /* PIECE_ACTIONS_H */
