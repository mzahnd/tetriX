/******************************************************************************
 * Copyright (C) 2020 Martín E. Zahnd
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
 * @file    gameStats.h
 * 
 * @brief   Draws the stats in the current game screen
 * 
 * @details While playing, functions in this file allow displaying all the 
 * stats in of the current game in the display.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    20/01/2020, 18:45
 * 
 * @copyright GNU General Public License v3
 */

#ifndef GAMESTATS_H
#    define GAMESTATS_H

// === Libraries and header files ===
// For stats_t
#    include "../../../backend/stats/stats_mgmt.h"
// For board_t 
#    include "../../../backend/board/board.h"
// For gbox_t
#    include "game.h"

// === Constants and Macro definitions ===

/**
 * @brief Stats shown in the display
 * 
 * @note This structure must be created and passed by reference to 
 * init_scrStats() before using any of the drawStats_*_f() functions
 */
typedef struct
{
    /// Game stats
    const stats_t * gStats;

    /// Score box
    gbox_t scoreBox;

    /// Current level box
    gbox_t levelBox;

    /// Lines cleared box
    gbox_t linesBox;

    /// Show next piece in its box
    bool showNextPiece;

    /// Next piece box
    gbox_t nextPieceBox;

    /// Pieces box
    gbox_t piecesBox;

} screenStats_t;

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize game stats displayed when using Allegro
void
init_scrStats (screenStats_t * stats, board_t * logic);

// Draw the current level box with its corresponding information.
void
drawStats_level_f (screenStats_t * stats);

// Draw the lines box with its corresponding information.
void
drawStats_lines_f (screenStats_t * stats);

// Draw the Next Piece box with its corresponding information
void
drawStats_nextPiece_f (screenStats_t * stats);

// Draw the pieces box with its corresponding information
void
drawStats_pieces_f (screenStats_t * stats);

// Draw the score box with its corresponding information
void
drawStats_score_f (screenStats_t * stats);

#endif /* GAMESTATS_H */
