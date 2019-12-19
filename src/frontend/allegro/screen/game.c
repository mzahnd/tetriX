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
 * @file    gameScreen.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    17/12/2019, 11:29
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     // For strcmp

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

// Game logic
#include "../../../backend/board/board.h"
#include "../../../backend/stats/stats_mgmt.h"

// This file
#include "game.h"

/// @privatesection
// === Constants and Macro definitions ===
#define KEY_SEEN            1
#define KEY_RELEASED        2
#define FPS                 60.0

#define BKGND_COLOR         "#000000"
#define BKGND_WIDTH         1920
#define BKGND_HEIGHT        1280

#define CELL_WIDTH          25.0
#define CELL_HEIGHT         CELL_WIDTH

#define ROUND_X             2.0
#define ROUND_Y             2.0

#define BOX_OFFSET          20.0
#define BOX_IN_OFFSET       5.0

#define TXT_SIZE            32

#define MS2S(t)             ((t) / 1000.0)

#define BOX_THIKNESS        3.0
#define CELL_THIKNESS       2.0

// === Enumerations, structures and typedefs ===

enum game_status
{
    NEW = 0,
    PLAYING,
    PAUSED
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
        ALLEGRO_TIMER * piece;
        ALLEGRO_TIMER * main;

        float piece_speed;
    } timer;

    // Selected option in the game
    int status;

    bool exit;
    bool redraw;

} game_t;

typedef struct
{
    float width;
    float height;

    float thickness;

    struct
    {
        const char * border;
        const char * bkgnd;

    } color;

    struct
    {
        float x;
        float y;
    } corner;
} gbox_t;

typedef struct
{
    const stats_t * stats;

    gbox_t boardBox;
    gbox_t gridBox[TETROMINOS];

    grid_t * r0c0;
} screenBoard_t;

typedef struct
{
    const stats_t * stats;

    gbox_t scoreBox;

    gbox_t levelBox;

    gbox_t nextPieceBox;

    gbox_t piecesBox;

} screenStats_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX], game_t * game, board_t * logic);

static void
drawBox (gbox_t * box);

static void
drawGameBoard (screenBoard_t * board);

static void
drawBlock (int x, int y, gbox_t * block);

static void
drawStats (screenStats_t * stats);

static void
drawScreen (game_t * game);

static void
destroy (game_t * game);

static void
init_scrBoard (screenBoard_t * board, board_t * logic);

static void
init_scrStats (screenStats_t * stats, board_t * logic);

static void
manageEvents (game_t * game, board_t * boardLogic);

// === ROM Constant variables with file level scope ===
static const char * tetrominos_bkgnd[TETROMINOS + 1] = {
    "#00A2E8", "#3F48CC", "#FF7F27", "#FFC90E",
    "#22B14C", "#A349A4", "#ED1C24",
    "#F0F0F0"
};

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

