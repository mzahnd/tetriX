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
 * @brief   Audio controller for RPI.
 * 
 * @details It has all the functions and libraries used to play sound on RPI.
 *          It doesn't work, when it comes to play the music the error
 *          that appears is "Couldn't open audio : No audio device available"
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
#include <SDL/SDL.h> //For audio

#include "audiolib/libaudio.h"
#include "audiolib/SDL/Include/SDL.h"


// This file
#include "audioControl.h"

/// @privatesection
// === Constants and Macro definitions ===

#define MUSIC_PATH      "./chau.wav"

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection


void
playMusic(void)
{
    ///Depending on the music status
    switch(player_status())
    {
        ///If it is not initialized, it does so.
        case NO_INIT:
            init_sound();
            break;
        ///If it is ready, it plays the music.
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
    ///Depending on the music status
    switch(player_status())
    {
        ///If it is playing, it stops.
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
    ///Depending on the efects status
    switch(player_status())
    {
        ///If they are not initialized. it does so.
        case NO_INIT:
            init_sound();
            break;
        ///If they are ready, it turns them on.
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
