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
 * @file    gamePause.h
 * 
 * @brief   Manage the in-game pause
 * 
 * @details Lets the user pause the game while playing, continue it, start a
 * new one and change some options (music, sound fx and showing the next piece)
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    20/01/2020, 19:28
 * 
 * @copyright GNU General Public License v3
 */

#ifndef GAMEPAUSE_H
#    define GAMEPAUSE_H

// === Libraries and header files ===

// For allegro_t
#    include "../alcontrol.h"
// For bool
#    include <stdbool.h>
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

/// Menu being drawn

enum beingDrawn
{
    /// Main pause menu
    MAINMENU = 0,
    /// Options menu
    OPTIONSMENU
};

/// Main menu items

enum pause
{
    /// Resume game
    RESUME = 0,
    /// Restart game
    RESTART,
    /// Options menu
    OPTIONS,
    /// Exit game
    EXIT
};

/// Options menu items

enum options
{
    /// Enable/disable music
    MUSIC = 0,
    /// Enable/disable sound FX
    FX,
    /// Enable/disable next piece
    NP,
    /// Return to previous menu
    OPRET
};

/**
 * @brief Pause box
 * 
 * Contains a gbox_t structure with its size, colors, positions and fonts;
 * another one with the size, colors and position of the box created to select
 * a label and the one one selected.
 * 
 * @warning Function init_pause() must be called before using this structure
 */
typedef struct
{
    /// Selected option in the menu

    struct
    {
        /// Selected option number
        int n;
    } selected;

    /// Read only

    struct
    {
        /// Number of options in the menu
        int nWords;

        /// Next piece should be drawn
        bool nextPiece;
    } get;

    /// Draw the different menus

    struct
    {
        /// The menu that's being drawn or should be. Use it with its enum
        int beingDrawn;

        /// Draw options menu
        void (* options)(void);

        /// Draw the main pause menu
        void (* main)(void);
    } draw;

} pause_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
// Initialize pause menus
void
init_pause (pause_t * pStru, allegro_t * alStru);

#endif /* GAMEPAUSE_H */
