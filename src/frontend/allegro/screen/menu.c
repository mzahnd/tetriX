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
 * @brief   ;
 * 
 * @details ; 
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

// This file
#include "menu.h"

/// @privatesection
// === Constants and Macro definitions ===
#define BKGND_WIDTH         1920
#define BKGND_HEIGHT        1280

#define BKGND_COLOR         "#FFFFFF"

#define TXT_NUM             4
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

#define TXT_EXIT_NUM        3
#define TXT_EXIT_COLOR      "#0613DD"
#define TXT_EXIT_SIZE       (TXT_SIZE * 2.5)

#define FPS                 60.0

#define KEY_SEEN            1
#define KEY_RELEASED        2
// === Enumerations, structures and typedefs ===

enum
{
    NONE,
    PLAY,
    EXIT
};

typedef struct
{
    // Display
    ALLEGRO_DISPLAY * display;
    // Background
    ALLEGRO_BITMAP * bkgnd;

    // Event queue
    ALLEGRO_EVENT_QUEUE * evq;

    struct
    {
        ALLEGRO_TIMER * main;
    } timer;

    // Selected option in the menu
    int txt_sel;

    int action;
    bool redraw;

} menu_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

static void
checkKeys (menu_t * menu, unsigned char key[ALLEGRO_KEY_MAX]);

static void
drawExit (menu_t * menu);

static void
drawMainMenu (menu_t * menu);

static void
destroy (menu_t * menu);

static void
manageEvents (menu_t * menu);

static void
selectText (ALLEGRO_FONT * font, const menu_t * const menu);

// === ROM Constant variables with file level scope ===
static const float txt_pos[TXT_NUM][2] = {
    {SCREEN_WIDTH / 5.5 + TXT_OFFSET, SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH / 5.5 + TXT_OFFSET, SCREEN_HEIGHT / 2.3},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH * 0.78, SCREEN_HEIGHT / 2.3},
};

static const float txt_exit_pos[TXT_EXIT_NUM][2] = {
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 4},
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30},
    {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 - 50}

};

static const char * txt_str[TXT_NUM] = {
    "PLAY", "OPTIONS", "ABOUT", "EXIT"
};

static const char * txt_exit_str[TXT_EXIT_NUM] = {
    "THANKS", "FOR", "PLAYING!"
};

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

int
alg_menu (allegro_t * alStru)
{
    menu_t menu;

    menu.display = NULL;
    menu.bkgnd = NULL;
    menu.txt_sel = 0;
    menu.action = NONE;
    menu.redraw = false;

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

    // Enable antialiasing
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 10, ALLEGRO_SUGGEST);
    // Show nicer bitmaps
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Create display

    menu.display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if ( menu.display == NULL )
    {
        fputs("Error creating main menu.", stderr);
        alg_destroy();
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

    // Draw and display window
    while ( menu.action == NONE )
    {
        // Draw main menu
        drawMainMenu(&menu);

        al_flip_display();

        menu.redraw = false;

        while ( menu.redraw == false && menu.action == NONE )
        {
            manageEvents(&menu);
        }
    }

    if ( menu.action == EXIT )
    {
        drawExit(&menu);
        sleep(5);

        destroy(&menu);
    }

    if ( menu.action == PLAY )
    {
        destroy(&menu);
        alg_game(alStru);
    }

    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

static void
checkKeys (menu_t * menu, unsigned char key[ALLEGRO_KEY_MAX])
{
    if ( key[ALLEGRO_KEY_UP] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_UP] &= KEY_SEEN;

        // Move up one position in the menu
        if ( menu -> txt_sel == 1 || menu -> txt_sel == 3 )
        {
            (menu -> txt_sel)--;
            menu -> redraw = true;
        }

    }

    if ( key[ALLEGRO_KEY_DOWN] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_DOWN] &= KEY_SEEN;

        // Move down one position in the menu
        if ( menu -> txt_sel == 0 || menu -> txt_sel == 2 )
        {
            (menu -> txt_sel)++;
            menu -> redraw = true;
        }

    }
    if ( key[ALLEGRO_KEY_LEFT] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_LEFT] &= KEY_SEEN;

        // Move up left position in the menu
        if ( menu -> txt_sel == 2 || menu -> txt_sel == 3 )
        {
            (menu -> txt_sel) -= 2;
            menu -> redraw = true;
        }

    }

    if ( key[ALLEGRO_KEY_RIGHT] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_RIGHT] &= KEY_SEEN;

        // Move right one position in the menu
        if ( menu -> txt_sel == 0 || menu -> txt_sel == 1 )
        {
            (menu -> txt_sel) += 2;
            menu -> redraw = true;
        }
    }

    if ( key[ALLEGRO_KEY_ENTER] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_ENTER] &= KEY_SEEN;

        switch ( menu -> txt_sel )
        {
            case 0:
                menu -> action = PLAY;
                break;

            case 1:
                break;

            case 2:
                break;

            case 3:
                menu -> action = EXIT;
                break;

            default:
                break;
        }
    }
}

