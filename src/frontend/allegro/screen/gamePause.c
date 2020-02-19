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
 * @file    gamePause.c
 * 
 * @brief   Manage the in-game pause
 * 
 * @details Lets the user pause the game while playing, continue it, start a
 * new one and change some options (music, sound fx and showing the next piece)
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    20/01/2020, 19:28
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

// For macros
#include "game.h"

// For drawin boxes
#include "../primitives.h"

// This file
#include "gamePause.h"

/// @privatesection
// === Constants and Macro definitions ===
// Pause box
#define PAUSE_BKGND_COLOR   "#1E1E60"
#define PAUSE_BORDER_COLOR  "#C5C5E1"
#define PAUSE_TXT_COLOR     "#BA3020"
#define PAUSE_TXT_SIZE      48
#define PAUSE_BOX_THICKNESS 5.0
#define PAUSE_TXT_FONT_PATH \
                          "res/fonts/pixel-operator/PixelOperatorSC.ttf"

// Pause box - selected option
#define SEL_BKGND_COLOR     "#1E1E60"
#define SEL_BORDER_COLOR    "#C5C5E1"
#define SEL_TXT_COLOR       "#FFFF00"
#define SEL_TXT_SIZE        52
#define SEL_BOX_THICKNESS   5.0
#define SEL_TXT_FONT_PATH   \
                         "res/fonts/pixel-operator/PixelOperatorSC-Bold.ttf"

// Space between text lines and boxes borders
#define TXT_OFFSET          10

// === Enumerations, structures and typedefs ===

/// ON / OFF values

enum onoff
{
    ON = 0,
    OFF,
    NONE
};

/**
 * @brief Private structure of pause_t
 * 
 * Contains the boxes information of the main and options menus, as well as the
 * box to surround the selected option
 */
typedef struct
{
    /// Main pause menu

    struct
    {
        /// Pause menu box
        gbox_t box;

        /// Number of items in this menu
        int nWords;
    } menu;

    /// Selected option

    struct
    {
        /// Box
        gbox_t box;
    } selected;

    /// Options menu

    struct
    {
        /// Box
        gbox_t box;

        /// Number of items in this menu
        int nWords;
    } options;

    /// Public
    pause_t * public;

    /// Game's top structure. Used to manage the audio in the options menu
    allegro_t * alStru;
} pause_private_t;


// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Draw pause main menu in the display
static void
drawMain (void);

// Draw options menu in the display
static void
drawOptions (void);

// Calculate the menu box size
static void
getBoxSize (gbox_t * menuBox, int * nWords, const char * wordsArray[]);

// Get the longest word in an array, as well as how many are in it
static int
getWords (int * nWords, const char * wordsArray[]);

// === ROM Constant variables with file level scope ===

// Words in the main pause menu
static const char * menuWords[] = {
    "Resume", "Restart", "Options", "Exit", NULL
};

// Words in the options menu
static const char * optionsWords[] = {
    "Music:", "FX:", "Next Piece:", "Return", NULL
};

// On / Off words
static const char * onOffWords[] = {
    "ON", "OFF", "", NULL
};

// === Static variables and constant variables with file level scope ===
// Private pause menu
static pause_private_t pMenu;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize pause menus
 * 
 * This function must be called before using any pause_t
 * 
 * @param pStru Public pause structure from where data will be read
 * @param alStru allegro_t structure to perform audio management
 * 
 * @return Nothing
 */