int
alg_game (allegro_t * alStru)
{
    game_t game;
    screenBoard_t screenBoard;
    screenStats_t screenStats;
    board_t boardLogic;

    // Verify if everything is in order
    if ( alStru == NULL )
    {
        return AL_ERROR;
    }

    if ( al_is_keyboard_installed() == false )
    {
        return AL_ERROR;
    }

    game.display = NULL;
    game.bkgnd = NULL;
    game.evq = NULL;
    game.timer.piece_speed = 1000;
    game.timer.piece = NULL;
    game.timer.main = NULL;
    game.status = NEW;
    game.exit = false;
    game.redraw = true;

    // Init game logic
    board_init(&boardLogic);

    if ( boardLogic.init == false )
    {
        fputs("Error initializing boardLogic.", stderr);
        return AL_ERROR;
    }

    init_scrBoard(&screenBoard, &boardLogic);
    init_scrStats(&screenStats, &boardLogic);

    // == Display ==

    // Enable antialiasing
    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 10, ALLEGRO_SUGGEST);
    // Show nicer bitmaps
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Create display

    game.display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if ( game.display == NULL )
    {
        fputs("Error creating main game.", stderr);
        alg_destroy();
        return AL_ERROR;
    }

    // == Timers ==
    // Main timer
    game.timer.main = al_create_timer(1.0 / FPS);

    if ( !game.timer.main )
    {
        fputs("Failed to create timer.", stderr);
        destroy(&game);
        alg_destroy();
        return AL_ERROR;
    }

    game.timer.piece = al_create_timer(MS2S(game.timer.piece_speed));

    if ( !game.timer.piece )
    {
        fputs("Failed to create timer.", stderr);
        destroy(&game);
        alg_destroy();
        return AL_ERROR;
    }

    // == Event Queue ==
    game.evq = al_create_event_queue();

    if ( !game.evq )
    {
        fputs("Failed to create event queue.", stderr);
        destroy(&game);
        alg_destroy();
        return AL_ERROR;
    }

    // Register events
    // Display
    al_register_event_source(game.evq,
                             al_get_display_event_source(game.display));
    // Keyboard
    al_register_event_source(game.evq, al_get_keyboard_event_source());
    // Main timer
    al_register_event_source(game.evq,
                             al_get_timer_event_source(game.timer.main));

    // Piece timer
    al_register_event_source(game.evq,
                             al_get_timer_event_source(game.timer.piece));

    // Start timers
    al_start_timer(game.timer.main);
    al_start_timer(game.timer.piece);

    // Draw and display window
    while ( game.exit == false )
    {
        // Draw game screen
        drawScreen(&game);

        // Draw board
        drawGameBoard(&screenBoard);

        // Draw Stats
        drawStats(&screenStats);

        al_flip_display();

        game.redraw = false;
        while ( game.redraw == false && game.exit == false )
        {
            manageEvents(&game, &boardLogic);
        }
    }

    destroy(&game);

    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX], game_t * game, board_t * logic)
{
    if ( key[ALLEGRO_KEY_UP] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_UP] &= KEY_SEEN;

        // Move up one position in the game
        ;

    }

    if ( key[ALLEGRO_KEY_DOWN] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_DOWN] &= KEY_SEEN;

        // Move down one position in the game
        logic -> piece.softDrop();

        game -> redraw = true;
    }
    if ( key[ALLEGRO_KEY_LEFT] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_LEFT] &= KEY_SEEN;

        // Move up left position in the game
        logic -> piece.shift(LEFT);

        game -> redraw = true;
    }

    if ( key[ALLEGRO_KEY_RIGHT] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_RIGHT] &= KEY_SEEN;

        // Move right one position in the game
        logic -> piece.shift(RIGHT);

        game -> redraw = true;
    }

    if ( key[ALLEGRO_KEY_ENTER] )
    {
        // Clear key from array
        key[ALLEGRO_KEY_ENTER] &= KEY_SEEN;

        switch ( game -> status )
        {
            case NEW:
            case PLAYING:
                game -> status = PAUSED;
                break;

            case PAUSED:
                game -> status = PLAYING;
        }
    }

    if ( key[ALLEGRO_KEY_Q] )
    {
        key[ALLEGRO_KEY_Q] &= KEY_SEEN;
        game -> exit = true;
    }
}

static void
drawScreen (game_t * game)
{
    static int first_time = true;

    al_clear_to_color(al_color_html(BKGND_COLOR));

    if ( first_time == true )
    {
        game -> bkgnd = al_load_bitmap("res/images/game/background.png");

        if ( game -> bkgnd == NULL )
        {
            fputs("Error loading background.", stderr);
            return;
        }

        first_time = false;
    }

    al_draw_scaled_bitmap(game -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);
}

