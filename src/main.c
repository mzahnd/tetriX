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
 * @file    main.c
 * 
 * @brief Allegro or RPI functions are called from here.
 * 
 * @author Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:17
 * 
 * @copyright GNU General Public License v3
 */

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

#include "backend/board/board.h"
#include "frontend/rpi/display.h"
#include "frontend/rpi/libs/disdrv.h"

/// @privatesection
// === Constants and Macro definitions ===

#ifdef RPI
///This definition allows the use of usleep()
//#    define __USE_MISC
#    include "frontend/rpi/rpcontrol.h"

#endif

#ifdef ALLEGRO
#    include "frontend/allegro/alcontrol.h"
#endif


// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Main
 */
int
main (void)
{
    // Game is being run with a Raspberry PI
#ifdef RPI
    if ( rpi() == ERROR )
    {
        printf("Something went wrong on your RPI\n");
    }
    else
    {
        printf("Thanks for playing\n");
    }
#endif

    // Game is being run with an Allegro GUI
#ifdef ALLEGRO
    if ( !allegro() )
    {
        return (EXIT_FAILURE);
    }
#endif

    return (EXIT_SUCCESS);
}

/// @privatesection
// === Local function definitions ===
