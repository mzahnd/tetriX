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
 * @file    stats_mgmt.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:35
 * 
 * @copyright GNU General Public License v3
 */

#ifndef STATS_MGMT_H
#    define STATS_MGMT_H

// === Libraries and header files ===
// For TETROMINOS
#    include "../board/board.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

/**
 * @brief Holds current game statistics.
 * 
 * Read and write the current game stats in this structure. Includes the 
 * current level, how many lines have been cleared, the next piece, how many
 * pieces of each kind have been deployed and the top and actual scores.
 * 
 * @note For "next" use PIECE_ definitions in board.h
 * 
 * @headerfile stats_mgmt.h
 */
typedef struct STATS
{
    /**
     * @brief Set to true when the piece is initialized.
     * 
     * @note It is strongly recommended to check this parameter before calling
     * anything inside this structure.
     */
    bool init;

    /// Current level
    int level;

    /// Lines information

    struct
    {
        /// Amount of lines cleared
        int cleared;
    } lines;

    /// Piece information

    struct
    {
        /// Current piece
        int current;

        /// Next piece
        int next;

        /// How many pieces of each type have been deployed in this game.
        int number[TETROMINOS];
    } piece;

    /// Top and Actual scores

    struct
    {
        /// Top score
        int top;
        /// Actual score
        int actual;
    } score;

    /**
     * @brief Call when a soft drop has been performed
     * 
     * @param None
     * 
     * @return Nothing
     */
    void (* softDrop) (void);

    /**
     * @brief Update the stats with a new piece
     * 
     * @param cp Current Piece (using pieces enum in board.h)
     * @param np Next Piece (using pieces enum in board.h)
     * 
     * @return Nothing
     */
    void (* newPiece) (int * cp, int * np);

    /**
     * @brief Update scoring when a/some line(s) is/are cleared
     * 
     * @param rows Number of rows to be cleared
     * 
     * @return Nothing
     */
    void (* update) (int rows);

    /**
     * @brief Destroy current stats, erasing all the structure's information
     * 
     * @param None
     * 
     * @return Nothing
     */
    void (* destroy) (void);

} stats_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize a STATS structure
int
initStats (struct STATS * stats);

#endif /* STATS_MGMT_H */
