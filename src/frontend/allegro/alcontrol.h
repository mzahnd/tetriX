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
 * @file    alcontrol.h
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
 */

#ifndef ALCONTROL_H
#    define ALCONTROL_H

// === Libraries and header files ===
#    include <stdbool.h>

#    include <allegro5/allegro5.h>
#    include <allegro5/allegro_audio.h>

// === Constants and Macro definitions ===
#    define AL_ERROR    0
#    define AL_OK       !AL_ERROR

#    define SCREEN_WIDTH        800
#    define SCREEN_HEIGHT       600

// Key detection options
#define KEY_SEEN            1
#define KEY_RELEASED        2
#define KEY_READY            ( (KEY_SEEN | KEY_RELEASED) & KEY_RELEASED)

// === Enumerations, structures and typedefs ===
typedef struct ALLEGRO_MGMT allegro_t;

struct ALLEGRO_MGMT
{
    /// True when user asked to exit the game
    bool exit;

    /**
     * @brief Audio samples managment
     */
    struct
    {

        struct
        {
            /**
             * @brief Play music
             * @param None
             * @return Nothing
             */
            void (* play) (void);

            /**
             * @brief Stop music
             * @param None
             * @return Nothing
             */
            void (* stop) (void);

            /**
             * @brief Music playing status.
             * @param None
             * @return True: Playing is enables
             * @return False: Paying is disabled
             */
            bool (* status) (void);

            /**
             * @brief Invert Music playing status.
             * @param None
             * @return Nothing
             */
            void (* invertStatus) (void);
        } music;

        struct
        {
            /**
             * @brief Play FX in path
             * @param path Path to the desired sound FX
             * @return Nothing
             */
            void (* play) (const char * path);

            /**
             * @brief FX playing status.
             * @param None
             * @return True: Playing is enables
             * @return False: Paying is disabled
             */
            bool (* status) (void);

            /**
             * @brief Invert FX playing status.
             * @param None
             * @return Nothing
             */
            void (* invertStatus) (void);
        } fx;

    } samples;

    struct
    {
        ALLEGRO_DISPLAY * display;
    } screen;

};

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
int
allegro (void);

void
alg_destroy (void);

#endif /* ALCONTROL_H */
