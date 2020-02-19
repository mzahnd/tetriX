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
 * @file    endGame.c
 * 
 * @brief   Draw a screen after the game has ended.
 * 
 * @details Displays the top ten score table or an "arcade character selector"
 * for entering a name whenever a new top score has been made.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    24/01/2020, 12:22
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Allegro
#include <allegro5/allegro5.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <allegro5/allegro_color.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 

#include "../alcontrol.h"

// Stats management
#include "../../../backend/stats/stats_mgmt.h"

// Top score management
#include "../../../backend/rw/rw_ops.h"

// For constants
#include "game.h"

// For triangle and box primitives
#include "../primitives.h"

// This file
#include "endGame.h"

/// @privatesection
// === Constants and Macro definitions ===
#define FPS                     60.0

#define ENDGAME_BKGND_COLOR     "#000000"
#define BKGND_WIDTH             1920
#define BKGND_HEIGHT            1280
#define ENDGAME_BKGND           "res/images/game/endgame.png"

#define NEWTOP_TXT_COLOR        "#FFFF00"

#define ENDGAME_BOX_OFFSET      (BOX_OFFSET / 2)

#define BTN_ROUNDNESS           25.0
#define BTN_TXT_COLOR           "#574642"
#define BTN_SEL_THICKNESS       2.5
#define BTN_SEL_TXT_COLOR       "#E6F1F9"
#define BTN_SEL_COLOR_BDR       "#000000"
#define BTN_SEL_COLOR_BKGND     "#65E724"
#define BTN_TXT_OFFSET          GAME_TXT_OFFSET
#define BTN_TXT_SIZE            40
#define BTN_TXT_FONT_PATH       \
                          "res/fonts/liberation_serif/LiberationSerif-Bold.ttf"

#define TRI_THICKNESS           5.0
#define TRI_COLOR_BDR           "#000000"
#define TRI_COLOR_BKGND         "#FFFFFF"
#define TRI_HEIGHT              50.0
#define TRI_WIDTH               (TRI_HEIGHT * 0.9)

// === Enumerations, structures and typedefs ===

enum eg_coords
{
    EG_COORD_X = 0,
    EG_COORD_Y,
    EG_COORD_N
};

enum subScreen
{
    INSNAME = 0,
    DISPLAYTOP
};

enum displayTopTxt
{
    DT_TITLE = 0,
    DT_BUTTON0,
    DT_BUTTON1,
    DT_NTXT
};

enum insNameTxt
{
    IN_TITLE = 0,
    IN_NTXT
};

typedef struct
{
    // Allegro structure with sound control and the original display
    allegro_t * alStru;

    // Game stats
    rwScores_t * topScoreFile;

    // Display
    ALLEGRO_DISPLAY * display;
    // Background
    ALLEGRO_BITMAP * bkgnd;

    // Event queue
    ALLEGRO_EVENT_QUEUE * evq;

    // Timers

    struct
    {
        // Display refresh and general timer
        ALLEGRO_TIMER * main;
    } timer;

    struct
    {
        int n;

        struct
        {
            /// Player name inserted
            char playerName[NAMESIZE];

            /// Pointer to the new score
            const int * newScore;

            /// Characters to create the word

            struct
            {
                /// Box
                gbox_t box;
                /// Number of characters to display in each character selection
                int n;
                /// Selected character in the GUI
                int selGUI;

                /// Position in the array of charactes of the selected one
                int selChar[NAMESIZE - 1];

            } characters;

            /// Subscreen title
            gbox_t title;

            struct
            {
                /// Top triangle
                /**
                 *     *     \n
                 *    * *    \n
                 *   *   *   \n
                 *  *     *  \n
                 * ********* \n
                 */
                triangle_t top;

                /// Bottom triangle
                /**
                 * ********* \n
                 *  *     *  \n
                 *   *   *   \n
                 *    * *    \n
                 *     *     \n
                 */
                triangle_t bottom;
            } triangle;
        } insName;

        struct
        {
            gbox_t title;
            gbox_t topList;

            struct
            {
                gbox_t exit;
                gbox_t playAgain;
            } button;

            int selected;
        } displayTop;

    } subScreens;

    bool newTop;

    bool redraw;

    bool exit;

} endGame_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Initialize common properties of every box
static void
box_initCommon (gbox_t * box);

// Set common box colors
static void
box_loadColors (gbox_t * box);

// Set common box fonts
static void
box_loadFonts (gbox_t * box);

// Verify if a pressed key is valid and perform its corresponding actions
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX], endGame_t * stru, int screen);

// Set a key as KEY_SEEN
static void
clearKey (int keyName, unsigned char * keyArr);

// Draw DISPLAYTOP screen
static void
draw_displayTop (endGame_t * stru);

// Draw INSNAME screen
static void
draw_insName (endGame_t * stru);

// Initialize a button
static void
init_Btn (gbox_t * button, const char * text);

// Initialize DISPLAYTOP screen
static void
init_displayTop (endGame_t * stru);

// Initialize INSNAME screen
static void
init_insName (endGame_t * stru);

