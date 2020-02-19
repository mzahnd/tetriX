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
 * @file    primitives.h
 * 
 * @brief   Generate two kind of primitives: a box and a triangle.
 * 
 * @details Triangles generated here have four orientations (defined in enum
 * triangle_directions).
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    24/01/2020, 22:26
 * 
 * @copyright GNU General Public License v3
 */

#ifndef PRIMITIVES_H
#    define PRIMITIVES_H 1

// === Libraries and header files ===
// For ALLEGRO_FONT
 #include <allegro5/allegro_font.h>

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

// === Enumerations, structures and typedefs ===

/**
 * @brief Coordinates of a triangle and its ammount
 */
enum triangleCoords
{
    /// Top cornerof the triangle
    TRI_POINT = 0,
    /// Base corner
    TRI_BASE1,
    /// Base corner
    TRI_BASE2,
    /// Number of corners
    TRIANGLE_COORDS
};

/**
 * @brief Posible directions in which the triangle can be drawn
 */
enum triangle_directions
{
    /// TRI_POINT points to the top
    TRI_TOP = 0,
    /// TRI_POINT points to the bottom
    TRI_BOTTOM,
    /// TRI_LEFT points to the left
    TRI_LEFT,
    /// TRI_RIGHT points to the right
    TRI_RIGHT,
    /// Number of posible directions
    TRI_NDIRS
};

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

/**
 * @brief Triangle creator structure
 * 
 * Structure used to create a triangle. Must be initialized by the programmer
 * as there are no predefined settings for it.
 */
typedef struct
{
    // Triangle size
    /// Triangle width
    float width;
    /// Triangle height
    float height;

    /// Border thickness
    float thickness;

    /// Colors (should be written in hex. e.g. "#FF00AA")

    struct
    {
        /// Border color
        const char * border;
        /// Background color
        const char * bkgnd;
    } color;

    /// Corner coordinates

    struct
    {
        /// Top left X coordinate
        float x;
        /// Top left Y coordinate
        float y;
    } corner[TRIANGLE_COORDS];

} triangle_t;
// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Draws a box using the given information in the structure
void
primitive_drawBox (gbox_t * box);

// Draw a triangle in the screen using a reference box.
void
primitive_drawTriangle (int direction, triangle_t * tri, gbox_t * box);

#endif /* PRIMITIVES_H */
