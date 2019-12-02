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
// For BOARD_WIDTH definition in this header and PIECE_X in the .c file
#    include "../board.h"

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
 * @note When setting a value to @verbatim true @endverbatim or 
 * @verbatim false @endverbatim use stdbool.h
 * 
 * @warning Function piece_init() must be called before using this object
 * 
 * @headerfile piece_actions.h
 */
typedef struct PIECE
{
    /**
     * @brief Perform piece rotation
     * 
     *  rotation.status must be also set to true
     * 
     * @param board Board where the piece must be updated
     * @param boardHeight Board height.
     */
    void (* rotate) (int board [][BOARD_WIDTH], const int boardHeight);

    /**
     * @brief Update the board with this piece's information
     * 
     * @param boardStr board_t structure with board's information
     * @param board Board where the piece must be updated
     * @param boardHeight Board height.
     */
    void (* update) (struct GAMEBOARD * boardStr,
                     int board [][BOARD_WIDTH], const int boardHeight);

    /// Type of piece. Set using definitions in board.h file.
    char type;

    /// True: perform soft drop ; False: no soft drop
    int drop;
    /// LEFT or RIGHT: perform shifting ; NONE: don't perform any shifting
    int shifting;

    /// Rotation status (true or false) and which is the piece's position

    struct
    {
        /// True: user asked for piece rotation ; False: don't rotate the piece
        int status;
        /// Piece position (from 1 to 4). Do not manually edit this variable
        int position;
    } rotation;

    /// Board coordinates of the piece. Use coords enum in board.h

    struct
    {
        /// Piece's block 1
        int b1[COORD_NUM];
        /// Piece's block 2
        int b2[COORD_NUM];
        /// Piece's block 3
        int b3[COORD_NUM];
        /// Piece's block 4
        int b4[COORD_NUM];
    } coord;

} piece_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
// Piece initialization
int
piece_init (struct PIECE * pstruct,
            const char * bag, const int position);

#endif /* PIECE_ACTIONS_H */
