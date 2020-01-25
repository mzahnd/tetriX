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
 * @file    menu.c
 * 
 * @brief   Main menu when running with Allegro
 * 
 * @details Displays the main menu to the user
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    17/12/2019, 12:26
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

// Allegro
#include <allegro5/allegro5.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <allegro5/allegro_color.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h> 

#include "game.h"

#include "../keys.h"

// This file
#include "menu.h"

/// @privatesection
// === Constants and Macro definitions ===
#define BKGND_WIDTH         1920
#define BKGND_HEIGHT        1280

#define BKGND_COLOR         "#FFFFFF"

#define TXT_NUM             3
#define TXT_COLOR           "#574642"
#define TXT_COLOR_SEL       "#E6F1F9"
//#define TXT_COLOR_SEL_BDR   "#349ED8"
#define TXT_COLOR_SEL_BDR   "#000000"
#define TXT_COLOR_SEL_BKGND "#65E724"
//#define TXT_COLOR_SEL_BKGND "#3bb5f7"
#define TXT_SIZE            40
#define TXT_FONT_PATH       \
                          "res/fonts/liberation_serif/LiberationSerif-Bold.ttf"
#define TXT_OFFSET          10

#define BTN_ROUNDNESS       25.0
#define BTN_THICKNESS       2.5

#define TXT_EXIT_NUM        3
#define TXT_EXIT_COLOR      "#0613DD"
#define TXT_EXIT_SIZE       (TXT_SIZE * 2.5)

#define TXT_ABOUT_NUM       6

#define FPS                 60.0

#define LOGO_FILE_WIDTH     490
#define LOGO_FILE_HEIGHT    166
#define LOGO_WIDTH          LOGO_FILE_WIDTH / 2
#define LOGO_HEIGHT         LOGO_FILE_HEIGHT / 2

#define FX_BYEBYE           "res/audio/fx/chau.wav"

// === Enumerations, structures and typedefs ===

typedef struct
{
    // Public structure
    menu_t public;

    // Game display
    ALLEGRO_DISPLAY * display;
    // Display background
    ALLEGRO_BITMAP * bkgnd;

    // Event queue
    ALLEGRO_EVENT_QUEUE * evq;

    // Timers

    struct
    {
        // Main timer
        ALLEGRO_TIMER * main;
    } timer;

} menu_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

static void
drawExit (menu_private_t * menu);

static void
drawMainMenu (menu_private_t * menu);

static void
drawAbout (menu_private_t * menu);

static void
destroy (menu_private_t * menu);

static void
manageEvents (menu_private_t * menu);

static void
selectInMenu (const ALLEGRO_FONT * font, const menu_private_t * menu);

static void
selectText (const int tlx, const int tly, const int textWidth);

// === ROM Constant variables with file level scope ===
// Buttons text in main menu
static const char * txt_str[TXT_NUM] = {
    "PLAY", "ABOUT", "EXIT"
};

// Buttons text positions (x, y)
static const float txt_pos[TXT_NUM][2] = {
    {SCREEN_WIDTH / 5.5 + TXT_OFFSET, SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 2.3},
};

// Text drawn when the game is exited
static const char * txt_exit_str[TXT_EXIT_NUM] = {
    "THANKS", "FOR", "PLAYING!"
};

// Exit text position
static const float txt_exit_pos[TXT_EXIT_NUM][2] = {
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30},
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 - 50}

};

// Text shown in the about screen
static const char * txt_about_str[TXT_ABOUT_NUM] = {
    "Developed by", "Gino Minnucci", "Martin E. Zahnd",
    "Version", "February 2020", "0.0.1"
};