static void
drawGameBoard (screenBoard_t * board)
{
    int x, y;
    grid_t current;

    drawBox(&(board -> boardBox));

    for ( y = 0; y < BOARD_HEIGHT; y++ )
    {
        for ( x = 0; x < BOARD_WIDTH; x++ )
        {
            current = GET_CELL(y, x, board -> r0c0);
            if ( current == CELL_MOVING )
            {
                switch ( board -> stats -> piece.current )
                {
                    case TETROMINO_I:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_I]));
                        break;

                    case TETROMINO_J:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_J]));
                        break;

                    case TETROMINO_L:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_L]));
                        break;

                    case TETROMINO_O:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_O]));
                        break;

                    case TETROMINO_S:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_S]));
                        break;

                    case TETROMINO_T:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_T]));
                        break;

                    case TETROMINO_Z:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_Z]));
                        break;

                    default:
                        break;
                }
            }

            else
            {
                switch ( current )
                {
                    case CELL_I:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_I]));
                        break;

                    case CELL_J:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_J]));
                        break;

                    case CELL_L:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_L]));
                        break;

                    case CELL_O:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_O]));
                        break;

                    case CELL_S:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_S]));
                        break;

                    case CELL_T:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_T]));
                        break;

                    case CELL_Z:
                        drawBlock(x, y, &(board -> gridBox[TETROMINO_Z]));
                        break;

                    default:
                        break;
                }
            }

        }
    }
}

static void
drawStats (screenStats_t * stats)
{
    drawBox(&(stats -> scoreBox));
    drawBox(&(stats -> levelBox));
    drawBox(&(stats -> nextPieceBox));
    drawBox(&(stats -> piecesBox));
}

static void
drawBlock (int x, int y, gbox_t * block)
{
    al_draw_filled_rounded_rectangle(block -> corner.x + x * CELL_WIDTH,
                                     block -> corner.y + y * CELL_HEIGHT,
                                     block -> corner.x + x * CELL_WIDTH + \
                                                            block -> width,
                                     block -> corner.y + y * CELL_HEIGHT + \
                                                            block -> height,
                                     ROUND_X, ROUND_Y,
                                     al_color_html(block -> color.bkgnd));

    al_draw_rounded_rectangle(block -> corner.x + x * CELL_WIDTH,
                              block -> corner.y + y * CELL_HEIGHT,
                              block -> corner.x + x * CELL_WIDTH + \
                                                            block -> width,
                              block -> corner.y + y * CELL_HEIGHT + \
                                                            block -> height,
                              ROUND_X, ROUND_Y,
                              al_color_html(block -> color.border),
                              (block -> thickness));
}

static void
drawBox (gbox_t * box)
{
    if ( strcmp(box -> color.bkgnd, "#FFFFFF") )
    {
        al_draw_filled_rounded_rectangle(box -> corner.x, box -> corner.y,
                                         box -> width, box -> height,
                                         ROUND_X, ROUND_Y,
                                         al_color_html(box -> color.bkgnd));
    }

    al_draw_rounded_rectangle((box -> corner.x), (box -> corner.y),
                              (box -> width + box -> corner.x),
                              (box -> height + box -> corner.y),
                              ROUND_X, ROUND_Y,
                              al_color_html(box -> color.border),
                              (box -> thickness));
}

static void
destroy (game_t * game)
{
    al_destroy_timer(game -> timer.main);
    al_destroy_event_queue(game -> evq);
    al_destroy_bitmap(game -> bkgnd);
    al_destroy_display(game -> display);
}

static void
manageEvents (game_t * game, board_t * boardLogic)
{
    ALLEGRO_EVENT event;

    static unsigned char key[ALLEGRO_KEY_MAX];
    static char key_init = false;

    if ( key_init == false )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    if ( al_get_next_event(game -> evq, &event) )
    {
        switch ( event.type )
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game -> exit = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_TIMER:
                if ( event.timer.source == game -> timer.main )
                {
                    checkKeys(key, game, boardLogic);
                }

                else if ( event.timer.source == game -> timer.piece &&
                          game -> status != PAUSED )
                {
                    boardLogic -> update();

                    game -> redraw = true;

                    if ( game -> status == NEW )
                    {
                        game -> status = PLAYING;
                    }
                }
                break;

            default:
                break;
        }
    }
}