void
init_pause (pause_t * pStru, allegro_t * alStru)
{
    // Allegro
    pMenu.alStru = alStru;

    // Public
    pMenu.public = pStru;

    pMenu.public -> draw.main = &drawMain;
    pMenu.public -> draw.options = &drawOptions;
    pMenu.public -> selected.n = 0;

    pMenu.public -> draw.beingDrawn = MAINMENU;

    pMenu.public -> get.nextPiece = true;

    // Box options
    pMenu.menu.box.color.bkgnd = pMenu.options.box.color.bkgnd = \
                                                             PAUSE_BKGND_COLOR;
    pMenu.menu.box.color.border = pMenu.options.box.color.border = \
                                                            PAUSE_BORDER_COLOR;
    pMenu.menu.box.text.color = pMenu.options.box.text.color = PAUSE_TXT_COLOR;
    pMenu.menu.box.text.rsize = pMenu.options.box.text.rsize = PAUSE_TXT_SIZE;
    pMenu.menu.box.text.regular = pMenu.options.box.text.regular = \
               al_load_font(PAUSE_TXT_FONT_PATH, pMenu.menu.box.text.rsize, 0);

    pMenu.menu.box.thickness = pMenu.options.box.thickness = \
                                                           PAUSE_BOX_THICKNESS;

    pMenu.selected.box.color.bkgnd = SEL_BKGND_COLOR;
    pMenu.selected.box.color.border = SEL_BORDER_COLOR;
    pMenu.selected.box.text.color = SEL_TXT_COLOR;
    pMenu.selected.box.text.rsize = SEL_TXT_SIZE;
    pMenu.selected.box.text.regular = al_load_font(SEL_TXT_FONT_PATH, \
                                           pMenu.selected.box.text.rsize, 0);

    pMenu.selected.box.thickness = SEL_BOX_THICKNESS;

    // Calculate height and width of the menu box
    getBoxSize(&(pMenu.menu.box), &(pMenu.menu.nWords), menuWords);

    // Calculate height and width of the options box
    getBoxSize(&(pMenu.options.box), &(pMenu.options.nWords), optionsWords);
    // As this box has some extra "ON"/"OFF" text, add it to the box width
    pMenu.options.box.width += \
          al_get_text_width(pMenu.selected.box.text.regular, onOffWords[OFF]) \
          + TXT_OFFSET;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Draw pause main menu in the display
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
drawMain (void)
{
    // Counter
    int i;
    // Not selected option font height
    float fontHeight_nSel = \
                          al_get_font_line_height(pMenu.menu.box.text.regular);

    // Draw the box
    primitive_drawBox(&(pMenu.menu.box));

    // Change the public nWords variable
    pMenu.public -> get.nWords = pMenu.menu.nWords;

    // Draw the text
    for ( i = 0; i < pMenu.menu.nWords; i++ )
    {
        // Option is selected
        if ( (pMenu.public -> selected.n) == i )
        {
            al_draw_text(pMenu.selected.box.text.regular,
                         al_color_html(pMenu.selected.box.text.color),
                         pMenu.menu.box.corner.x + pMenu.menu.box.width / 2,
                         pMenu.menu.box.corner.y + (1 + i) * \
                                       GAME_TXT_OFFSET + (i * fontHeight_nSel),
                         ALLEGRO_ALIGN_CENTRE, menuWords[i]);
        }

            // Option is not selected
        else
        {
            al_draw_text(pMenu.menu.box.text.regular,
                         al_color_html(pMenu.menu.box.text.color),
                         pMenu.menu.box.corner.x + pMenu.menu.box.width / 2,
                         pMenu.menu.box.corner.y + (1 + i) * \
                                       GAME_TXT_OFFSET + (i * fontHeight_nSel),
                         ALLEGRO_ALIGN_CENTRE, menuWords[i]);
        }
    }
}

/**
 * @brief Draw options menu in the display
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
drawOptions (void)
{
    // Counter
    int i;
    // Not selected option font height
    float fontHeight_nSel = \
                          al_get_font_line_height(pMenu.options.box.text.regular);

    // Draw the box
    primitive_drawBox(&(pMenu.options.box));

    // Change the public nWords variable
    pMenu.public -> get.nWords = pMenu.options.nWords;

    // Draw options
    for ( i = 0; i < pMenu.options.nWords; i++ )
    {
        // ON / OFF text should be drawn. Uses onoff enum
        int onOff = OFF;

        switch ( i )
        {
                // Music enabled/disabled
            case MUSIC:
                (pMenu.alStru -> samples.music.enabled() == true) ?
                        (onOff = ON) : (onOff = OFF);
                break;

                // Sound fx enabled/disabled
            case FX:
                (pMenu.alStru -> samples.fx.enabled() == true) ?
                        (onOff = ON) : (onOff = OFF);
                break;

                // Next piece is shown or not
            case NP:
                (pMenu.public->get.nextPiece == true) ?
                        (onOff = ON) : (onOff = OFF);
                break;

                // Return to previous menu
            case OPRET:
                onOff = NONE;
                break;
        }

        // Draw text
        // Selected option
        if ( (pMenu.public -> selected.n) == i )
        {
            al_draw_textf(pMenu.selected.box.text.regular,
                          al_color_html(pMenu.selected.box.text.color),
                          pMenu.options.box.corner.x + \
                                                   pMenu.options.box.width / 2,
                          pMenu.options.box.corner.y + (1 + i) * \
                                       GAME_TXT_OFFSET + (i * fontHeight_nSel),
                          ALLEGRO_ALIGN_CENTRE,
                          "%s %s", optionsWords[i], onOffWords[onOff]);
        }

            // Not selected option
        else
        {
            al_draw_textf(pMenu.options.box.text.regular,
                          al_color_html(pMenu.options.box.text.color),
                          pMenu.options.box.corner.x + \
                                                   pMenu.options.box.width / 2,
                          pMenu.options.box.corner.y + (1 + i) * \
                                       GAME_TXT_OFFSET + (i * fontHeight_nSel),
                          ALLEGRO_ALIGN_CENTRE,
                          "%s %s", optionsWords[i], onOffWords[onOff]);
        }
    }
}

/**
 * @brief Calculate the menu box size
 * 
 * @note The array is expected to end in a NULL pointer. For example
 * @code{.c}
 * const char * myArray[] = { "word0", "word1", "word3", NULL };
 * @endcode
 * 
 * @param menuBox Box to calculate its proper size
 * @param nWords Pointer to variable storing the number of words in the array
 * @param wordsArray Array with the words that will appear in the box
 * 
 * @return Nothing
 */
static void
getBoxSize (gbox_t * menuBox, int * nWords, const char * wordsArray[])
{
    // Selected and not selected font height
    float fontHeight_Sel, fontHeight_nSel;

    // Selected and not selected word height (takes the longest word in the 
    // array)
    int sWidth = 0, nWidth = 0;

    // Position of the longest word in the array
    int longestWord;

    // Height of a selected word
    fontHeight_Sel = al_get_font_line_height(pMenu.selected.box.text.regular);

    // Height of a non selected word
    fontHeight_nSel = al_get_font_line_height(menuBox -> text.regular);

    // Get the longest word in menuWords array as well as the number of words
    // in it
    longestWord = getWords(nWords, wordsArray);

    // Box height
    menuBox -> height = (1 + *nWords) * GAME_TXT_OFFSET + fontHeight_Sel + \
                                               (*nWords - 1) * fontHeight_nSel;

    // Take the longest word and get it's width when it's selected
    sWidth = al_get_text_width(pMenu.selected.box.text.regular,
                               wordsArray[longestWord]) + 2 * GAME_TXT_OFFSET;
    // And when it's not
    nWidth = al_get_text_width(menuBox -> text.regular,
                               wordsArray[longestWord]) + 2 * GAME_TXT_OFFSET;

    // Compare them both and stick to the bigger one
    (sWidth > nWidth) ? \
         (menuBox -> width = sWidth) : (menuBox -> width = nWidth);

    // Calculate the top left (x,y) coordinates
    menuBox -> corner.x = (SCREEN_WIDTH / 2) - (menuBox -> width / 2);
    menuBox -> corner.y = (SCREEN_HEIGHT / 2) - (menuBox -> height / 2);
}

/**
 * @brief Get the longest word in an array, as well as how many are in it
 * 
 * In case that two or more words are equally large, the first one is returned
 * 
 * @note The array is expected to end in a NULL pointer. For example
 * @code{.c}
 * const char * myArray[] = { "word0", "word1", "word3", NULL };
 * @endcode
 * 
 * @param nWords Where the number of words should be stored
 * @param wordsArray Array to read
 * @return Array position of the bigger word
 */
static int
getWords (int * nWords, const char * wordsArray[])
{
    int longestWord = 0, bWordSize = 0;
    int tmp = 0;

    // Clear number of words
    *nWords = 0;
    // Read the array
    while ( wordsArray[*nWords] != NULL )
    {
        // Get word size
        tmp = strlen(wordsArray[*nWords]);

        // A bigger word is found (compared to the previously founded)
        if ( bWordSize < tmp )
        {
            // Set it as the bigger one
            bWordSize = tmp;
            // Position of this word in the array
            longestWord = *nWords;
        }

        // Increase number of words
        (*nWords)++;
    }

    // Return the longest word position
    return longestWord;
}
