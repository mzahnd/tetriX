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
 * @file    rpcontrol.h
 * 
 * @brief   Raspberry module. It works with audio, display and joystick 
 *          control to show on raspberry pi the game(backend).
 * 
 * @details It is the main file of raspberry control
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    13/12/2019, 13:43
 * 
 * @copyright GNU General Public License v3
 */

#ifndef RPCONTROL_H
#    define RPCONTROL_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

///@def NO_ERRROR
///@brief No errors.
#define NO_ERROR 0

///@def ERROR
///@brief Error value, it indicates that something went wrong.
#define ERROR -1

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Run game on Raspberry PI.
int
rpi (void);

#endif /* RPCONTROL_H */
