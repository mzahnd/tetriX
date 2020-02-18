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
 * @file    graph.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    16/12/2019, 15:33
 * 
 * @copyright GNU General Public License v3
 */

#ifndef GRAPH_H
#    define GRAPH_H

// === Libraries and header files ===
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "../../backend/board/board.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

typedef struct
{
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT ev;
    ALLEGRO_STATE state;
    board_t board;
}allegro_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
int allegro();
bool init_allegro();
void dispatch_key (allegro_t *);
void draw_board(board_t*);
#endif /* GRAPH_H */
