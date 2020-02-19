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
 */

#ifndef ALCONTROL_H
#    define ALCONTROL_H 1

// === Libraries and header files ===
// For bool, true and false
#    include <stdbool.h>

// For ALLEGRO_DISPLAY
#    include <allegro5/allegro5.h>

// === Constants and Macro definitions ===
// Return values from functions
/// Error return value
#    define AL_ERROR    0
/// OK return value
#    define AL_OK       !AL_ERROR

// Game display size
/// Game display width
#    define SCREEN_WIDTH        800
/// Game display height
#    define SCREEN_HEIGHT       600

// Key detection options
/// A key has been pressed
#    define KEY_SEEN            1
/// The pressed key has been released
#    define KEY_RELEASED        2
/// A key has been pressed and released. Actions can be taked
#    define KEY_READY           ( (KEY_SEEN | KEY_RELEASED) & KEY_RELEASED)

// === Enumerations, structures and typedefs ===

/**
 * @typedef allegro_t
 * @brief Higher game structure.
 * 
 * Contains an exit variable, the game displays and the audio management.
 */
typedef struct ALLEGRO_MGMT allegro_t;

/**
 * @struct ALLEGRO_MGMT
 * @brief Higher game structure.
 * 
 * Contains an exit variable, the game displays and the audio management.
 */
struct ALLEGRO_MGMT
{
    /// true when user asked to exit the game
    bool exit;
    
    /**
     * @brief Audio samples managment
     */
    struct
    {

        /**
         * @brief Music control
         */
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
             * @brief Music playing is enabled or not.
             * @param None
             * @return true: Playing is enabled
             * @return false: Paying is disabled
             */
            bool (* enabled) (void);

            /**
             * @brief Music playing status.
             * @param None
             * @return true: Music is being played
             * @return false: Music is not being played
             */
            bool (* status) (void);

            /**
             * @brief Invert Music playing status.
             * @param None
             * @return Nothing
             */
            void (* invertStatus) (void);
        } music;

        /**
         * @brief FX control
         */
        struct
        {
            /**
             * @brief Play FX in path
             * @param path Path to the desired sound FX
             * @return Nothing
             */
            void (* play) (const char * path);

            /**
             * @brief Play FX in path
             * @param path Path to the desired sound FX
             * @return Nothing
             */
            void (* playSecond) (const char * path);

            /**
             * @brief Music playing is enabled or not.
             * @param None
             * @return true: Playing is enabled
             * @return false: Paying is disabled
             */
            bool (* enabled) (void);

            /**
             * @brief Invert FX playing status.
             * @param None
             * @return Nothing
             */
            void (* invertStatus) (void);
        } fx;

    } samples;

    /** 
     * @brief Game displays
     */
    struct
    {
        /// Main display
        ALLEGRO_DISPLAY * display;
    } screen;

};

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
// Initialize Allegro GUI
int
allegro (void);

// Destroy Allegro GUI
void
alg_destroy (void);

#endif /* ALCONTROL_H */
