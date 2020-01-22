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

/******************************************************************************
 * 
 * @file    keys.c
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
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

#include "alcontrol.h"

#include "screen/menu.h"

// This file
#include "keys.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Set a key as KEY_SEEN
static void
clearKey (int keyName, unsigned char * keyArr);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===


// === Global function definitions ===
/// @publicsection

/**
 * @brief Manages the main menu keys. Taking action if a valid one is pressed.
 * 
 * @param menu Game menu
 * @param key Array where the pressed keys are stored
 * 
 * @return Nothing
 */
void
mainMenuKeys (menu_t * menu, unsigned char key[ALLEGRO_KEY_MAX])
{
    /**
     * KEY_UP
     * KEY_DOWN
     * KEY_LEFT
     * KEY_RIGHT
     * KEY_ENTER
     */
    if ( key[ALLEGRO_KEY_UP] == KEY_READY )
    {
        // Clear key from array
        clearKey(ALLEGRO_KEY_UP, key);

        // Move up one position in the menu
        if ( menu -> txt_sel == 2 )
        {
            (menu -> txt_sel)--;
            menu -> redraw = true;
        }

    }

    if ( key[ALLEGRO_KEY_DOWN] == KEY_READY )
    {
        // Clear key from array
        clearKey(ALLEGRO_KEY_DOWN, key);

        // Move down one position in the menu
        if ( menu -> txt_sel == 1 )
        {
            (menu -> txt_sel)++;
            menu -> redraw = true;
        }

    }
    if ( key[ALLEGRO_KEY_LEFT] == KEY_READY )
    {
        // Clear key from array
        clearKey(ALLEGRO_KEY_LEFT, key);

        // Move up left position in the menu
        if ( menu -> txt_sel == 1 )
        {
            (menu -> txt_sel)--;
            menu -> redraw = true;
        }

    }

    if ( key[ALLEGRO_KEY_RIGHT] == KEY_READY )
    {
        // Clear key from array
        clearKey(ALLEGRO_KEY_RIGHT, key);

        // Move right one position in the menu
        if ( menu -> txt_sel == 0 )
        {
            (menu -> txt_sel)++;
            menu -> redraw = true;
        }
    }

    if ( key[ALLEGRO_KEY_ENTER] == KEY_READY )
    {
        // Clear key from array
        clearKey(ALLEGRO_KEY_ENTER, key);

        if ( menu -> action == NONE )
        {
            switch ( menu -> txt_sel )
            {
                case 0:
                    menu -> action = PLAY;
                    break;

                case 1:
                    menu -> action = ABOUT;
                    break;

                case 2:
                    menu -> action = EXIT;
                    break;

                default:
                    break;
            }
        }

        else if ( menu -> action == ABOUT )
        {
            // Return to main menu
            menu -> action = NONE;
        }
    }
}

/// @privatesection
// === Local function definitions ===}

/**
 * @brief Set a key as KEY_SEEN
 * 
 * @param keyName
 * @param keyArr
 * 
 * @return Nothing
 */
static void
clearKey (int keyName, unsigned char * keyArr)
{
    keyArr[keyName] &= KEY_SEEN;
}
