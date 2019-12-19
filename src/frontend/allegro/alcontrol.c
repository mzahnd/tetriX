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

/******************************************************************************
 * 
 * @file    alcontrol.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    17/12/2019, 11:27
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


// Allegro
#include <allegro5/allegro5.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <allegro5/allegro_color.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 

// This file
#include "alcontrol.h"

#include "screen/menu.h"

/// @privatesection
// === Constants and Macro definitions ===
#define MUSIC_PATH      "res/audio/tetris.wav"
// === Enumerations, structures and typedefs ===

enum samples
{
    MUSIC = 0,
    FX
};

typedef struct
{
    allegro_t * public;

    struct
    {

        struct
        {
            ALLEGRO_SAMPLE * sample;
            ALLEGRO_SAMPLE_ID * sample_id;

            bool status;
        } fx;

        struct
        {
            ALLEGRO_SAMPLE * sample;
            ALLEGRO_SAMPLE_ID * sample_id;

            bool status;
        } music;
    } samples;

} allegro_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
static int
allegro_init ();

static void
invertFx (void);

static void
invertMusic (void);

static void
playFx (const char * path);

static void
playMusic (void);

static void
playSample (const char *path, const int type);

static bool
statusFx (void);

static bool
statusMusic (void);

static void
stopMusic (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static allegro_private_t alStru;

// === Global function definitions ===
/// @publicsection

int
allegro (void)
{
    int ret = 0;

    static allegro_t pStru;

    // Check if everithing is initialized
    if ( allegro_init() == AL_ERROR )
    {
        ret = 1;
    }

    // Set struct pointers
    // Private stuff
    alStru.public = &pStru;

    alStru.samples.fx.sample = NULL;
    alStru.samples.fx.sample_id = NULL;
    alStru.samples.fx.status = true;
    alStru.samples.music.sample = NULL;
    alStru.samples.music.sample_id = NULL;
    alStru.samples.music.status = true;

    // Public stuff
    //alStru.public -> screen.game.create = &alg_game;
    alStru.public -> screen.menu.create = &alg_menu;

    alStru.public -> samples.fx.play = &playFx;
    alStru.public -> samples.fx.invertStatus = &invertFx;
    alStru.public -> samples.fx.status = &statusFx;

    alStru.public -> samples.music.play = &playMusic;
    alStru.public -> samples.music.stop = &stopMusic;
    alStru.public -> samples.music.invertStatus = &invertMusic;
    alStru.public -> samples.music.status = &statusMusic;

    alStru.public -> exit = false;

    // Create main menu
    if ( ret != 1 && alg_menu(alStru.public) == AL_ERROR )
    {
        ret = 1;
    }

    // Destroy and exit
    alg_destroy();

    return ret;
}

void
alg_destroy (void)
{
    alStru.public -> exit = true;
    alStru.public -> samples.fx.play = NULL;
    alStru.public -> samples.music.play = NULL;
    alStru.public -> samples.music.play = NULL;
    alStru.public -> screen.menu.create = NULL;
    //alStru.public -> screen.game.create = NULL;

    alStru.samples.fx.sample = NULL;
    alStru.samples.fx.sample_id = NULL;
    alStru.samples.music.sample = NULL;
    alStru.samples.music.sample_id = NULL;

    al_destroy_sample(alStru.samples.fx.sample);
    al_destroy_sample(alStru.samples.music.sample);
    al_uninstall_audio();
    al_shutdown_font_addon();
    al_uninstall_keyboard();
    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
}

/// @privatesection
// === Local function definitions ===

static int
allegro_init ()
{
    char status = AL_OK;

    /*
     * Allegro
     * Image addon
     * Primitives addon
     * Keyboard
     * Fonts addon
     * TTF addon
     * Audio system
     */
    if ( status == AL_OK && !al_init() )
    {
        fputs("Allegro could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_init_image_addon() )
    {
        fputs("Image addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_init_primitives_addon() )
    {
        fputs("Primitives addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_install_keyboard() )
    {
        fputs("Keyboard could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_init_font_addon() )
    {
        fputs("Fonts addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_init_ttf_addon() )
    {
        fputs("TTF fonts addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_install_audio() )
    {
        fputs("Audio could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_init_acodec_addon() )
    {
        fputs("Audio codecs could not be initialized.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_OK && !al_reserve_samples(2) )
    {
        fputs("Audio samples could not be reserved.", stderr);
        status = AL_ERROR;
    }

    if ( status == AL_ERROR )
    {
        alg_destroy();
    }

    return status;
}

static void
invertFx (void)
{
    if ( alStru.samples.fx.status == true )
    {
        alStru.samples.fx.status = false;
    }

    else
    {
        alStru.samples.fx.status = true;
    }
}

static void
invertMusic (void)
{
    if ( alStru.samples.music.status == true )
    {
        alStru.samples.music.status = false;
    }

    else
    {
        alStru.samples.music.status = true;
    }
}

static void
playSample (const char *path, const int type)
{
    switch ( type )
    {
        case MUSIC:
            if ( alStru.samples.music.sample_id == NULL &&
                 alStru.samples.music.status == true )
            {
                alStru.samples.music.sample = al_load_sample(MUSIC_PATH);

                if ( !al_play_sample(alStru.samples.music.sample,
                                     1.0, 0.0, 1.0,
                                     ALLEGRO_PLAYMODE_LOOP,
                                     alStru.samples.music.sample_id) )
                {
                    alStru.samples.music.sample_id = NULL;
                    fputs("Error while playing music.", stderr);
                }
            }
            break;

        case FX:
            if ( alStru.samples.fx.sample_id == NULL &&
                 alStru.samples.fx.status == true )
            {
                alStru.samples.music.sample = al_load_sample(path);

                if ( !al_play_sample(alStru.samples.fx.sample,
                                     1.0, 0.0, 1.0,
                                     ALLEGRO_PLAYMODE_LOOP,
                                     alStru.samples.fx.sample_id) )
                {
                    alStru.samples.fx.sample_id = NULL;
                    fputs("Error while playing music.", stderr);
                }
            }
            break;

        default:
            break;
    }
}

static void
playFx (const char * path)
{
    playSample(path, FX);
}

static void
playMusic (void)
{
    playSample("", MUSIC);
}

static bool
statusFx (void)
{
    return alStru.samples.fx.status;
}

static bool
statusMusic (void)
{
    return alStru.samples.music.status;
}

static void
stopMusic (void)
{
    if ( alStru.samples.music.sample != NULL )
    {
        al_stop_sample(alStru.samples.music.sample_id);

        alStru.samples.music.sample_id = NULL;
    }
}
