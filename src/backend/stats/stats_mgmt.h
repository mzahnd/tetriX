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
typedef struct
{
    /// Current level
    unsigned int level;
    /// Amount of lines cleared
    unsigned int lines;
    /// Next piece
    unsigned char next;

    /// How many pieces of each type have been deployed in this game.
    struct PIECES_STRC
    {
        /// How many I have been deployed
        unsigned int I;
        /// How many J have been deployed
        unsigned int J;
        /// How many L have been deployed
        unsigned int L;
        /// How many O have been deployed
        unsigned int O;
        /// How many S have been deployed
        unsigned int S;
        /// How many T have been deployed
        unsigned int T;
        /// How many Z have been deployed
        unsigned int Z;
    } pieces;

    /// Top and Actual scores
    struct SCORE_STRC
    {
        /// Top score
        unsigned int top;
        /// Actual score
        unsigned int actual;
    } score;

    /// Current game status (0 if running, 1 if ended)
    int status;
} stats_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

#endif /* STATS_MGMT_H */
