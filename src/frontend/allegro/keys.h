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
 * @file    keys.h
 * 
 * @brief   Manages menu keys
 * 
 * @details
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    21/01/2020, 20:28
 * 
 * @copyright GNU General Public License v3
 */

#ifndef KEYS_H
#    define KEYS_H 1

// === Libraries and header files ===

// For menu_t
#    include "screen/menu.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Manages the main menu keys. Taking action if a valid one is pressed.
void
mainMenuKeys (menu_t * menu, unsigned char key[ALLEGRO_KEY_MAX]);

#endif /* KEYS_H */
