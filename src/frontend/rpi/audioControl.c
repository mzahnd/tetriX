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
 * @file    audioControl.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    18/02/2020, 14:26
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h> //For audio
//#include <SDL/SDL.h> //For audio


#include "audiolib/libaudio.h"
//#include "audiolib/SDL/Include/SDL.h"

// This file
#include "audioControl.h"

/// @privatesection
// === Constants and Macro definitions ===

#define MUSIC_PATH      "./res/audio/tetris.wav"

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===


// === Global function definitions ===
/// @publicsection

int
init_rpisound(void)
{
    return init_sound();
}

void
playMusic(void)
{
    switch(player_status())
    {
        case NO_INIT:
            init_rpisound();
            break;

        case READY:
            set_file_to_play(MUSIC_PATH);
            play_sound();
            break;

        default:
            break;
    }
}

void
stopMusic(void)
{
    switch(player_status())
    {
        case PLAYING:
            stop_sound();
            break;

        default:
            break;
    }
}

void
playFX(const char * path)
{
    switch(player_status())
    {
        case NO_INIT:
            init_rpisound();
            break;

        case READY:
            set_file_to_play(path);
            play_sound();

            break;

        default:
            break;
    }
}

/// @privatesection
// === Local function definitions ===