static void
drawMainMenu (menu_t * menu)
{
    static int first_time = true;
    int i;

    ALLEGRO_FONT * text = al_load_font(TXT_FONT_PATH, TXT_SIZE, 0);

    al_clear_to_color(al_color_html(BKGND_COLOR));

    if ( first_time == true )
    {
        menu -> bkgnd = al_load_bitmap("res/images/main/background.png");

        if ( menu -> bkgnd == NULL )
        {
            fputs("Error loading background.", stderr);
            return;
        }

        first_time = false;
    }

    al_draw_scaled_bitmap(menu -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);

    selectText(text, menu);

    // Draw text
    for ( i = 0; i < TXT_NUM; i++ )
    {
        if ( i != menu->txt_sel )
        {
            al_draw_text(text, al_color_html(TXT_COLOR),
                         txt_pos[i][0], txt_pos[i][1],
                         ALLEGRO_ALIGN_CENTRE, txt_str[i]);
        }

        else
        {
            al_draw_text(text, al_color_html(TXT_COLOR_SEL),
                         txt_pos[i][0], txt_pos[i][1],
                         ALLEGRO_ALIGN_CENTRE, txt_str[i]);
        }
    }
}

static void
drawExit (menu_t * menu)
{
    int i;
    ALLEGRO_FONT * text = al_load_font(TXT_FONT_PATH, TXT_EXIT_SIZE, 0);

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
    al_flip_display();

}

static void
destroy (menu_t * menu)
{
    al_destroy_timer(menu -> timer.main);
    al_destroy_event_queue(menu -> evq);
    al_destroy_bitmap(menu -> bkgnd);
    al_destroy_display(menu -> display);
}

static void
manageEvents (menu_t * menu)
{
    ALLEGRO_EVENT event;

    static unsigned char key[ALLEGRO_KEY_MAX];
    static char key_init = false;

    if ( key_init == false )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    if ( al_get_next_event(menu -> evq, &event) )
    {
        switch ( event.type )
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                menu -> action = EXIT;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_TIMER:
                if ( event.timer.source == menu -> timer.main )
                {
                    checkKeys(menu, key);
                }
                break;

            default:
                break;
        }
    }
}

static void
selectText (ALLEGRO_FONT * font, const menu_t * const menu)
{
    const int * pos = &(menu -> txt_sel);

    int width = al_get_text_width(font, txt_str[*pos]);

    al_draw_filled_rounded_rectangle(\
                                   txt_pos[*pos][0] - TXT_OFFSET - (width / 2),
                                     txt_pos[*pos][1] - TXT_OFFSET, \
                                   txt_pos[*pos][0] + TXT_OFFSET + (width / 2),
                                     txt_pos[*pos][1] + TXT_OFFSET + TXT_SIZE,\
                                 25.0, 25.0,
                                     al_color_html(TXT_COLOR_SEL_BKGND));

    al_draw_rounded_rectangle(
                              txt_pos[*pos][0] - TXT_OFFSET - (width / 2),
                              txt_pos[*pos][1] - TXT_OFFSET,
                              txt_pos[*pos][0] + TXT_OFFSET + (width / 2),
                              txt_pos[*pos][1] + TXT_OFFSET + TXT_SIZE,
                              25.0, 25.0,
                              al_color_html(TXT_COLOR_SEL_BDR), 2.5);
}
