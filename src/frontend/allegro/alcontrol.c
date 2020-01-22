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
 * @brief   Initialize Allegro and manages the display, inputs and outputs.
 * 
 * @details Everything related to the Allegro frontend of the game starts here
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

#define AUDIO_SAMPLES   8
// === Enumerations, structures and typedefs ===

// Meant to be used along with playSample()

enum samples
{
    MUSIC = 0,
    FX
};

/**
 * @brief Private version of allegro_t structure
 * 
 * Contains the public allegro_t structure, as well as the different audio 
 * samples
 * 
 */
typedef struct
{
    // Public allegro_t
    allegro_t * public;

    // Audio Samples

    struct
    {
        // FX sample, id and status

        struct
        {
            // FX sample
            ALLEGRO_SAMPLE * sample;
            // FX ID
            ALLEGRO_SAMPLE_ID sample_id;

            // FX status
            bool enabled;
        } fx;

        // Music sample, id and status

        struct
        {
            // Music sample
            ALLEGRO_SAMPLE * sample;
            // Music ID
            ALLEGRO_SAMPLE_ID sample_id;

            // Music is enabled
            bool enabled;
            // Music is being played
            bool status;
        } music;
    } samples;

} allegro_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Initialize system, addons and reserve audio samples
static int
allegro_init ();

// Return bool with the music enabling value
static bool
enabledMusic (void);

// Invert bool status of FX
static void
invertFx (void);

// Invert bool status of music
static void
invertMusic (void);

// Play a given FX
static void
playFx (const char * path);

// Play the game music
static void
playMusic (void);

// Play a given sample
static void
playSample (const char *path, const int type);

// Return bool according to the FX enabling status
static bool
statusFx (void);

// Return if music is being played or not
static bool
statusMusic (void);

// Stop game music
static void
stopMusic (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static allegro_private_t alStru;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize Allegro GUI
 * 
 * Initializes everything Allegro needs to run and starts the game.
 * 
 * @param None
 * 
 * @return AL_OK if everything is fine
 * @return AL_ERROR if something failed
 */
int
allegro (void)
{
    // Return value
    int ret = AL_OK;

    // Higher structure with basic Allegro stuff needed to run the game
    static allegro_t pStru;

    // Check if everithing is initialized
    if ( allegro_init() == AL_ERROR )
    {
        ret = AL_ERROR;
    }

    // Set struct pointers
    // Private
    alStru.public = &pStru;

    alStru.samples.fx.sample = NULL;
    alStru.samples.fx.enabled = true;
    alStru.samples.music.sample = NULL;
    alStru.samples.music.enabled = true;
    alStru.samples.music.status = false;

    // Public
    alStru.public -> samples.fx.play = &playFx;
    alStru.public -> samples.fx.invertStatus = &invertFx;
    alStru.public -> samples.fx.enabled = &statusFx;

    alStru.public -> samples.music.play = &playMusic;
    alStru.public -> samples.music.stop = &stopMusic;
    alStru.public -> samples.music.invertStatus = &invertMusic;
    alStru.public -> samples.music.enabled = &enabledMusic;
    alStru.public -> samples.music.status = &statusMusic;

    alStru.public -> exit = false;

    // Create display
    // Enable antialiasing
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 15, ALLEGRO_SUGGEST);
    // Show nicer bitmaps
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    alStru.public -> screen.display = al_create_display(SCREEN_WIDTH,
                                                        SCREEN_HEIGHT);
    if ( alStru.public->screen.display == NULL )
    {
        fputs("Error creating display.", stderr);
        alg_destroy();
        ret = AL_ERROR;
    }

    // Create main menu
    if ( ret != AL_ERROR && alg_menu(alStru.public) == AL_ERROR )
    {
        ret = AL_ERROR;
    }

    // Destroy and exit
    alg_destroy();

    return ret;
}

/**
 * @brief Destroy Allegro GUI
 * 
 * @param None
 * 
 * @return Nothing
 */
