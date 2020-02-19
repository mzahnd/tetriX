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
 * @file    endGame.h
 * 
 * @brief   Draw a screen after the game has ended.
 * 
 * @details Displays the top ten score table or an "arcade character selector"
 * for entering a name whenever a new top score has been made.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    24/01/2020, 12:22
 * 
 * @copyright GNU General Public License v3
 */

#ifndef ENDGAME_H
#    define ENDGAME_H 1

// === Libraries and header files ===

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Draw End Game screen with the top scores list in it.
int
alg_endGame (allegro_t * alStru, const stats_t * gameStats);

#endif /* ENDGAME_H */