// Load screen background
static void
load_bkgnd (endGame_t * stru);

// Manage display events for both, "arcade input" and top score printing
static void
manageEvents (endGame_t * stru, int screen);

// Initialize common settings for triangles
static void
triangle_initCommon (triangle_t * tri);

// Set key as valid and then clear it
static void
validKey (int keyName,
          unsigned char * keyArr, unsigned char * counterArr,
          endGame_t * stru);



// === ROM Constant variables with file level scope ===
static const char * txtDisplayTop[DT_NTXT] = {
    "GAME OVER!", "Play again", "Menu"
};

static const char * txtInsName[IN_NTXT] = {
    "Enter your name"
};

static const char * validCharacters[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3",
    "4", "5", "6", "7", "8", "9", ".", ",", ":", ";", "-", "_", "<", ">", "+",
    "*", "/", "?", "!", "#", "$", "%", "&", "(", ")", "|", "[", "]", "{", "}",
    " ", NULL
};
// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Draw End Game screen with the top scores list in it.
 * 
 * Also allows inserting a new top score to it if needed.
 * 
 * @param alStru Structure with an already initialized display and system
 * 
 * @param gameStats Stats used during the game.
 * 
 * @return AL_OK if everything is fine
 * @return AL_ERROR if something failed
 */
int
alg_endGame (allegro_t * alStru, const stats_t * gameStats)
{
    // Structure with information to draw in the display
    endGame_t endGameSt;

    // Verify if everything is in order
    if ( alStru == NULL || gameStats == NULL )
    {
        fputs("NULL Structure passed.\n", stderr);
        return AL_ERROR;
    }

    if ( al_is_keyboard_installed() == false )
    {
        return AL_ERROR;
    }

    if ( gameStats->_tsLoaded == false )
    {
        fputs("Top Scores file was not initialized. Exiting.\n", stderr);
        endGameSt.exit = true;
        alStru -> exit = true;
        return AL_ERROR;
    }
    else
    {
        endGameSt.exit = false;
    }

    // Copy alStru to the structure
    endGameSt.alStru = alStru;
    // Copy display pointer
    endGameSt.display = alStru -> screen.display;

    // Copy rwScores_t pointer to endGameSt
    endGameSt.topScoreFile = gameStats -> topScores;

    // Initialize the rest of the structure
    endGameSt.bkgnd = NULL;
    endGameSt.timer.main = NULL;
    endGameSt.evq = NULL;

    endGameSt.redraw = true;

    endGameSt.subScreens.displayTop.selected = DT_BUTTON0;

    strncpy(endGameSt.subScreens.insName.playerName, "AAA", NAMESIZE - 1);
    // Null character manually added
    endGameSt.subScreens.insName.playerName[NAMESIZE - 1 ] = '\0';

    // Lowest score in the top list is lower than the new one
    if ( endGameSt.topScoreFile->get._scores[NTOPSCORE - 1] <
         gameStats -> score.actual )
    {
        endGameSt.subScreens.n = INSNAME;

        // Copy the new score to the rwScores_t structure inside endGameSt
        endGameSt.subScreens.insName.newScore = &(gameStats -> score.actual);

        // This is a new top score!
        endGameSt.newTop = true;

        // Initialize both subscreens
        init_displayTop(&endGameSt);
        init_insName(&endGameSt);
    }

        // Lowest score in the top list is higher than the new one
    else
    {
        endGameSt.subScreens.n = DISPLAYTOP;

        // Initialize only this subscreen
        init_displayTop(&endGameSt);

        // No new top score
        endGameSt.newTop = false;
    }

    // == Timers ==
    // Main timer
    endGameSt.timer.main = al_create_timer(1.0 / FPS);

    if ( !endGameSt.timer.main )
    {
        fputs("Failed to create timer.", stderr);
        //destroy(&endGameSt);
        alg_destroy();
        return AL_ERROR;
    }

    // == Event Queue ==
    endGameSt.evq = al_create_event_queue();

    if ( !endGameSt.evq )
    {
        fputs("Failed to create event queue.", stderr);
        //destroy(&endGameSt);
        alg_destroy();
        return AL_ERROR;
    }

    // Register events
    // Display
    al_register_event_source(endGameSt.evq,
                             al_get_display_event_source(endGameSt.display));
    // Keyboard
    al_register_event_source(endGameSt.evq, al_get_keyboard_event_source());
    // Main timer
    al_register_event_source(endGameSt.evq,
                             al_get_timer_event_source(endGameSt.timer.main));

    // == Background ==
    // Load screen background
    load_bkgnd(&endGameSt);

    // Start events timers
    al_start_timer(endGameSt.timer.main);

    while ( endGameSt.exit == false )
    {
        if ( endGameSt.subScreens.n == INSNAME )
        {
            (endGameSt.redraw == true) ?
                    (draw_insName(&endGameSt)) :
                    (manageEvents(&endGameSt, INSNAME));
        }

        else if ( endGameSt.subScreens.n == DISPLAYTOP )
        {
            (endGameSt.redraw == true) ?
                    (draw_displayTop(&endGameSt)) :
                    (manageEvents(&endGameSt, DISPLAYTOP));
        }
    }
    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Initialize common properties of every box
 * 
 * @param box Box to initialize
 * 
 * @return Nothing
 */
static void
box_initCommon (gbox_t * box)
{
    box_loadFonts(box);
    box_loadColors(box);
    box -> thickness = BOX_THICKNESS;
}

/**
 * @brief Set common box colors
 * 
 * @param box Box to initialize
 * 
 * @return Nothing
 */
static void
box_loadColors (gbox_t * box)
{
    box -> color.border = BOX_BORDER_COLOR;
    box -> color.bkgnd = BOX_BKGND_COLOR;
}

/**
 * @brief Set common box fonts
 * 
 * @param box Box to initialize
 * 
 * @return Nothing
 */
static void
box_loadFonts (gbox_t * box)
{
    // Bold
    box -> text.bsize = GAME_TXT_SIZE_BOLD;
    box -> text.bold = al_load_font(GAME_TXT_FONT_BOLD_PATH, \
                                    box -> text.bsize, 0);

    // Regular
    box -> text.rsize = GAME_TXT_SIZE;
    box -> text.regular = al_load_font(GAME_TXT_FONT_PATH, \
                                       box -> text.rsize, 0);

    // Color
    box -> text.color = GAME_TXT_COLOR;
}

/**
 * @brief Verify if a pressed key is valid and perform its corresponding action
 * 
 * @param key Key array with keys that have been pressed.
 * @param stru Current endGame_t structure.
 * @param screen Screen that's being printed.
 * 
 * @return Nothing
 */
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX], endGame_t * stru, int screen)
{
    // Counter
    int i;

    // Counter to make keys less sensitive
    static unsigned char counter[ALLEGRO_KEY_MAX] = {};


    // Update counter of a given key when necessary
    for ( i = 0; i < ALLEGRO_KEY_MAX; i++ )
    {
        if ( counter[i] == 5 )
        {
            counter[i] = 0;
        }

        else if ( counter[i] > 0 )
        {
            counter[i]++;
        }
    }

    switch ( screen )
    {
            // Insert name Screen
        case INSNAME:
            if ( key[ALLEGRO_KEY_UP] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_UP, key, counter, stru);

                // Change letter
                if ( stru -> subScreens.insName.characters.selChar[ \
                    stru -> subScreens.insName.characters.selGUI] <
                     stru -> subScreens.insName.characters.n )
                {
                    stru -> subScreens.insName.characters.selChar[ \
                            stru -> subScreens.insName.characters.selGUI]++;
                }
                else
                {
                    stru -> subScreens.insName.characters.selChar[ \
                            stru -> subScreens.insName.characters.selGUI] = 0;
                }
            }

            if ( key[ALLEGRO_KEY_DOWN] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_DOWN, key, counter, stru);

                // Change letter
                if ( stru -> subScreens.insName.characters.selChar[ \
                    stru -> subScreens.insName.characters.selGUI] > 0 )
                {
                    stru -> subScreens.insName.characters.selChar[ \
                            stru -> subScreens.insName.characters.selGUI]--;
                }
                else
                {
                    stru -> subScreens.insName.characters.selChar[ \
                            stru -> subScreens.insName.characters.selGUI] = \
                                       stru -> subScreens.insName.characters.n;
                }

            }

            if ( key[ALLEGRO_KEY_LEFT] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_LEFT, key, counter, stru);

                // Change button
                (stru -> subScreens.insName.characters.selGUI > 0) ?
                        (stru -> subScreens.insName.characters.selGUI--) : \
               (stru -> subScreens.insName.characters.selGUI = NAMESIZE - 2);

            }

            if ( key[ALLEGRO_KEY_RIGHT] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_RIGHT, key, counter, stru);

                // Change button
                (stru -> subScreens.insName.characters.selGUI < \
                 NAMESIZE - 2) ?
                        (stru -> subScreens.insName.characters.selGUI++) :
                        (stru -> subScreens.insName.characters.selGUI = 0);

            }

            if ( key[ALLEGRO_KEY_ENTER] == KEY_READY )
            {
                // Counter
                int i;

                // Clear key from array
                clearKey(ALLEGRO_KEY_ENTER, key);

                // Copy characters to playerName array
                for ( i = 0; i < NAMESIZE - 1; i++ )
                {
                    strncpy(&(stru ->subScreens.insName.playerName[i]),
                            validCharacters[ \
                            stru -> subScreens.insName.characters.selChar[i]],
                            1);
                }

                // Add name and score to the top list
                strncpy(stru -> topScoreFile -> add.name,
                        stru -> subScreens.insName.playerName, NAMESIZE);

                stru -> topScoreFile -> add.score = \
                                        *(stru -> subScreens.insName.newScore);

                stru -> topScoreFile -> add.writeIntoScore( \
                                                         stru -> topScoreFile);

                stru -> subScreens.n = DISPLAYTOP;
                stru -> redraw = true;
            }
            break;

            // Display TOP Scores list
        case DISPLAYTOP:
            if ( key[ALLEGRO_KEY_LEFT] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_LEFT, key, counter, stru);

                // Change button
                (stru -> subScreens.displayTop.selected == DT_BUTTON1) ?
                        (stru -> subScreens.displayTop.selected)-- :
                        (stru -> subScreens.displayTop.selected)++;
            }

            if ( key[ALLEGRO_KEY_RIGHT] == KEY_READY )
            {
                // Clear key from array
                validKey(ALLEGRO_KEY_RIGHT, key, counter, stru);

                // Change button
                (stru -> subScreens.displayTop.selected == DT_BUTTON1) ?
                        (stru -> subScreens.displayTop.selected)-- :
                        (stru -> subScreens.displayTop.selected)++;

            }

            if ( key[ALLEGRO_KEY_ENTER] == KEY_READY )
            {
                // Clear key from array
                clearKey(ALLEGRO_KEY_ENTER, key);

                // Change button
                switch ( stru -> subScreens.displayTop.selected )
                {
                    case DT_BUTTON0:
                        stru -> alStru -> exit = false;
                        stru -> exit = true;
                        break;

                    case DT_BUTTON1:
                        stru -> alStru -> exit = true;
                        stru -> exit = true;
                        break;
                }
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Set a key as KEY_SEEN
 * 
 * @param keyName Key to clear
 * @param keyArr Array with all the keys
 * 
 * @return Nothing
 */
static void
clearKey (int keyName, unsigned char * keyArr)
{
    // And key with KEY_SEEN
    keyArr[keyName] &= KEY_SEEN;
}

/**
 * @brief Draw DISPLAYTOP screen
 * 
 * @param stru End Game screen structure
 * 
 * @return Nothing
 */
static void
draw_displayTop (endGame_t * stru)
{
    // Counter
    int i;

    // Text position
    // Player name
    float nameTXT[EG_COORD_N] = {
        stru -> subScreens.displayTop.topList.corner.x + BOX_IN_OFFSET,
        stru -> subScreens.displayTop.topList.corner.y + BOX_IN_OFFSET
    };

    // Score
    /*al_get_text_width(stru -> subScreens.displayTop.topList.text.bold, \
                         stru -> topScoreFile->get._scores[0]),*/
    float scoreTXT[EG_COORD_N] = {
        stru -> subScreens.displayTop.topList.corner.x + \
        stru -> subScreens.displayTop.topList.width - BOX_IN_OFFSET,
        nameTXT[EG_COORD_Y]
    };

    // Background
    al_clear_to_color(al_color_html(ENDGAME_BKGND_COLOR));

    if ( stru -> bkgnd != NULL )
    {
        al_draw_scaled_bitmap(stru -> bkgnd,
                              0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              0);
    }

    // Draw title
    al_draw_textf(stru -> subScreens.displayTop.title.text.bold,
                  al_color_html( \
                               stru -> subScreens.displayTop.title.text.color),
                  SCREEN_WIDTH / 2,
                  stru -> subScreens.displayTop.title.corner.y,
                  ALLEGRO_ALIGN_CENTRE,
                  "%s", txtDisplayTop[DT_TITLE]);
    // Draw Top Scores list
    primitive_drawBox(&(stru -> subScreens.displayTop.topList));

    // Draw list text
    for ( i = 0; i < NTOPSCORE; i++ )
    {
        if ( stru -> newTop == true &&
             !strcmp(stru -> subScreens.insName.playerName, \
             stru -> topScoreFile -> get._names[i]) &&
             *(stru -> subScreens.insName.newScore) ==
             stru -> topScoreFile -> get._scores[i] )
        {
            al_draw_textf(stru -> subScreens.displayTop.topList.text.bold,
                          al_color_html(NEWTOP_TXT_COLOR),
                          nameTXT[EG_COORD_X], nameTXT[EG_COORD_Y],
                          ALLEGRO_ALIGN_LEFT,
                          "%s", stru -> topScoreFile->get._names[i]);

            al_draw_textf(stru -> subScreens.displayTop.topList.text.bold,
                          al_color_html(NEWTOP_TXT_COLOR),
                          scoreTXT[EG_COORD_X], scoreTXT[EG_COORD_Y],
                          ALLEGRO_ALIGN_RIGHT,
                          "%u", stru -> topScoreFile->get._scores[i]);
        }

        else
        {
            al_draw_textf(stru -> subScreens.displayTop.topList.text.regular,
                          al_color_html( \
                             stru -> subScreens.displayTop.topList.text.color),
                          nameTXT[EG_COORD_X], nameTXT[EG_COORD_Y],
                          ALLEGRO_ALIGN_LEFT,
                          "%s", stru -> topScoreFile->get._names[i]);

            al_draw_textf(stru -> subScreens.displayTop.topList.text.regular,
                          al_color_html( \
                             stru -> subScreens.displayTop.topList.text.color),
                          scoreTXT[EG_COORD_X], scoreTXT[EG_COORD_Y],
                          ALLEGRO_ALIGN_RIGHT,
                          "%u", stru -> topScoreFile->get._scores[i]);
        }

        // Increase Y coordinate for the next score in the list
        nameTXT[EG_COORD_Y] += stru -> subScreens.displayTop.topList.text.bsize \
                            + GAME_TXT_OFFSET;
        scoreTXT[EG_COORD_Y] = nameTXT[EG_COORD_Y];
    }


    // Draw buttons
    // Change text colors properly
    switch ( stru -> subScreens.displayTop.selected )
    {
        case DT_BUTTON0:
            stru -> subScreens.displayTop.button.playAgain.text.color = \
                                                             BTN_SEL_TXT_COLOR;
            stru -> subScreens.displayTop.button.exit.text.color = \
                                                                 BTN_TXT_COLOR;
            break;

        case DT_BUTTON1:
            stru -> subScreens.displayTop.button.playAgain.text.color = \
                                                                 BTN_TXT_COLOR;
            stru -> subScreens.displayTop.button.exit.text.color = \
                                                             BTN_SEL_TXT_COLOR;
            break;

        default:
            break;
    }
    // Left button
    primitive_drawBox(&(stru -> subScreens.displayTop.button.playAgain));
    // Right button
    primitive_drawBox(&(stru -> subScreens.displayTop.button.exit));

    // Draw buttons text
    if ( stru -> subScreens.displayTop.selected == DT_BUTTON0 )
    {
        al_draw_textf( \
                    stru -> subScreens.displayTop.button.playAgain.text.bold, \
                      al_color_html(BTN_SEL_TXT_COLOR),
                      stru -> subScreens.displayTop.button.playAgain.corner.x \
                      + BTN_TXT_OFFSET,
                      stru -> subScreens.displayTop.button.playAgain.corner.y \
                      + BTN_TXT_OFFSET,
                      ALLEGRO_ALIGN_LEFT,
                      "%s", txtDisplayTop[DT_BUTTON0]);

        al_draw_textf(stru -> subScreens.displayTop.button.exit.text.regular,
                      al_color_html( \
                         stru -> subScreens.displayTop.button.exit.text.color),
                      stru -> subScreens.displayTop.button.exit.corner.x \
                      + BTN_TXT_OFFSET,
                      stru -> subScreens.displayTop.button.exit.corner.y \
                      + BTN_TXT_OFFSET,
                      ALLEGRO_ALIGN_LEFT,
                      "%s", txtDisplayTop[DT_BUTTON1]);
    }
    else
    {
        al_draw_textf( \
                 stru -> subScreens.displayTop.button.playAgain.text.regular, \
                     al_color_html( \
                    stru -> subScreens.displayTop.button.playAgain.text.color),
                      stru -> subScreens.displayTop.button.playAgain.corner.x \
                      + BTN_TXT_OFFSET,
                      stru -> subScreens.displayTop.button.playAgain.corner.y \
                      + BTN_TXT_OFFSET,
                      ALLEGRO_ALIGN_LEFT,
                      "%s", txtDisplayTop[DT_BUTTON0]);

        al_draw_textf(stru -> subScreens.displayTop.button.exit.text.bold,
                      al_color_html(BTN_SEL_TXT_COLOR),
                      stru -> subScreens.displayTop.button.exit.corner.x \
                      + BTN_TXT_OFFSET,
                      stru -> subScreens.displayTop.button.exit.corner.y \
                      + BTN_TXT_OFFSET,
                      ALLEGRO_ALIGN_LEFT,
                      "%s", txtDisplayTop[DT_BUTTON1]);
    }

    stru -> redraw = false;

    al_flip_display();
}

/**
 * @brief Draw INSNAME screen
 * 
 * This will be printed by this function (insted of A, any character from 
 * validCharacters[] can also be printed):
 * 
 *          /\             /\           /\
 *         /  \           /  \         /  \
 *        /    \         /    \       /    \
 *        ------         ------       ------
 *  
 *     ------------- ------------- -------------
 *     |           | |           | |           |
 *     |     *     | |     *     | |     *     |
 *     |    * *    | |    * *    | |    * *    |
 *     |   *   *   | |   *   *   | |   *   *   |
 *     |  * *** *  | |  * *** *  | |  * *** *  |
 *     | *       * | | *       * | | *       * |
 *     |           | |           | |           |
 *     ------------- ------------- -------------
 * 
 *        ------         ------       ------
 *        \    /         \    /       \    /
 *         \  /           \  /         \  /
 *          \/             \/           \/
 * 
 * @param stru End Game screen structure
 * 
 * @return Nothing
 */
static void
draw_insName (endGame_t * stru)
{
    // Counter
    int i;

    // Original X coordinate of the middle box of characters
    float charBoxCoordinateX = \
                            stru -> subScreens.insName.characters.box.corner.x;

    // Background
    al_clear_to_color(al_color_html(ENDGAME_BKGND_COLOR));

    if ( stru -> bkgnd != NULL )
    {
        al_draw_scaled_bitmap(stru -> bkgnd,
                              0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              0);
    }

    // Draw title
    al_draw_textf(stru -> subScreens.insName.title.text.bold,
                  al_color_html(stru -> subScreens.insName.title.text.color),
                  stru -> subScreens.insName.title.corner.x,
                  stru -> subScreens.insName.title.corner.y,
                  ALLEGRO_ALIGN_CENTRE,
                  "%s", txtInsName[IN_TITLE]);

    // Draw character boxes
    // Modify X coordinates for the left most box
    stru -> subScreens.insName.characters.box.corner.x -= \
            (stru -> subScreens.insName.characters.box.width - BOX_OFFSET) * \
                                                  (((NAMESIZE - 1) / 2.0));

    for ( i = 0; i < NAMESIZE - 1; i++ )
    {
        // Draw box
        primitive_drawBox(&(stru -> subScreens.insName.characters.box));

        // Draw content
        al_draw_textf(stru -> subScreens.insName.characters.box.text.regular,
                      al_color_html( \
                       stru -> subScreens.insName.characters.box.text.color),
                      stru -> subScreens.insName.characters.box.corner.x + \
                      stru -> subScreens.insName.characters.box.width / 2 + \
                        BOX_IN_OFFSET,
                      stru -> subScreens.insName.characters.box.corner.y + \
                      /*stru -> subScreens.insName.characters.box.height / 2*/
                      BOX_IN_OFFSET,
                      ALLEGRO_ALIGN_CENTRE,
                      "%s", validCharacters[ \
                            stru -> subScreens.insName.characters.selChar[i]]);

        // Draw triangles
        if ( stru -> subScreens.insName.characters.selGUI == i )
        {
            // Top
            primitive_drawTriangle(TRI_TOP, \
                                  &(stru -> subScreens.insName.triangle.top), \
                                &(stru -> subScreens.insName.characters.box));
            //Bottom
            primitive_drawTriangle(TRI_BOTTOM, \
                               &(stru -> subScreens.insName.triangle.bottom), \
                                &(stru -> subScreens.insName.characters.box));
        }

        // Increase box coordinates
        stru -> subScreens.insName.characters.box.corner.x += \
                stru -> subScreens.insName.characters.box.width + BOX_OFFSET;
    }

    // Restore box coordinates
    stru -> subScreens.insName.characters.box.corner.x = charBoxCoordinateX;

    stru -> redraw = false;

    al_flip_display();
}

/**
 * @brief Initialize a button
 * 
 * @note The text color initialized is the one that appears when the button is
 * not selected
 * 
 * @param text Text that the button will have
 * @param button Button to initialize
 * 
 * @return Nothing
 */
static void
init_Btn (gbox_t * button, const char * text)
{
    button -> color.bkgnd = BTN_SEL_COLOR_BKGND;
    button -> color.border = BTN_SEL_COLOR_BDR;

    button -> thickness = BTN_SEL_THICKNESS;

    button -> text.rsize = BTN_TXT_SIZE;
    button -> text.regular = al_load_font(BTN_TXT_FONT_PATH,
                                          button -> text.rsize, 0);

    button -> text.bsize = BTN_TXT_SIZE;
    button -> text.bold = al_load_font(BTN_TXT_FONT_PATH,
                                       button -> text.rsize, 0);

    button -> text.color = BTN_TXT_COLOR;

    button -> width = al_get_text_width(button -> text.bold, text) + \
                      2 * BTN_TXT_OFFSET;
    button -> height = button -> text.bsize + 2 * BTN_TXT_OFFSET;
}

/**
 * @brief Initialize DISPLAYTOP screen
 * 
 * @param stru End Game screen structure
 * 
 * @return Nothing
 */
static void
init_displayTop (endGame_t * stru)
{
    // == Title ==
    box_initCommon(&(stru -> subScreens.displayTop.title));

    // Increase bold font
    stru -> subScreens.displayTop.title.text.bsize *= 2;
    stru -> subScreens.displayTop.title.text.bold = \
            al_load_font(GAME_TXT_FONT_BOLD_PATH, \
                            stru -> subScreens.displayTop.title.text.bsize, 0);

    // Height, width and corners coordinates
    stru -> subScreens.displayTop.title.height = \
            stru -> subScreens.displayTop.title.text.bsize + 4 * BOX_IN_OFFSET;
    stru -> subScreens.displayTop.title.width = \
            al_get_text_width(stru -> subScreens.displayTop.title.text.bold, \
                                                      txtDisplayTop[DT_TITLE]);

    stru -> subScreens.displayTop.title.corner.x = SCREEN_WIDTH / 2 - \
                                 stru -> subScreens.displayTop.title.width / 2;
    stru -> subScreens.displayTop.title.corner.y = ENDGAME_BOX_OFFSET;

    // == Top List ==
    box_initCommon(&(stru -> subScreens.displayTop.topList));

    // Height, width and corners coordinates
    stru -> subScreens.displayTop.topList.height = \
         NTOPSCORE * (GAME_TXT_SIZE + GAME_TXT_OFFSET - 1) + 2 * BOX_IN_OFFSET;
    stru -> subScreens.displayTop.topList.width = SCREEN_WIDTH - \
                                                  2 * ENDGAME_BOX_OFFSET;
    stru -> subScreens.displayTop.topList.corner.x = SCREEN_WIDTH / 2 - \
                            stru -> subScreens.displayTop.topList.width / 2;
    stru -> subScreens.displayTop.topList.corner.y = \
            stru -> subScreens.displayTop.title.corner.y + \
           stru -> subScreens.displayTop.title.height + ENDGAME_BOX_OFFSET / 2;

    // == Buttons ==
    // Initialize them
    init_Btn(&(stru -> subScreens.displayTop.button.playAgain),
             txtDisplayTop[DT_BUTTON0]);
    init_Btn(&(stru -> subScreens.displayTop.button.exit),
             txtDisplayTop[DT_BUTTON1]);

    // Set corners
    stru -> subScreens.displayTop.button.playAgain.corner.x = \
                                                           SCREEN_WIDTH / 2 - \
                      (stru -> subScreens.displayTop.button.playAgain.width + \
                         ENDGAME_BOX_OFFSET + \
                          stru -> subScreens.displayTop.button.exit.width) / 2;

    stru -> subScreens.displayTop.button.exit.corner.x = \
                    stru -> subScreens.displayTop.button.playAgain.corner.x + \
                    stru -> subScreens.displayTop.button.playAgain.width + \
                    ENDGAME_BOX_OFFSET;

    stru -> subScreens.displayTop.button.playAgain.corner.y = \
            stru -> subScreens.displayTop.button.exit.corner.y = \
            stru -> subScreens.displayTop.topList.corner.y + \
            stru -> subScreens.displayTop.topList.height + ENDGAME_BOX_OFFSET;
}

/**
 * @brief Initialize INSNAME screen
 * 
 * @param stru End Game screen structure
 * 
 * @return Nothing
 */
static void
init_insName (endGame_t * stru)
{
    // Counter
    int i = 0;

    int biggestCharWidth = 0;

    // == Title ==
    // Initialize
    box_initCommon(&(stru -> subScreens.insName.title));
    // Increase bold font
    stru -> subScreens.insName.title.text.bsize *= 2;
    stru -> subScreens.insName.title.text.bold = \
            al_load_font(GAME_TXT_FONT_BOLD_PATH, \
                         stru -> subScreens.insName.title.text.bsize, 0);

    // Height
    stru -> subScreens.insName.title.height = 2 * BOX_IN_OFFSET + \
                                   stru -> subScreens.insName.title.text.bsize;

    stru -> subScreens.insName.title.corner.x = SCREEN_WIDTH / 2;
    stru -> subScreens.insName.title.corner.y = BOX_OFFSET;


    // == Top triangle ==
    // Initialize
    triangle_initCommon(&(stru -> subScreens.insName.triangle.top));

    // == Characters ==
    // Initialize box
    box_initCommon(&(stru -> subScreens.insName.characters.box));

    // Increase font size
    stru -> subScreens.insName.characters.box.text.rsize *= 5;
    stru -> subScreens.insName.characters.box.text.regular = \
            al_load_font(GAME_TXT_FONT_PATH, \
                      stru -> subScreens.insName.characters.box.text.rsize, 0);

    // Text inside
    // Get number of posible characters and the size of the biggest in the list
    i = 0;
    while ( validCharacters[i] != NULL )
    {
        int tmpSize = al_get_text_width( \
        stru -> subScreens.insName.characters.box.text.regular,
                                        validCharacters[i]);

        // Biggest character width
        if ( tmpSize > biggestCharWidth )
        {
            biggestCharWidth = tmpSize;
        }

        // One more character
        i++;
    }

    stru -> subScreens.insName.characters.n = i - 1;

    // Set all selected options as the first character in the array

    for ( i = 0; i < NAMESIZE - 1; i++ )
    {
        memcpy(&(stru -> subScreens.insName.playerName[i]),
               validCharacters[0],
               1);
        /*strncpy(&(stru -> subScreens.insName.playerName[i]),
                validCharacters[0], 1);*/
        stru -> subScreens.insName.characters.selChar[i] = 0;
    }
    // Manually set NULL character at the end
    stru -> subScreens.insName.playerName[NAMESIZE - 1] = '\0';

    stru -> subScreens.insName.characters.box.width = 2 * BOX_IN_OFFSET + \
                                                      biggestCharWidth;

    stru -> subScreens.insName.characters.box.height = 2 * BOX_IN_OFFSET + \
                        stru -> subScreens.insName.characters.box.text.rsize;

    // Set cursor in the first character of the name
    stru -> subScreens.insName.characters.selGUI = 0;

    // == Bottom triangle ==
    // Initialize
    triangle_initCommon(&(stru -> subScreens.insName.triangle.bottom));

    // == Corners ==
    float groupYMiddle = (SCREEN_HEIGHT + \
                          stru -> subScreens.insName.title.height) / 2;

    // = Triangles =
    // X Coord
    stru -> subScreens.insName.triangle.top.corner[TRI_POINT].x = \
    stru -> subScreens.insName.triangle.bottom.corner[TRI_POINT].x = \
            SCREEN_WIDTH / 2;


    stru -> subScreens.insName.triangle.top.corner[TRI_BASE1].x = \
    stru -> subScreens.insName.triangle.bottom.corner[TRI_BASE1].x = \
            SCREEN_WIDTH / 2 - \
            stru -> subScreens.insName.triangle.top.width / 2;

    stru -> subScreens.insName.triangle.top.corner[TRI_BASE2].x = \
    stru -> subScreens.insName.triangle.bottom.corner[TRI_BASE2].x = \
            SCREEN_WIDTH / 2 + \
            stru -> subScreens.insName.triangle.top.width / 2;


    // Y Coord
    // Top
    stru -> subScreens.insName.triangle.top.corner[TRI_POINT].y = \
            groupYMiddle \
            - stru -> subScreens.insName.characters.box.height / 2 \
            - BOX_OFFSET - stru -> subScreens.insName.triangle.top.height;

    stru -> subScreens.insName.triangle.top.corner[TRI_BASE1].y = \
    stru -> subScreens.insName.triangle.top.corner[TRI_BASE2].y = \
            stru -> subScreens.insName.triangle.top.corner[TRI_POINT].y + \
            stru -> subScreens.insName.triangle.top.height;

    // Bottom
    stru -> subScreens.insName.triangle.bottom.corner[TRI_POINT].y = \
            groupYMiddle \
            + stru -> subScreens.insName.characters.box.height / 2 \
            + BOX_OFFSET + stru -> subScreens.insName.triangle.bottom.height;

    stru -> subScreens.insName.triangle.bottom.corner[TRI_BASE1].y = \
    stru -> subScreens.insName.triangle.bottom.corner[TRI_BASE2].y = \
            stru -> subScreens.insName.triangle.bottom.corner[TRI_POINT].y - \
            stru -> subScreens.insName.triangle.bottom.height;

    // = Characters =
    stru -> subScreens.insName.characters.box.corner.x = SCREEN_WIDTH / 2 - \
    stru -> subScreens.insName.characters.box.width / 2;


    stru -> subScreens.insName.characters.box.corner.y = \
            groupYMiddle - stru -> subScreens.insName.characters.box.height / 2;
}

/**
 * @brief Load screen background
 * 
 * If the background cannot be loaded, ENDGAME_BKGND_COLOR will be shown.
 * 
 * @param stru End Game screen structure
 * 
 * @return Nothing
 */
static void
load_bkgnd (endGame_t * stru)
{
    // Background
    al_clear_to_color(al_color_html(ENDGAME_BKGND_COLOR));

    stru -> bkgnd = al_load_bitmap(ENDGAME_BKGND);

    if ( stru -> bkgnd == NULL )
    {

        fputs("Error loading background.\n", stderr);
        return;
    }

    al_draw_scaled_bitmap(stru -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);
}

/**
 * @brief Manage display events for both, "arcade input" and top score printing
 * 
 * @param stru Current endGame_t structure.
 * @param screen Screen that it's being printed.
 * 
 * @return Nothing
 */
static void
manageEvents (endGame_t * stru, int screen)
{
    // Current event
    ALLEGRO_EVENT event;

    // Keys array
    static unsigned char key[ALLEGRO_KEY_MAX];

    // Used to keep track of the first time the function is executed
    static bool key_init = false;

    if ( stru == NULL )
    {
        fputs("NULL Structure passed.\n", stderr);
        return;
    }
    else if ( screen != DISPLAYTOP && screen != INSNAME )
    {
        fputs("Invalid screen passed.\n", stderr);
        return;
    }

    // Clear keys array the first time this functions is executed
    if ( key_init == false )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    // Wait for next event
    al_wait_for_event(stru -> evq, &event);

    // Check events
    switch ( event.type )
    {
            // Display
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            stru -> exit = true;
            break;

            // Keyboard
        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            break;

            // Timers
        case ALLEGRO_EVENT_TIMER:

            // Main timer
            if ( event.timer.source == stru -> timer.main )
            {
                checkKeys(key, stru, screen);
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Initialize common settings for triangles.
 * 
 * @param tri Triangle to be initialized with common settings.
 * 
 * @return Nothing
 */
static void
triangle_initCommon (triangle_t * tri)
{

    tri -> color.bkgnd = TRI_COLOR_BKGND;
    tri -> color.border = TRI_COLOR_BDR;
    tri -> thickness = TRI_THICKNESS;
    tri -> height = TRI_HEIGHT;
    tri -> width = TRI_WIDTH;
}

/**
 * @brief Set key as valid and then clear it
 * 
 * @param keyName Key to valid
 * @param keyArr Array with all the keys
 * @param counterArr Array with the key counter (to reduce sensitibility)
 * @param game Current game structure
 */
static void
validKey (int keyName,
          unsigned char * keyArr, unsigned char * counterArr,
          endGame_t * stru)
{
    // Clear key
    clearKey(keyName, keyArr);
    // Increase counter of this key
    counterArr[keyName]++;
    // Redraw screen
    stru -> redraw = true;
}
