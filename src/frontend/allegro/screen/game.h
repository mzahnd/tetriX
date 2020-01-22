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
#    define GAMESCREEN_H

// === Libraries and header files ===
// For allegro_t
#    include "../alcontrol.h"

// === Constants and Macro definitions ===
// Boxes
/// Space between boxes
#    define BOX_OFFSET          20.0
/// Minimum space between box border and items inside
#    define BOX_IN_OFFSET       5.0
/// Color treated as "transparent" for the boxes background
#    define BOX_NOBKGND         "#FFFFFF"
/// Boxes border bolor
#    define BOX_BORDER_COLOR    "#F0F0F0"
/// Boxes background color
#    define BOX_BKGND_COLOR     BOX_NOBKGND
/// Boxes border thickness
#    define BOX_THICKNESS       3.0


// Box corners
/// Boxes roundness X coordinate
#    define BOX_ROUND_X         2.0
/// Boxes roundness Y coordinate
#    define BOX_ROUND_Y         BOX_ROUND_X

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

/**
 * @brief Box options
 * 
 * Contains box width, height, thickness, font, size, colors and positions
 */
typedef struct
{
    // Box size
    /// Box width
    float width;
    /// Box height
    float height;

    /// Border thickness
    float thickness;

    /// Colors (should be written in hex. e.g. "#FF00AA")

    struct
    {
        /// Border color
        const char * border;
        /// Box background
        const char * bkgnd;
    } color;

    /// Top left corner coordinates

    struct
    {
        /// Top left X coordinate
        float x;
        /// Top left Y coordinate
        float y;
    } corner;

    /// Text fonts, size and color

    struct
    {
        /// Bold text font
        ALLEGRO_FONT * bold;
        /// Bold font size
        unsigned int bsize;

        /// Regular text font
        ALLEGRO_FONT * regular;
        /// Regular font size
        unsigned int rsize;
        
        // Font color
        const char * color;
    } text;

} gbox_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Starts the game while running in Allegro mode
int
alg_game (allegro_t * alStru);

// Draws a box using the given information in the structure
void
drawBox (gbox_t * box);

#endif /* GAMESCREEN_H */