// About text's position
static const float txt_about_pos[TXT_ABOUT_NUM][2] = {
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 5},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 5 + TXT_SIZE + TXT_OFFSET},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 5 + 2 * TXT_SIZE + 2 * TXT_OFFSET},
    {SCREEN_WIDTH / 4.8, SCREEN_HEIGHT / 5},
    {SCREEN_WIDTH / 4.8, SCREEN_HEIGHT / 5 + TXT_SIZE + TXT_OFFSET},
    {SCREEN_WIDTH / 4.8, SCREEN_HEIGHT / 5 + 2 * TXT_SIZE + 2 * TXT_OFFSET}
};

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Display game menu
 * 
 * Draws the main menu, as well as the about and exit screens
 * 
 * @param alStru Structure with an already initialized display and system
 * 
 * @return AL_OK if everything is fine
 * @return AL_ERROR if something failed
 */
int
alg_menu (allegro_t * alStru)
{
    menu_private_t menu;

    (alStru -> samples.fx.enabled) ? 1 : (alStru -> samples.fx.invertStatus());
    (alStru -> samples.music.enabled) ?
            1 : (alStru -> samples.music.invertStatus());

    menu.display = NULL;
    menu.bkgnd = NULL;
    menu.public.txt_sel = 0;
    menu.public.action = NONE;
    menu.public.redraw = false;

    // Verify if everything is in order
    if ( alStru == NULL )
    {
        return AL_ERROR;
    }

    if ( al_is_keyboard_installed() == false )
    {
        return AL_ERROR;
    }

    // == Display ==
    // Create display
    menu.display = alStru -> screen.display;

    // Load background
    menu.bkgnd = al_load_bitmap("res/images/main/background.png");

    if ( menu.bkgnd == NULL )
    {
        fputs("Error loading background.", stderr);
        return AL_ERROR;
    }

    // == Timers ==
    // Main timer
    menu.timer.main = al_create_timer(1.0 / FPS);

    if ( !menu.timer.main )
    {
        fputs("Failed to create timer.", stderr);
        destroy(&menu);
        alg_destroy();
        return AL_ERROR;
    }

    // == Event Queue ==
    menu.evq = al_create_event_queue();

    if ( !menu.evq )
    {
        fputs("Failed to create event queue.", stderr);
        destroy(&menu);
        alg_destroy();
        return AL_ERROR;
    }

    // Register events
    // Display
    al_register_event_source(menu.evq,
                             al_get_display_event_source(menu.display));
    // Keyboard
    al_register_event_source(menu.evq, al_get_keyboard_event_source());
    // Main timer
    al_register_event_source(menu.evq,
                             al_get_timer_event_source(menu.timer.main));

    // Start timers
    al_start_timer(menu.timer.main);

    // Draw and display screen
    while ( menu.public.action != EXIT )
    {
        switch ( menu.public.action )
        {
                // Main menu
            case NONE:
                // Draw main menu
                drawMainMenu(&menu);

                al_flip_display();

                menu.public.redraw = false;

                while ( menu.public.redraw == false &&
                        menu.public.action == NONE )
                {
                    manageEvents(&menu);
                }
                break;

                // Game
            case PLAY:
                alg_game(alStru);

                // Clear events from queue to avoid catching a wrong one
                al_flush_event_queue(menu.evq);

                if ( alStru -> exit == true )
                {
                    // Perform menu actions
                    menu.public.action = NONE;

                    // Restore option
                    alStru -> exit = false;
                }
                else
                {
                    // Play again!
                    menu.public.action = PLAY;
                }
                break;

                // About screen
            case ABOUT:
                drawAbout(&menu);

                while ( menu.public.action == ABOUT )
                {
                    manageEvents(&menu);
                }
                break;

            default:
                break;
        }
    }

    alStru -> exit = true;

    // Play exit sound and stop music
    (alStru -> samples.music.status) ? (alStru -> samples.music.stop()) : 0;
    alStru -> samples.fx.play(FX_BYEBYE);
    // Display exit screen
    drawExit(&menu);
    sleep(3);

    // Destroy everything
    destroy(&menu);

    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Draw main menu screen in the game display
 * 
 * This is the first screem the user sees when the game is opened
 * 
 * @param menu Used to draw the screen background and call selectInMenu()
 * 
 * @return Nothing
 */
static void
drawMainMenu (menu_private_t * menu)
{
    // Counter
    int i;

    // Text font
    ALLEGRO_FONT * text = al_load_font(TXT_FONT_PATH, TXT_SIZE, 0);

    // Background
    al_clear_to_color(al_color_html(BKGND_COLOR));

    al_draw_scaled_bitmap(menu -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);

    // Draw selection rectangle
    selectInMenu(text, menu);

    // Draw text
    for ( i = 0; i < TXT_NUM; i++ )
    {
        // Selected
        if ( i != menu -> public.txt_sel )
        {
            al_draw_text(text, al_color_html(TXT_COLOR),
                         txt_pos[i][0], txt_pos[i][1],
                         ALLEGRO_ALIGN_CENTRE, txt_str[i]);
        }

            // Not selected
        else
        {
            al_draw_text(text, al_color_html(TXT_COLOR_SEL),
                         txt_pos[i][0], txt_pos[i][1],
                         ALLEGRO_ALIGN_CENTRE, txt_str[i]);
        }
    }
}

/**
 * @brief Draw about screen in the game display
 * 
 * @param menu Used to draw the screen background
 * 
 * @return Nothing
 */
static void
drawAbout (menu_private_t * menu)
{
    // Counter
    int i;

    // ITBA Logo bitmap
    ALLEGRO_BITMAP * logo;

    // Text font
    ALLEGRO_FONT * txtFont = al_load_font(TXT_FONT_PATH, TXT_SIZE * 0.8, 0);
    // Button font
    ALLEGRO_FONT * btnFont = al_load_font(TXT_FONT_PATH, TXT_SIZE, 0);

    // Button text
    const char * buttonText = "Return";

    // Button text position (x,y)
    const int txtPos[2] = {SCREEN_WIDTH / 2, 7 * SCREEN_HEIGHT / 9};

    // Background
    al_clear_to_color(al_color_html(BKGND_COLOR));

    if ( menu -> bkgnd == NULL )
    {
        menu -> bkgnd = al_load_bitmap("res/images/main/background.png");

        if ( menu -> bkgnd == NULL )
        {
            fputs("Error loading background.", stderr);
            return;
        }
    }

    al_draw_scaled_bitmap(menu -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);

    // Button
    // Selection rectangle
    selectText(txtPos[0], txtPos[1],
               al_get_text_width(btnFont, buttonText));

    // Text
    al_draw_text(btnFont, al_color_html(TXT_COLOR_SEL),
                 txtPos[0], txtPos[1],
                 ALLEGRO_ALIGN_CENTRE, buttonText);

    // ITBA Logo
    logo = al_load_bitmap("res/images/main/itba_logo.png");
    if ( logo == NULL )
    {
        fputs("Error loading logo.", stderr);
    }

    else
    {
        al_draw_scaled_bitmap(logo,
                              0, 0, LOGO_FILE_WIDTH, LOGO_FILE_HEIGHT,
                              SCREEN_WIDTH / 2 - LOGO_WIDTH / 2,
                              SCREEN_HEIGHT * 5 / 9 - LOGO_HEIGHT / 2,
                              LOGO_WIDTH, LOGO_HEIGHT,
                              0);
    }

    // Text
    for ( i = 0; i < TXT_ABOUT_NUM; i++ )
    {
        al_draw_textf(txtFont, al_color_html(TXT_COLOR),
                      txt_about_pos[i][0], txt_about_pos[i][1],
                      ALLEGRO_ALIGN_CENTRE, txt_about_str[i]);
    }

    // Flip
    al_flip_display();
}

/**
 * @brief Draw exit screen in the game display
 * 
 * @param menu Used to draw the screen background
 * 
 * @return Nothing
 */
static void
drawExit (menu_private_t * menu)
{
    // Counter
    int i;
    // Text font
    ALLEGRO_FONT * text = al_load_font(TXT_FONT_PATH, TXT_EXIT_SIZE, 0);

    // Background
    al_clear_to_color(al_color_html(BKGND_COLOR));

    if ( menu -> bkgnd == NULL )
    {
        menu -> bkgnd = al_load_bitmap("res/images/main/background.png");

        if ( menu -> bkgnd == NULL )
        {
            fputs("Error loading background.", stderr);
            return;
        }
    }

    al_draw_scaled_bitmap(menu -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);

    // Draw text
    for ( i = 0; i < TXT_EXIT_NUM; i++ )
    {
        al_draw_textf(text, al_color_html(TXT_EXIT_COLOR),
                      txt_exit_pos[i][0], txt_exit_pos[i][1],
                      ALLEGRO_ALIGN_CENTRE, txt_exit_str[i]);
    }

    // Flip
    al_flip_display();
}

/**
 * @brief Destroy main menu (previous to exit the game)
 * 
 * @param menu Structure to destroy
 * 
 * @return Nothing
 */
static void
destroy (menu_private_t * menu)
{
    // Timer
    al_destroy_timer(menu -> timer.main);
    // Event Queue
    al_destroy_event_queue(menu -> evq);
    // Bitmaps
    al_destroy_bitmap(menu -> bkgnd);
}

/**
 * @brief Manage main menu events, such as timers key pressing and display
 * 
 * @param menu
 * 
 * @return Nothing
 */
static void
manageEvents (menu_private_t * menu)
{
    // Current event
    ALLEGRO_EVENT event;

    // Keys array
    static unsigned char key[ALLEGRO_KEY_MAX];
    // Keys have been initialized
    static char key_init = false;

    // Initialize keys (only the first time the function is run)
    if ( key_init == false )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    // Events
    if ( al_get_next_event(menu -> evq, &event) )
    {
        switch ( event.type )
        {
                // Display
                // Close
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                menu -> public.action = EXIT;
                break;

                // Keyboard
                // Key pressed
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

                // Key released
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

                // Timers
            case ALLEGRO_EVENT_TIMER:
                // Main timer
                if ( event.timer.source == menu -> timer.main )
                {
                    // Check keys
                    mainMenuKeys(&(menu -> public), key);
                }
                break;

            default:
                break;
        }
    }
}

/**
 * @brief Draw a rectangle around the selected text (to create a button)
 * 
 * @param font Text font
 * @param menu Main menu structure
 * 
 * @return Nothing
 */
static void
selectInMenu (const ALLEGRO_FONT * font, const menu_private_t * menu)
{
    // Get text selected
    const int * pos = &(menu -> public.txt_sel);

    // Draw rectangle
    selectText(txt_pos[*pos][0], txt_pos[*pos][1],
               al_get_text_width(font, txt_str[*pos]));
}

/**
 * @brief Draw rectangle around text
 * 
 * @param tlx Top Left X coordinate of the rectangle
 * @param tly Top Left Y coordinate of the rectangle
 * @param textWidth Text With
 * 
 * @return Nothing
 */
static void
selectText (const int tlx, const int tly, const int textWidth)
{
    // Draw background
    al_draw_filled_rounded_rectangle(\
                                   tlx - TXT_OFFSET - (textWidth / 2),
                                     tly - TXT_OFFSET, \
                                   tlx + TXT_OFFSET + (textWidth / 2),
                                     tly + TXT_OFFSET + TXT_SIZE,\
                                 BTN_ROUNDNESS, BTN_ROUNDNESS,
                                     al_color_html(TXT_COLOR_SEL_BKGND));

    // Draw border
    al_draw_rounded_rectangle(
                              tlx - TXT_OFFSET - (textWidth / 2),
                              tly - TXT_OFFSET,
                              tlx + TXT_OFFSET + (textWidth / 2),
                              tly + TXT_OFFSET + TXT_SIZE,
                              BTN_ROUNDNESS, BTN_ROUNDNESS,
                              al_color_html(TXT_COLOR_SEL_BDR), BTN_THICKNESS);
}
