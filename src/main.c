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
 * @mainpage tetriX documentation
 * 
 * @section Introduction
 * 
 * <i>tetriX</i> is a Tetris game written in C, meant to be runned on any Linux
 * distribution or in a Raspberry Pi with a matrix display + joystick module 
 * attached.
 * 
 * @section sco Source code organization
 * 
 * The project source code is located inside the <i>src</i> folder and it is
 * clearly devided in <i>backend</i> and <i>frontend</i> related sources.\n
 * Also, the whole backend is tested using CUnit test framework and its tests
 * are located in <i>tests</i> folder, which is in the project parent 
 * directory.
 * 
 * @note Altough all tests are properly documented, that documentation is not
 * located here but inside each test.\n
 * This is meant to keep the documentation more clear and show the programmer 
 * only the important stuff.
 * 
 * @subsection Sources
 * 
 * The game rules and options inspiration was taken from \n
 * https://tetris.wiki/Tetris_Guideline \n
 * https://tetris.wiki/Nintendo_Rotation_System \n
 * https://tetris.wiki/Random_Generator \n
 * https://tetris.wiki/Tetris_(NES,_Nintendo) \n
 * https://www.playemulator.com/nes-online/classic-tetris/ \n
 *
 * \n Some ideas / pieces of code inspiration were taken from \n
 * https://stackoverflow.com/questions/351733/how-would-one-write-object-oriented-code-in-c \n
 * https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array \n
 * https://stackoverflow.com/questions/2572366/how-to-use-dev-random-or-urandom-in-c \n
 * http://man7.org/linux/man-pages/man2/getrandom.2.html \n
 * https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace%3A-Input \n
 * https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c/7430262 \n
 * https://stackoverflow.com/questions/8719867/linux-mkdir-function-cant-authorize-full-permission \n
 * 
 * \n Sound effects where taken from \n
 * http://soundbible.com/2067-Blop.html \n
 * https://freesound.org/people/korgchops/sounds/268694/ \n
 * https://freesound.org/people/myfox14/sounds/382310/ \n
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
