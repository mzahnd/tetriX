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
 * @file    graph.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    16/12/2019, 15:27
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

#include "../../backend/board/board.h"
#include "../../backend/stats/stats_mgmt.h"

// This file
#include "graph.h"

/// @privatesection
// === Constants and Macro definitions ===
#define FPS 60.0
#define DISPLAY_W 800
#define DISPLAY_H 800
#define SQUARE_SIZE 80
#define COLORS  7

/**
 * @def CELL(r,c)
 * @brief Returns the information in the row (r) and column (c) of the board
 */
#define CELL(c,r,m) ( (m)[ ((r) * BOARD_WIDTH) + (c) ] )

// === Enumerations, structures and typedefs ===

typedef enum
{
    QUIT, MENU, PLAYING, PAUSED
} allegro_state;

const ALLEGRO_COLOR colors[TETROMINOS] = {
    al_map_rgb(250, 0, 0),
    al_map_rgb(0, 250, 0),
    al_map_rgb(0, 0, 250),
    al_map_rgb(250, 0, 250),
    al_map_rgb(250, 250, 0),
    al_map_rgb(0, 250, 250),
    al_map_rgb(100, 0, 100)
};

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

int
allegro ()
{
    allegro_t allegro;
    int pressed;
    allegro_state = MENU;

    if ( allegro_state = init_allegro(allegro) )
    {
        //running = init_bitmaps(draw_list);
        board_init(&allegro.board);
    }

    while ( allegro_state != QUIT )
    {
        al_get_next_event(allegro.queue, &(allegro.ev));
        switch ( allegro.ev.type )
        {
            case ALLEGRO_EVENT_TIMER:
                if ( al_is_event_queue_empty(allegro.queue) )
                {
                    allegro.board.update();
                    draw_board(&(allegro.board));
                }
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                allegro_state != QUIT;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                pressed = allegro.ev.keyboard.keycode;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                if ( allegro.ev.keyboard.keycode == pressed )
                    dispatch_key(&allegro);
                break;
            default:

                break;
        }
    }
}

void
draw_board (board_t* board)
{
    //draw background
    al_clear_to_color(al_map_rgb(0, 0, 0));
    grid_t * mat = board->ask.board;

    stats_t * stats = (stats_t *) board->ask.stats();

    int x, y;
    for ( y = 0; y < BOARD_HEIGHT; y++ )
    {
        for ( x = 0; x < BOARD_WIDTH; x++ )
        {
            if ( CELL(x, y, mat) > 0 )
            {
                al_draw_filled_rectangle(y * SQUARE_SIZE - SQUARE_SIZE / 2,
                                         x * SQUARE_SIZE - SQUARE_SIZE / 2,
                                         y * SQUARE_SIZE + SQUARE_SIZE / 2,
                                         x * SQUARE_SIZE + SQUARE_SIZE / 2,
                                         colors[CELL(x, y, mat)]);
            }

            else if ( CELL(x, y, mat) == CELL_MOVING )
            {

                al_draw_filled_rectangle(y * SQUARE_SIZE - SQUARE_SIZE / 2,
                                         x * SQUARE_SIZE - SQUARE_SIZE / 2,
                                         y * SQUARE_SIZE + SQUARE_SIZE / 2,
                                         x * SQUARE_SIZE + SQUARE_SIZE / 2,
                                         colors[stats->piece.current]);
            }
        }
    }

    al_flip_display();
}

void
dispatch_key (allegro_t* allegro)
{
    int r;
    switch ( allegro.ev.keyboard.keycode )
    {
        case ALLEGRO_KEY_UP:
            if ( allegro->state == MENU || allegro->state == PAUSED )
            {
                //hace algo
            }
            break;
        case ALLEGRO_KEY_DOWN:
            if ( allegro->state == MENU || allegro->state == PAUSED )
            {
                //hace algo
            }
            else if ( allegro->state == PLAYING )
            {
                allegro->board->piece.softDrop();
            }
            break;
        case ALLEGRO_KEY_LEFT:
            if ( allegro->state == PLAYING )
            {
                allegro->board->piece.shift(LEFT);
                al_start_timer(allegro->timer);
            }
            break;
        case ALLEGRO_KEY_RIGHT:
            if ( allegro->state == PLAYING )
            {
                allegro->board->piece.shift(RIGHT);
                al_start_timer(allegro->timer);
            }
            break;
        case ALLEGRO_KEY_Z:
            if ( allegro->state == PLAYING )
            {
                allegro->board->piece.rotate(LEFT);
                al_start_timer(allegro->timer);
            }
            break;
        case ALLEGRO_KEY_X:
            if ( allegro->state == PLAYING )
            {
                allegro->board->piece.rotate(RIGHT);
                al_start_timer(allegro->timer);
            }
            break;
        case ALLEGRO_KEY_P:
            if ( allegro->state == PLAYING )
            {
                allegro->state = PAUSED;
            }
            else if ( allegro->state == PAUSED )
            {
                allegro->state = PLAYING;
            }
            break;
        case ALLEGRO_KEY_ENTER:
            if ( allegro->state == MENU || allegro->state == PAUSED )
            {
                //hace algo
            }

            break;
        default:
            //nada
            break;
    }

}

//bool init_bitmaps(ALLEGRO_BITMAP** draw_list){
//    int i = 0;
//    for (i = 0; i < COLORS; i++){
//        *(draw_list + i) = al_load_bitmap
//    }
//}

void
destroy_allegro (allegro_t* allegro)
{
    //destroy_bitmaps(draw_list);

    al_destroy_display(allegro->display);
    al_destroy_timer(allegro->timer);
    al_destroy_event_queue(allegro->queue);
    al_uninstall_keyboard();
    al_uninstall_system();
}

bool
init_allegro (allegro_t* allegro)
{
    bool r = false;
    if ( al_init() )
    {
        if ( al_init_image_addon() )
        {
            if ( al_init_primitives_addon() )
            {
                if ( al_install_keyboard() )
                {
                    allegro->timer = al_create_timer(1 / FPS);
                    allegro->display = al_create_display(DISPLAY_W, DISPLAY_H);
                    allegro->queue = al_create_event_queue();
                    al_register_event_source(allegro->queue, al_get_display_event_source(allegro->display));
                    al_register_event_source(allegro->queue, al_get_timer_event_source(allegro->timer));
                    al_register_event_source(allegro->queue, al_get_keyboard_event_source());
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    al_flip_display();
                    al_start_timer();
                    r = true;
                }
                printf("No se pudo inicializar el teclado");
            }
            printf("No se pudo inicializar el primitives addon");
        }
        printf("No se pudo inicializar el image addon");
    }
    printf("No se pudo inicializar Allegro");

    return r;
}

/// @privatesection
// === Local function definitions ===