void
alg_destroy (void)
{
    alStru.public -> exit = true;
    alStru.public -> samples.fx.play = NULL;
    alStru.public -> samples.music.play = NULL;
    alStru.public -> samples.music.play = NULL;

    alStru.samples.fx.sample = NULL;
    alStru.samples.music.sample = NULL;

    al_destroy_display(alStru.public -> screen.display);

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

/**
 * @brief Initialize system, addons and reserve audio samples
 * 
 * Order: 
 * Allegro
 * Image addon
 * Primitives addon
 * Keyboard
 * Fonts addon
 * TTF addon
 * Audio system
 * 
 * @param None
 * @return AL_OK if everything is fine
 * @return AL_ERROR if something failed
 */
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
    // System
    if ( status == AL_OK && !al_init() )
    {
        fputs("Allegro could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Images
    if ( status == AL_OK && !al_init_image_addon() )
    {
        fputs("Image addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Primitives
    if ( status == AL_OK && !al_init_primitives_addon() )
    {
        fputs("Primitives addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Keyboard
    if ( status == AL_OK && !al_install_keyboard() )
    {
        fputs("Keyboard could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Fonts
    if ( status == AL_OK && !al_init_font_addon() )
    {
        fputs("Fonts addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // TTF Fonts
    if ( status == AL_OK && !al_init_ttf_addon() )
    {
        fputs("TTF fonts addon could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Audio
    if ( status == AL_OK && !al_install_audio() )
    {
        fputs("Audio could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Audio codecs
    if ( status == AL_OK && !al_init_acodec_addon() )
    {
        fputs("Audio codecs could not be initialized.", stderr);
        status = AL_ERROR;
    }

    // Reserve samples
    if ( status == AL_OK && !al_reserve_samples(AUDIO_SAMPLES) )
    {
        fputs("Audio samples could not be reserved.", stderr);
        status = AL_ERROR;
    }

    // Destroy everything if something went wrong
    if ( status == AL_ERROR )
    {
        alg_destroy();
    }

    return status;
}

/**
 * @brief Return bool with the music enabling value
 * 
 * @param None
 * @return true if music is enabled
 * @return false if music is disabled
 */
static bool
enabledMusic (void)
{
    return alStru.samples.music.enabled;
}

/**
 * @brief Invert bool status of FX
 * 
 * If FX are enabled, disable them, otherwise, enable them.
 * 
 * @param None
 * @return Nothing
 */
static void
invertFx (void)
{
    if ( alStru.samples.fx.enabled == true )
    {
        alStru.samples.fx.enabled = false;
    }

    else
    {
        alStru.samples.fx.enabled = true;
    }
}

/**
 * @brief Invert bool status of music
 * 
 * If music is enabled, disable it, otherwise, enable it.
 * 
 * @param None
 * @return Nothing
 */
static void
invertMusic (void)
{
    if ( alStru.samples.music.enabled == true )
    {
        alStru.samples.music.enabled = false;
    }

    else
    {
        alStru.samples.music.enabled = true;
    }
}

/**
 * @brief Play a given FX
 * 
 * @param path Path to FX that should be played
 * 
 * @return Nothing
 */
static void
playFx (const char * path)
{
    playSample(path, FX);
}

/**
 * @brief Play the game music
 * 
 * @param None
 * @return Nothing
 */
static void
playMusic (void)
{
    playSample(NULL, MUSIC);
    // Music is being played
    alStru.samples.music.status = true;
}

/**
 * @brief Play a given sample
 * 
 * Play an FX or the game music, according to @p type. When @p type is MUSIC,
 * any @p path will be ignored (so NULL can be passed in this case).
 * 
 * @param path Path of the FX to play
 * @param type Using enum samples, could be FX or MUSIC
 * @return Nothing
 */
static void
playSample (const char *path, const int type)
{
    switch ( type )
    {
            // Play game music
        case MUSIC:
            if ( alStru.samples.music.enabled == true )
            {
                alStru.samples.music.sample = al_load_sample(MUSIC_PATH);

                if ( !al_play_sample(alStru.samples.music.sample,
                                     0.5, 0.0, 1.0,
                                     ALLEGRO_PLAYMODE_LOOP,
                                     &(alStru.samples.music.sample_id)) )
                {
                    //alStru.samples.music.sample_id = 0;
                    fputs("Error while playing music.", stderr);
                }
            }
            break;

        case FX:

            // Play an FX
            if ( path == NULL )
            {
                fputs("Bad FX path.\n", stderr);
            }

            else if ( alStru.samples.fx.enabled == true )
            {
                alStru.samples.fx.sample = al_load_sample(path);

                if ( !al_play_sample(alStru.samples.fx.sample,
                                     1.5, 0.0, 1.0,
                                     ALLEGRO_PLAYMODE_ONCE,
                                     &(alStru.samples.fx.sample_id)) )
                {
                    // alStru.samples.fx.sample_id = 0;
                    fputs("FX not played.\n", stderr);
                }
            }

            break;

        default:
            break;
    }
}

/**
 * @brief Return bool according to the FX enabling status
 * 
 * @param None
 * 
 * @return true FX are enabled
 * @return false FX are disabled
 */
static bool
statusFx (void)
{
    return alStru.samples.fx.enabled;
}

/**
 * @brief Return if music is being played or not
 * 
 * @return true Music is being played
 * @return false Music is stoped
 */
static bool
statusMusic (void)
{
    return alStru.samples.music.status;
}

/**
 * @brief Stop game music
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
stopMusic (void)
{
    if ( alStru.samples.music.sample != NULL )
    {
        al_stop_sample(&(alStru.samples.music.sample_id));

        //alStru.samples.music.sample_id = NULL;
    }

    alStru.samples.music.status = false;
}