static void
init_scrBoard (screenBoard_t * board, board_t * logic)
{
    int i;

    board -> boardBox.color.bkgnd = "#FFFFFF";
    board -> boardBox.color.border = "#F0F0F0";

    board -> boardBox.thickness = BOX_THIKNESS;

    board -> boardBox.height = CELL_HEIGHT * BOARD_HEIGHT;
    board -> boardBox.width = CELL_WIDTH * BOARD_WIDTH;

    board -> boardBox.corner.x = (SCREEN_WIDTH / 2) - \
                                               ((board -> boardBox.width) / 2);
    board -> boardBox.corner.y = (SCREEN_HEIGHT - BOX_OFFSET) - \
                                              ((board -> boardBox.height));

    // Initialize Tetrominos
    for ( i = TETROMINO_I; i < TETROMINOS; i++ )
    {
        board -> gridBox[i].color.bkgnd = tetrominos_bkgnd[i];
        board -> gridBox[i].color.border = tetrominos_bkgnd[TETROMINOS];

        board -> gridBox[i].thickness = CELL_THIKNESS;

        board -> gridBox[i].height = CELL_HEIGHT;
        board -> gridBox[i].width = CELL_WIDTH;

        board -> gridBox[i].corner.x = board -> boardBox.corner.x;
        board -> gridBox[i].corner.y = board -> boardBox.corner.y;
    }

    board -> r0c0 = logic -> ask.board();

    board -> stats = (stats_t*) logic -> ask.stats();
}

static void
init_scrStats (screenStats_t * stats, board_t * logic)
{
    // Level box
    stats -> levelBox.color.bkgnd = "#FFFFFF";
    stats -> levelBox.color.border = "#F0F0F0";

    stats -> levelBox.thickness = BOX_THIKNESS;

    stats -> levelBox.height = TXT_SIZE + BOX_IN_OFFSET * 2;
    stats -> levelBox.width = CELL_WIDTH * BOARD_WIDTH;

    stats -> levelBox.corner.x = (SCREEN_WIDTH / 2) - \
                                               ((stats -> levelBox.width) / 2);
    stats -> levelBox.corner.y = BOX_OFFSET;

    // Score box
    stats -> scoreBox.color.bkgnd = "#FFFFFF";
    stats -> scoreBox.color.border = "#F0F0F0";

    stats -> scoreBox.thickness = BOX_THIKNESS;

    stats -> scoreBox.height = TXT_SIZE * 2 + BOX_IN_OFFSET * 3;
    stats -> scoreBox.width = TXT_SIZE * 6 + BOX_IN_OFFSET * 2;

    stats -> scoreBox.corner.x = SCREEN_WIDTH - BOX_OFFSET - \
                                (stats -> scoreBox.width);
    stats -> scoreBox.corner.y = BOX_OFFSET;


    // Next Piece box
    stats -> nextPieceBox.color.bkgnd = "#FFFFFF";
    stats -> nextPieceBox.color.border = "#F0F0F0";

    stats -> nextPieceBox.thickness = BOX_THIKNESS;

    stats -> nextPieceBox.height = CELL_HEIGHT * 4 + 2 * BOX_IN_OFFSET;
    stats -> nextPieceBox.width = stats -> scoreBox.width;

    stats -> nextPieceBox.corner.x = stats -> scoreBox.corner.x;
    stats -> nextPieceBox.corner.y = stats -> scoreBox.height + 5 * BOX_OFFSET;

    // Pieces box
    stats -> piecesBox.color.bkgnd = "#FFFFFF";
    stats -> piecesBox.color.border = "#F0F0F0";

    stats -> piecesBox.thickness = BOX_THIKNESS;

    stats -> piecesBox.height = (TETROMINOS - 1) * 2 * CELL_HEIGHT + \
                                CELL_WIDTH + TETROMINOS * 2 * BOX_IN_OFFSET;
    stats -> piecesBox.width = 3 * BOX_IN_OFFSET + 4 * CELL_WIDTH + \
                                TXT_SIZE * 3;

    stats -> piecesBox.corner.x = BOX_OFFSET;
    stats -> piecesBox.corner.y = BOX_OFFSET;

    stats -> stats = (stats_t*) logic -> ask.stats();
}
