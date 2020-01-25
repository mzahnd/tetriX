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
 * @file    primitives.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    24/01/2020, 22:26
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

// Allegro
#include <allegro5/allegro5.h>

#include <allegro5/allegro_color.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_primitives.h> 


// This file
#include "primitives.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===


// === Global function definitions ===
/// @publicsection

/**
 * @brief Draws a box using the given information in the structure
 * 
 * The box color.bkgnd property is able to be drawn without background if the
 * BOX_NOBKGND macro is used
 * 
 * @param box Box structure with information to draw it
 * 
 * @return Nothing
 */
void
primitive_drawBox (gbox_t * box)
{
    // If the background is set to anything different than white, draw the
    // background
    if ( strcmp(box -> color.bkgnd, BOX_NOBKGND) )
    {

        al_draw_filled_rounded_rectangle((box -> corner.x), (box -> corner.y),
                                         (box -> width + box -> corner.x),
                                         (box -> height + box -> corner.y),
                                         BOX_ROUND_X, BOX_ROUND_Y,
                                         al_color_html(box -> color.bkgnd));
    }

    al_draw_rounded_rectangle((box -> corner.x), (box -> corner.y),
                              (box -> width + box -> corner.x),
                              (box -> height + box -> corner.y),
                              BOX_ROUND_X, BOX_ROUND_Y,
                              al_color_html(box -> color.border),
                              (box -> thickness));
}

/**
 * @brief Draw a triangle in the screen using a reference box.
 * 
 * @param direction Direction to which the triangle should point. Use enum 
 * triangle_directions.
 * @param tri Triangle to draw.
 * @param box Box around which the triangle will be. NOT the box that contains
 * the triangle but the one that serves as border reference. See note below.
 * 
 * @note 
 * @verbatim           Triangle             Box
 *                    --            --------------
 *                   - -            -            -
 *                  -  -            -            -
 *                 -   -            -            -
 *                  -  -            -            -
 *                   - -            -            -
 *                    --            --------------
 * @endverbatim
 * The triangle takes the boxes coordinates and size to position itself.
 * 
 * @return Nothing
 */
void
primitive_drawTriangle (int direction, triangle_t * tri, gbox_t * box)
{
    // First calculate the triangle coordinates
    if ( direction == TRI_TOP || direction == TRI_BOTTOM )
    {
        tri -> corner[TRI_POINT].x = box -> corner.x + box -> width / 2;
        tri -> corner[TRI_BASE1].x = tri -> corner[TRI_POINT].x - \
                                     tri -> width / 2;
        tri -> corner[TRI_BASE2].x = tri -> corner[TRI_POINT].x + \
                                     tri -> width / 2;

        switch ( direction )
        {
            case TRI_TOP:
                tri -> corner[TRI_POINT].y = box -> corner.y - BOX_OFFSET - \
                                             tri -> height;
                tri -> corner[TRI_BASE1].y = tri -> corner[TRI_BASE2].y = \
                        tri -> corner[TRI_POINT].y + tri -> height;
                break;

            case TRI_BOTTOM:
                tri -> corner[TRI_POINT].y = box -> corner.y + box-> height + \
                                             BOX_OFFSET + tri -> height;
                tri -> corner[TRI_BASE1].y = tri -> corner[TRI_BASE2].y = \
                        tri -> corner[TRI_POINT].y - tri -> height;
                break;

            default:
                break;
        }

    }

    else if ( direction == TRI_LEFT || direction == TRI_RIGHT )
    {
        tri -> corner[TRI_POINT].y = box -> corner.y + box -> height / 2;
        tri -> corner[TRI_BASE1].y = tri -> corner[TRI_POINT].y - \
                                     tri -> width / 2;
        tri -> corner[TRI_BASE1].y = tri -> corner[TRI_POINT].y + \
                                     tri -> width / 2;

        switch ( direction )
        {
            case TRI_LEFT:
                tri -> corner[TRI_POINT].x = box -> corner.x - BOX_OFFSET - \
                                             tri -> height;
                tri -> corner[TRI_BASE1].x = tri -> corner[TRI_BASE2].x = \
                                     tri -> corner[TRI_POINT].x + tri-> height;
                break;

            case TRI_RIGHT:
                tri -> corner[TRI_POINT].x = box -> corner.x + box -> width + \
                                             BOX_OFFSET + tri -> height;
                tri -> corner[TRI_BASE1].x = tri -> corner[TRI_BASE2].x = \
                                     tri -> corner[TRI_POINT].x - tri-> height;
                break;

            default:
                break;
        }
    }


    // Then draw the triangle itself
    al_draw_filled_triangle(tri -> corner[TRI_POINT].x,
                            tri -> corner[TRI_POINT].y,
                            tri -> corner[TRI_BASE1].x,
                            tri -> corner[TRI_BASE1].y,
                            tri -> corner[TRI_BASE2].x,
                            tri -> corner[TRI_BASE2].y,
                            al_color_html(tri -> color.bkgnd));

    al_draw_triangle(tri -> corner[TRI_POINT].x,
                     tri -> corner[TRI_POINT].y,
                     tri -> corner[TRI_BASE1].x,
                     tri -> corner[TRI_BASE1].y,
                     tri -> corner[TRI_BASE2].x,
                     tri -> corner[TRI_BASE2].y,
                     al_color_html(tri -> color.border), tri -> thickness);
}
/// @privatesection
// === Local function definitions ===
