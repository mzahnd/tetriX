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
 * @file    audioControl.h
 * 
 * @brief   Audio controller for RPI.
 * 
 * @details It has all the functions and libraries used to play sound on RPI. 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    18/02/2020, 14:26
 * 
 * @copyright GNU General Public License v3
 */

#ifndef AUDIOCONTROL_H
#    define AUDIOCONTROL_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

/**
 * @brief Plays RPI music.
 * 
 * It plays the music of the game.
 * 
 * @param None
 * 
 * @return Nothing
 */
void playMusic(void);

/**
 * @brief Stops the music.
 * 
 * It stops the music of the game
 * 
 * @param None
 * 
 * @return Nothing
 */
void stopMusic(void);

/**
 * @brief Plays the music effects.
 * 
 * It plays the music effects.
 *
 * @param path Effect sound path.
 * 
 * @return Nothing
 */
void playFX(const char * path);

#endif /* AUDIOCONTROL_H */
