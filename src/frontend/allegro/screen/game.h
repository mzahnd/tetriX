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
 * @file    game.h
 * 
 * @brief   Play the game using Allegro
 * 
 * @details Runs the entire game using Allegro
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    17/12/2019, 11:29
 * 
 * @copyright GNU General Public License v3
 */

#ifndef GAMESCREEN_H
#    define GAMESCREEN_H 1

// === Libraries and header files ===
// For allegro_t
#    include "../alcontrol.h"

// === Constants and Macro definitions ===

// Text options
/// Game regular text size
#    define GAME_TXT_SIZE       32
/// Game bold text size
#    define GAME_TXT_SIZE_BOLD  32
/// Game text color
#    define GAME_TXT_COLOR      "#FFFFFF"
/// Game regular font path
#    define GAME_TXT_FONT_PATH  \
                          "res/fonts/pixel-operator/PixelOperatorMono.ttf"
/// Game bold font path
#    define GAME_TXT_FONT_BOLD_PATH  \
                          "res/fonts/pixel-operator/PixelOperatorMono-Bold.ttf"

/// Space between lines of text
#    define GAME_TXT_OFFSET     10

/// Number of start levels the user can pick (from 0 to NLEVLS - 1)
#    define NLEVELS             10
/// Number of start rows the user can pick (from 0 to NLEVLS - 1)
#    define NROWS               NLEVELS

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Starts the game while running in Allegro mode
int
alg_game (allegro_t * alStru);

#endif /* GAMESCREEN_H */
