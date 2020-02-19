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
 * @file    menu.h
 * 
 * @brief   Main menu when running with Allegro
 * 
 * @details Displays the main menu to the user
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    17/12/2019, 12:26
 * 
 * @copyright GNU General Public License v3
 */

#ifndef MENU_H
#    define MENU_H 1

// === Libraries and header files ===
// For allegro_t
#    include "../alcontrol.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

/// Options displayed in Main Menu screen
enum menu_options
{
    /// Invalid option
    NONE = 0,
    /// Play
    PLAY,
    /// About
    ABOUT,
    /// Exit game
    EXIT
};

/**
 * @brief Main Menu structure
 * 
 * Garants some basic control over the Main Menu screen. Meant to be used with
 * mainMenuKeys().
 */
typedef struct
{
    /// Selected option in the menu ( 0 to menu_options-1 )
    int txt_sel;

    /// Action to perform (use menu options)
    int action;

    /// Main Menu screen has to be redrawn
    bool redraw;
    
} menu_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Display game menu
int
alg_menu (allegro_t * alStru);

#endif /* MENU_H */
