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
 * @file    joystick.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    10/12/2019, 17:51
 * 
 * @copyright GNU General Public License v3
 */

#ifndef JOYSTICK_H
#    define JOYSTICK_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

///@def DIRECTION LIMIT
///@brief Up to this number, it will be considered a 
///       right,left,top or bottom signal.
#    define DIRECTION_LIMIT 100

///@def CENTER LIMIT
///@brief From 0 until this limit, it will be considered a center signal.
#    define CENTER_LIMIT 60

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

/**
 * @brief Analyze the joystick status.
 * 
 * It gives the user the Joystick and Joyswitch status.
 * 
 * @param None.
 * 
 * @return Joystick or Joyswitch status.
 */

int
surf(void);

#endif /* JOYSTICK_H */
