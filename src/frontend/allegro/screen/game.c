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
#include <string.h>     // For strcmp and strlen

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
#include "../../../backend/board/timer/boardTimer.h"

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
#define BOX_BORDER_COLOR    "#F0F0F0"
#define BOX_BKGND_COLOR     "#FFFFFF"

#define PAUSE_BKGND_COLOR   "#1E1E60"
#define PAUSE_BORDER_COLOR  "#C5C5E1"
#define PAUSE_TXT_COLOR     "#BA3020"
#define PAUSE_TXT_SIZE      48
#define PAUSE_BOX_THICKNESS 5.0
#define PAUSE_TXT_FONT_PATH \
                          "res/fonts/pixel-operator/PixelOperatorSC.ttf"

#define SEL_BKGND_COLOR     "#1E1E60"
#define SEL_BORDER_COLOR    "#C5C5E1"
#define SEL_TXT_COLOR       "#FFFF00"
#define SEL_TXT_SIZE        52
#define SEL_BOX_THICKNESS   5.0
#define SEL_TXT_FONT_PATH   \
                         "res/fonts/pixel-operator/PixelOperatorSC-Bold.ttf"


#define TXT_SIZE            32
#define TXT_SIZE_BOLD       32
#define TXT_COLOR           "#FFFFFF"

#define MS2S(t)             ((t) / 1000.0)

#define BOX_THICKNESS       3.0
#define CELL_THICKNESS      2.0

#define KEY_READY            ( (KEY_SEEN | KEY_RELEASED) & KEY_RELEASED)

#define TXT_FONT_PATH       \
                          "res/fonts/pixel-operator/PixelOperatorMono.ttf"
#define TXT_FONT_BOLD_PATH  \
                          "res/fonts/pixel-operator/PixelOperatorMono-Bold.ttf"
#define TXT_OFFSET          10

#define DRAWSTATS(s)        drawStats_ ## s ## _f (&screenStats)

// === Enumerations, structures and typedefs ===

enum game_status
{
    NEW = 0,
    PLAYING,
    PAUSED
};

enum wordsarr
{
    STATS,
    LVL,
    ASCORE,
    TSCORE,
    NP,
    LINESC,
    GOVER,
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
    bool restart;

    board_t logic;

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

    struct
    {
        ALLEGRO_FONT * bold;
        ALLEGRO_FONT * regular;
        unsigned int bsize;
        unsigned int csize;
        const char * color;
    } text;

} gbox_t;

typedef struct
{
    gbox_t menuBox;

    struct
    {
        gbox_t box;

        int n;
    } selected;

    int nWords;
} pause_t;

typedef struct
{
    const stats_t * stats;

    gbox_t boardBox;
    gbox_t gridBox[TETROMINOS];

    grid_t * r0c0;
} screenBoard_t;

typedef struct
{
    const stats_t * gStats;

    gbox_t scoreBox;

    gbox_t levelBox;

    gbox_t linesBox;

    gbox_t nextPieceBox;

    gbox_t piecesBox;

} screenStats_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX],
           game_t * game, board_t * logic, pause_t * menu);

static void
clearKey (int keyName, unsigned char * keyArr);

static void
drawBox (gbox_t * box);

static void
drawGameBoard (screenBoard_t * board);

static void
drawBlock (int x, int y, gbox_t * block);

static void
drawStats_level_f (screenStats_t * stats);

static void
drawStats_lines_f (screenStats_t * stats);

static void
drawStats_nextPiece_f (screenStats_t * stats);

static void
drawStats_pieces_f (screenStats_t * stats);

static void
drawStats_score_f (screenStats_t * stats);

static void
drawScreen (game_t * game);

static void
drawPause (pause_t * pstru);

static void
drawPiece (float x, float y, int piece);

static void
destroy (game_t * game);

static void
init_pause (pause_t * pstru);

static void
init_scrBoard (screenBoard_t * board, board_t * logic);

static void
init_scrStats (screenStats_t * stats, board_t * logic);

static void
manageEvents (game_t * game, board_t * boardLogic, pause_t * menu);

static void
restartGame (game_t * game, screenBoard_t * board, screenStats_t * stats);

static void
restartPiecesTimer (game_t * game);

static void
validKey (int keyName,
          unsigned char * keyArr, unsigned char * counterArr,
          game_t * game);

// === ROM Constant variables with file level scope ===
static const char * tetrominos_bkgnd[TETROMINOS + 1] = {
    "#00A2E8", "#3F48CC", "#FF7F27", "#FFC90E",
    "#22B14C", "#A349A4", "#ED1C24",
    "#F0F0F0"
};

static const char * words[] = {
    "Statistics", "Level", "Actual Score",
    "Top Score", "Next Piece", "Lines Cleared",
    "GAME ENDED", NULL
};

static const char * menuWords[] = {
    "Resume", "Restart", "Options", "Exit", NULL
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
    pause_t pMenu;

    // Verify if everything is in order
    if ( alStru == NULL )
    {
        return AL_ERROR;
    }

    if ( al_is_keyboard_installed() == false )
    {
        return AL_ERROR;
    }

    game.display = alStru -> screen.display;
    game.bkgnd = NULL;
    game.evq = NULL;
    game.timer.piece_speed = 1000.0;
    game.timer.piece = NULL;
    game.timer.main = NULL;
    game.status = NEW;
    game.exit = false;
    game.redraw = true;
    game.restart = false;

    // Init game logic
    board_init(&(game.logic));

    if ( game.logic.init == false )
    {
        fputs("Error initializing boardLogic.", stderr);
        return AL_ERROR;
    }

    init_scrBoard(&screenBoard, &(game.logic));
    init_scrStats(&screenStats, &(game.logic));
    init_pause(&pMenu);

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

    initTimer(screenStats.gStats);
    game.timer.piece_speed = askTimeLimit();
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

    int filled, lines[BOARD_HEIGHT];
    // Draw and display window
    while ( game.exit == false )
    {
        // Draw game screen
        drawScreen(&game);

        // Draw board
        drawGameBoard(&screenBoard);

        // Draw Stats
        DRAWSTATS(pieces);
        DRAWSTATS(level);
        DRAWSTATS(score);
        DRAWSTATS(nextPiece);
        DRAWSTATS(lines);

        if ( game.status == PAUSED )
        {
            drawPause(&pMenu);
        }

        al_flip_display();

        game.redraw = false;

        if ( game.restart == true )
        {
            // Restart
            restartGame(&game, &screenBoard, &screenStats);
            pMenu.selected.n = 0;
        }

            // Check if a row has been filled
        else if ( (filled = game.logic.ask.filledRows(lines)) > 0 )
        {
            int i;
            for ( i = 0; i < filled; i++ )
            {
                game.logic.clear.line(lines, i);
            }
            game.redraw = true;

            restartPiecesTimer(&game);
        }

            // Check End Game
        else if ( game.logic.ask.endGame() )
        {
            al_stop_timer(game.timer.piece);

            // End game presentation goes here
            game.exit = true;
        }

        // Game goes on
        while ( game.redraw == false && game.exit == false )
        {
            manageEvents(&game, &(game.logic), &pMenu);
        }
    }

    destroy(&game);

    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

static void
restartGame (game_t * game, screenBoard_t * board, screenStats_t * stats)
{
    game -> logic.destroy();

    // Init game logic
    board_init(&(game -> logic));

    if ( game -> logic.init == false )
    {
        fputs("Error initializing boardLogic.", stderr);
        game -> exit = true;
    }

    init_scrBoard(board, &(game -> logic));
    init_scrStats(stats, &(game -> logic));

    game -> redraw = true;
    game -> restart = false;
    game -> status = NEW;

    restartPiecesTimer(game);
}

static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX],
           game_t * game, board_t * logic, pause_t * menu)
{
    // Counters
    int i, j;
    // Used to check if a key is in "Play" and "Pause" modes
    bool bothModes = false;

    // Counter to make pieces less sensitive
    static unsigned char counter[ALLEGRO_KEY_MAX];
    // Assure that the counter has been started
    static bool key_init = false;

    // Keys used in "Play Mode"
    const unsigned char playKeys[] = {
        ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
        ALLEGRO_KEY_X, ALLEGRO_KEY_Z,
        ALLEGRO_KEY_ENTER, ALLEGRO_KEY_Q
    };

    // Keys used in "Pause Mode"
    const unsigned char pauseKeys[] = {
        ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN,
        ALLEGRO_KEY_ENTER, ALLEGRO_KEY_Q
    };

    // Start the counter the first game running
    if ( key_init == false )
    {
        memset(counter, 0, sizeof (counter));
        key_init = true;
    }

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

    // Keys common to both modes
    // Enter (for pause and menu selection)
    if ( key[ALLEGRO_KEY_ENTER] == KEY_READY )
    {
        validKey(ALLEGRO_KEY_ENTER, key, counter, game);

        switch ( game -> status )
        {
            case NEW:
            case PLAYING:
                game -> status = PAUSED;

                break;

            case PAUSED:
                switch ( menu -> selected.n )
                {
                    case 0:
                        game -> status = PLAYING;
                        break;

                    case 1:
                        game -> restart = true;
                        break;

                    case 2:
                        break;

                    case 3:
                        game -> exit = true;
                        break;

                    default:
                        break;
                }

        }
    }

    // Quit game
    if ( key[ALLEGRO_KEY_Q] == KEY_READY )
    {
        validKey(ALLEGRO_KEY_Q, key, counter, game);

        game -> exit = true;
    }

    // Key management
    switch ( game -> status )
    {
            // Play mode
        case PLAYING:
            if ( key[ALLEGRO_KEY_UP] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_UP, key, counter, game);

                // Rotate piece right
                logic -> piece.rotate(RIGHT);
            }

            if ( (key[ALLEGRO_KEY_DOWN]) && (counter[ALLEGRO_KEY_DOWN] == 0) )
            {
                validKey(ALLEGRO_KEY_DOWN, key, counter, game);

                // Move down one position in the game
                logic -> piece.softDrop();
            }

            if ( key[ALLEGRO_KEY_LEFT] && (counter[ALLEGRO_KEY_LEFT] == 0) )
            {
                validKey(ALLEGRO_KEY_LEFT, key, counter, game);

                // Move up left position in the game
                logic -> piece.shift(LEFT);
            }

            if ( key[ALLEGRO_KEY_RIGHT] && (counter[ALLEGRO_KEY_RIGHT] == 0) )
            {
                validKey(ALLEGRO_KEY_RIGHT, key, counter, game);

                // Move right one position in the game
                logic -> piece.shift(RIGHT);
            }

            if ( key[ALLEGRO_KEY_X] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_X, key, counter, game);

                // Rotate piece right
                logic -> piece.rotate(RIGHT);
            }

            if ( key[ALLEGRO_KEY_Z] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_Z, key, counter, game);

                // Rotate piece left
                logic -> piece.rotate(LEFT);
            }

            // Clear unnecesary keys that are used only in "pause mode"

            for ( i = 0; i < (sizeof (playKeys) / sizeof (char)); i++ )
            {
                for ( j = 0; (j < (sizeof (pauseKeys) / sizeof (char))) &&
                      (bothModes == false); j++ )
                {
                    (playKeys[i] == pauseKeys[j]) ? (bothModes = true) : 0;
                }

                (bothModes == false) ? clearKey(playKeys[i], key) : (bothModes = false);
            }

            break;

            // Pause mode
        case PAUSED:

            if ( key[ALLEGRO_KEY_UP] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_UP, key, counter, game);

                (menu -> selected.n == 0) ? \
            (menu -> selected.n = menu -> nWords - 1) : (menu -> selected.n)--;

            }

            if ( key[ALLEGRO_KEY_DOWN] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_DOWN, key, counter, game);

                (menu -> selected.n == menu -> nWords - 1) ? \
                    (menu -> selected.n = 0) : (menu -> selected.n)++;
            }

            // Clear unnecesary keys used only in "play mode"
            for ( i = 0; i < (sizeof (playKeys) / sizeof (char)); i++ )
            {
                for ( j = 0; (j < (sizeof (pauseKeys) / sizeof (char))) &&
                      (bothModes == false); j++ )
                {
                    (playKeys[i] == pauseKeys[j]) ? (bothModes = true) : 0;
                }

                (bothModes == false) ? clearKey(playKeys[i], key) : (bothModes = false);
            }
            break;

        default:
            break;
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
drawStats_pieces_f (screenStats_t * stats)
{
    int i;
    float coord[COORD_NUM] = {};

    // == Draw pieces box ==
    // Information
    for ( i = TETROMINO_I; i < TETROMINOS; i++ )
    {
        coord[COORD_X] = stats -> piecesBox.corner.x + TXT_OFFSET;
        coord[COORD_Y] = stats->piecesBox.corner.y + \
                                     i * TXT_OFFSET + 2.5 * i * CELL_HEIGHT + \
                  1.5 * al_get_font_line_height(stats->piecesBox.text.regular);

        drawPiece(coord[COORD_X], coord[COORD_Y], i);

        coord[COORD_X] += CELL_WIDTH * 4 + TXT_OFFSET;

        al_draw_textf(stats->piecesBox.text.regular,
                      al_color_html(stats -> piecesBox.text.color),
                      coord[COORD_X],
                      coord[COORD_Y] + 0.25 * CELL_HEIGHT,
                      ALLEGRO_ALIGN_LEFT,
                      "%06d", stats -> gStats -> piece.number[i]);
    }

    // Size
    stats -> piecesBox.width = coord[COORD_X] + \
                    al_get_text_width(stats->piecesBox.text.regular, "000000");
    stats -> piecesBox.height = SCREEN_HEIGHT - BOX_OFFSET - \
                                                   stats -> piecesBox.corner.y;

    // Box title
    al_draw_textf(stats->piecesBox.text.bold,
                  al_color_html(stats -> piecesBox.text.color),
                  stats -> piecesBox.corner.x + stats->piecesBox.width / 2,
                  stats -> piecesBox.corner.y + TXT_OFFSET,
                  ALLEGRO_ALIGN_CENTRE,
                  words[STATS]);

    // Draw box
    drawBox(&(stats -> piecesBox));
}

static void
drawStats_level_f (screenStats_t * stats)
{
    char str[strlen(words[LVL]) + 7];

    strcpy(str, words[LVL]);
    strcat(str, " %03d");

    // Draw level information in its box
    al_draw_textf(stats->levelBox.text.bold,
                  al_color_html(stats -> levelBox.text.color),
                  (stats->levelBox.width / 2) + stats -> levelBox.corner.x,
                  (stats->levelBox.height / 2) - \
                (al_get_font_line_height(stats->levelBox.text.regular) / 2) + \
                  stats->levelBox.corner.y,
                  ALLEGRO_ALIGN_CENTRE,
                  str, stats -> gStats -> level);

    drawBox(&(stats -> levelBox));
}

static void
drawStats_score_f (screenStats_t * stats)
{
    float fontHeight = al_get_font_line_height(stats->scoreBox.text.regular);
    float fontHeight_B = al_get_font_line_height(stats->scoreBox.text.bold);
    float coord[COORD_NUM] = {};

    // == Draw score box ==
    // Actual score
    coord[COORD_X] = (stats->scoreBox.width / 2) + stats -> scoreBox.corner.x;
    coord[COORD_Y] = stats->scoreBox.corner.y + TXT_OFFSET;

    al_draw_text(stats->scoreBox.text.bold,
                 al_color_html(stats -> scoreBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[ASCORE]);

    coord[COORD_Y] += TXT_OFFSET + fontHeight_B;

    al_draw_textf(stats->scoreBox.text.regular,
                  al_color_html(stats -> scoreBox.text.color),
                  coord[COORD_X], coord[COORD_Y],
                  ALLEGRO_ALIGN_LEFT,
                  "%06d", stats -> gStats -> score.actual);

    // Top score
    coord[COORD_Y] += TXT_OFFSET + fontHeight;

    al_draw_text(stats->scoreBox.text.bold,
                 al_color_html(stats -> scoreBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[TSCORE]);

    coord[COORD_Y] += TXT_OFFSET + fontHeight_B;

    al_draw_textf(stats->scoreBox.text.regular,
                  al_color_html(stats -> scoreBox.text.color),
                  coord[COORD_X], coord[COORD_Y],
                  ALLEGRO_ALIGN_LEFT,
                  "%06d", stats -> gStats -> score.top);

    // Size
    stats -> scoreBox.height = coord[COORD_Y] + fontHeight;

    // Draw box
    drawBox(&(stats -> scoreBox));

}

static void
drawStats_nextPiece_f (screenStats_t * stats)
{
    float fontHeight_B = al_get_font_line_height(stats->nextPieceBox.text.bold);
    float coord[COORD_NUM] = {};

    // == Next Piece box ==
    stats -> nextPieceBox.corner.x = stats -> scoreBox.corner.x;
    stats -> nextPieceBox.corner.y = stats -> scoreBox.height + 2 * BOX_OFFSET;

    stats -> nextPieceBox.width = stats -> scoreBox.width;

    coord[COORD_X] = stats->nextPieceBox.width / 2 + \
                                                stats -> nextPieceBox.corner.x;
    coord[COORD_Y] = stats->nextPieceBox.corner.y + TXT_OFFSET;
    // Draw next piece information in its box
    al_draw_text(stats->nextPieceBox.text.bold,
                 al_color_html(stats -> nextPieceBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[NP]);

    if ( stats->gStats->piece.next != TETROMINO_NONE )
    {
        coord[COORD_X] = (stats->nextPieceBox.width / 2) + \
                               stats -> nextPieceBox.corner.x - CELL_WIDTH * 2;
        coord[COORD_Y] = stats->nextPieceBox.corner.y + 2.5 * TXT_OFFSET + \
                            fontHeight_B;

        drawPiece(coord[COORD_X], coord[COORD_Y], stats->gStats->piece.next);
    }

    stats -> nextPieceBox.height = 2 * CELL_HEIGHT + fontHeight_B + \
                                                                4 * TXT_OFFSET;
    drawBox(&(stats -> nextPieceBox));
}

static void
drawStats_lines_f (screenStats_t * stats)
{
    float fontHeight_B = al_get_font_line_height(stats->linesBox.text.bold);

    stats -> linesBox.corner.x = stats -> scoreBox.corner.x;
    stats -> linesBox.corner.y = stats -> nextPieceBox.corner.y + \
                                 stats -> nextPieceBox.height + BOX_OFFSET;

    stats -> linesBox.width = stats -> scoreBox.width;
    stats -> linesBox.height = 2 * fontHeight_B + 3 * TXT_OFFSET;

    // Draw level information in its box
    al_draw_text(stats -> linesBox.text.bold,
                 al_color_html(stats -> linesBox.text.color),
                 stats -> linesBox.width / 2 + stats -> linesBox.corner.x,
                 stats -> linesBox.corner.y + TXT_OFFSET,
                 ALLEGRO_ALIGN_CENTRE, words[LINESC]);

    al_draw_textf(stats->linesBox.text.regular,
                  al_color_html(stats -> linesBox.text.color),
                  stats->linesBox.width / 2 + stats -> linesBox.corner.x,
                  fontHeight_B + 2 * TXT_OFFSET + stats->linesBox.corner.y,
                  ALLEGRO_ALIGN_CENTRE,
                  "%03d", stats -> gStats -> lines.cleared);

    drawBox(&(stats -> linesBox));
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
        al_draw_filled_rounded_rectangle((box -> corner.x), (box -> corner.y),
                                         (box -> width + box -> corner.x),
                                         (box -> height + box -> corner.y),
                                         ROUND_X, ROUND_Y,
                                         al_color_html(box -> color.border));
    }

    al_draw_rounded_rectangle((box -> corner.x), (box -> corner.y),
                              (box -> width + box -> corner.x),
                              (box -> height + box -> corner.y),
                              ROUND_X, ROUND_Y,
                              al_color_html(box -> color.border),
                              (box -> thickness));
}

static void
drawPiece (float x, float y, int piece)
{
    // x , y are (0,0) -top left corner- of a 4x8 CELL_HEIGHT/2 rectangle
    int i, j;
    int block[BLOCKS][COORD_NUM] = {};
    float pieceGrid[BLOCKS][2 * BLOCKS ][COORD_NUM] = {};
    float modCoord[COORD_NUM] = {x, y};

    for ( i = 0; i < BLOCKS; i++ )
    {
        for ( j = 0; j < (2 * BLOCKS); j++ )
        {
            pieceGrid[i][j][COORD_X] = modCoord[COORD_X];
            pieceGrid[i][j][COORD_Y] = modCoord[COORD_Y];

            modCoord[COORD_X] += (CELL_WIDTH / 2);
        }
        modCoord[COORD_X] = x;
        modCoord[COORD_Y] += (CELL_HEIGHT / 2);
    }

    switch ( piece )
    {
        case TETROMINO_I:
            for ( i = b1; i < BLOCKS; i++ )
            {
                block[i][COORD_X] = pieceGrid[1][i * 2][COORD_X];
                block[i][COORD_Y] = pieceGrid[1][i * 2][COORD_Y];
            }

            break;

        case TETROMINO_J:
            j = 1;

            block[b4][COORD_X] = pieceGrid[0][j][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j][COORD_Y];

            for ( i = b1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }
            break;

        case TETROMINO_L:
            for ( i = b1, j = 1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            block[b4][COORD_X] = pieceGrid[0][j - 2][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j - 2][COORD_Y];
            break;

        case TETROMINO_O:
            for ( i = b1, j = 2; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            for ( i = (BLOCKS / 2), j = 2; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            break;

        case TETROMINO_S:
            for ( i = b1, j = 3; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            for ( i = (BLOCKS / 2), j = 1; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            break;

        case TETROMINO_T:
            for ( i = b1, j = 1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            block[b4][COORD_X] = pieceGrid[0][j - 4][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j - 4][COORD_Y];
            break;

        case TETROMINO_Z:
            for ( i = b1, j = 1; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            for ( i = (BLOCKS / 2), j = 3; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }
            break;

        default:
            break;
    }

    for ( i = 0; i < BLOCKS; i++ )
    {
        al_draw_filled_rounded_rectangle(block[i][COORD_X],
                                         block[i][COORD_Y],
                                         block[i][COORD_X] + CELL_WIDTH,
                                         block[i][COORD_Y] + CELL_HEIGHT,
                                         ROUND_X, ROUND_Y,
                                         al_color_html(tetrominos_bkgnd[piece]));

        al_draw_rounded_rectangle(block[i][COORD_X],
                                  block[i][COORD_Y],
                                  block[i][COORD_X] + CELL_WIDTH,
                                  block[i][COORD_Y] + CELL_HEIGHT,
                                  ROUND_X, ROUND_Y,
                                  al_color_html(tetrominos_bkgnd[TETROMINOS]),
                                  CELL_THICKNESS);
    }
}

static void
destroy (game_t * game)
{
    game -> logic.destroy();
    al_destroy_timer(game -> timer.main);
    al_destroy_event_queue(game -> evq);
    al_destroy_bitmap(game -> bkgnd);
}

static void
init_scrBoard (screenBoard_t * board, board_t * logic)
{
    int i;

    board -> boardBox.color.bkgnd = BOX_BKGND_COLOR;
    board -> boardBox.color.border = BOX_BORDER_COLOR;

    board -> boardBox.thickness = BOX_THICKNESS;

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

        board -> gridBox[i].thickness = CELL_THICKNESS;

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
    // Stats structure
    stats -> gStats = (stats_t*) logic -> ask.stats();

    // Level box
    stats -> levelBox.height = TXT_SIZE + BOX_IN_OFFSET * 2;
    stats -> levelBox.width = CELL_WIDTH * BOARD_WIDTH;

    stats -> levelBox.corner.x = (SCREEN_WIDTH / 2) - \
                                               ((stats -> levelBox.width) / 2);
    stats -> levelBox.corner.y = BOX_OFFSET;

    // Lines box
    stats -> nextPieceBox.height = 0;
    stats -> nextPieceBox.width = 0;

    stats -> nextPieceBox.corner.x = 0;
    stats -> nextPieceBox.corner.y = 0;

    // Score box
    stats -> scoreBox.corner.x = SCREEN_WIDTH / 2 + BOX_OFFSET + \
                                                                5 * CELL_WIDTH;
    stats -> scoreBox.corner.y = BOX_OFFSET;

    stats -> scoreBox.height = 0;
    stats -> scoreBox.width = SCREEN_WIDTH - BOX_OFFSET - \
                                                    stats -> scoreBox.corner.x;

    // Next Piece box
    stats -> nextPieceBox.height = 0;
    stats -> nextPieceBox.width = 0;

    stats -> nextPieceBox.corner.x = 0;
    stats -> nextPieceBox.corner.y = 0;

    // Pieces box
    stats -> piecesBox.corner.x = BOX_OFFSET;
    stats -> piecesBox.corner.y = BOX_OFFSET;

    // Common to all
    stats -> levelBox.color.bkgnd = stats -> scoreBox.color.bkgnd = \
            stats -> nextPieceBox.color.bkgnd = \
            stats -> piecesBox.color.bkgnd = \
            stats -> linesBox.color.bkgnd = BOX_BKGND_COLOR;

    stats -> levelBox.color.border = stats -> scoreBox.color.border = \
            stats -> nextPieceBox.color.border = \
            stats -> linesBox.color.border = \
            stats -> piecesBox.color.border = BOX_BORDER_COLOR;

    stats -> levelBox.thickness = stats -> scoreBox.thickness = \
            stats -> nextPieceBox.thickness = \
            stats -> linesBox.thickness = \
            stats -> piecesBox.thickness = BOX_THICKNESS;

    stats -> levelBox.text.color = stats -> scoreBox.text.color = \
            stats -> nextPieceBox.text.color = \
            stats -> linesBox.text.color = \
            stats -> piecesBox.text.color = TXT_COLOR;

    stats -> levelBox.text.csize = stats -> scoreBox.text.csize = \
            stats -> nextPieceBox.text.csize = \
            stats -> linesBox.text.csize = \
            stats -> piecesBox.text.csize = TXT_SIZE;

    stats -> levelBox.text.bsize = stats -> scoreBox.text.bsize = \
            stats -> nextPieceBox.text.bsize = \
            stats -> linesBox.text.bsize = \
            stats -> piecesBox.text.bsize = TXT_SIZE_BOLD;

    stats -> levelBox.text.regular = stats -> scoreBox.text.regular = \
            stats -> nextPieceBox.text.regular = \
            stats -> piecesBox.text.regular = \
            stats -> linesBox.text.regular = \
            al_load_font(TXT_FONT_PATH, stats -> levelBox.text.csize, 0);

    stats -> levelBox.text.bold = stats -> scoreBox.text.bold = \
            stats -> nextPieceBox.text.bold = \
            stats -> piecesBox.text.bold = \
            stats -> linesBox.text.bold = \
            al_load_font(TXT_FONT_BOLD_PATH, stats -> levelBox.text.bsize, 0);
}

static void
manageEvents (game_t * game, board_t * boardLogic, pause_t * menu)
{
    ALLEGRO_EVENT event;

    static unsigned char key[ALLEGRO_KEY_MAX];
    static bool key_init = false;

    if ( key_init == false )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    al_wait_for_event(game -> evq, &event);

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
                checkKeys(key, game, boardLogic, menu);

            }

            else if ( event.timer.source == game -> timer.piece &&
                      game -> status != PAUSED )
            {
                if ( game -> status == NEW )
                {
                    game -> status = PLAYING;
                }

                boardLogic -> update();

                game -> redraw = true;
            }
            break;

        default:
            break;
    }

}

static void
validKey (int keyName,
          unsigned char * keyArr, unsigned char * counterArr,
          game_t * game)
{
    clearKey(keyName, keyArr);
    counterArr[keyName]++;
    game -> redraw = true;
}

static void
clearKey (int keyName, unsigned char * keyArr)
{
    keyArr[keyName] &= KEY_SEEN;
}

static void
restartPiecesTimer (game_t * game)
{
    game -> timer.piece_speed = askTimeLimit();

    // Restart pieces timer
    al_stop_timer(game -> timer.piece);

    al_set_timer_speed(game -> timer.piece, MS2S(game -> timer.piece_speed));

    al_start_timer(game -> timer.piece);
}

static void
drawPause (pause_t * pstru)
{
    int i;
    float fontHeight_nSel = al_get_font_line_height(pstru->menuBox.text.regular);

    drawBox(&(pstru -> menuBox));

    for ( i = 0; i < pstru->nWords; i++ )
    {
        if ( (pstru->selected.n) == i )
        {
            al_draw_text(pstru -> selected.box.text.regular,
                         al_color_html(pstru -> selected.box.text.color),
                         pstru -> menuBox.corner.x + \
                                                 pstru->menuBox.width / 2,
                         pstru -> menuBox.corner.y + (1 + i) * TXT_OFFSET + \
                    (i * fontHeight_nSel),
                         ALLEGRO_ALIGN_CENTRE, menuWords[i]);
        }

        else
        {
            al_draw_text(pstru -> menuBox.text.regular,
                         al_color_html(pstru -> menuBox.text.color),
                         pstru -> menuBox.corner.x + pstru->menuBox.width / 2,
                         pstru -> menuBox.corner.y + (1 + i) * TXT_OFFSET + \
                    (i * fontHeight_nSel),
                         ALLEGRO_ALIGN_CENTRE, menuWords[i]);
        }
    }
}

static void
init_pause (pause_t * pstru)
{
    float fontHeight_Sel, fontHeight_nSel;

    int sWidth = 0, nWidth = 0;

    int biggerWord = 0, bWordSize = 0;
    int tmp = 0;

    pstru -> menuBox.color.bkgnd = PAUSE_BKGND_COLOR;
    pstru -> menuBox.color.border = PAUSE_BORDER_COLOR;
    pstru -> menuBox.text.color = PAUSE_TXT_COLOR;
    pstru -> menuBox.text.csize = PAUSE_TXT_SIZE;
    pstru -> menuBox.text.regular = al_load_font(PAUSE_TXT_FONT_PATH,
                                                 pstru -> menuBox.text.csize, 0);

    pstru -> menuBox.thickness = PAUSE_BOX_THICKNESS;

    pstru -> selected.box.color.bkgnd = SEL_BKGND_COLOR;
    pstru -> selected.box.color.border = SEL_BORDER_COLOR;
    pstru -> selected.box.text.color = SEL_TXT_COLOR;
    pstru -> selected.box.text.csize = SEL_TXT_SIZE;
    pstru -> selected.box.text.regular = al_load_font(SEL_TXT_FONT_PATH, \
                                           pstru -> selected.box.text.csize, 0);

    pstru -> selected.box.thickness = SEL_BOX_THICKNESS;

    pstru -> selected.n = 0;

    // Calculate height and width of the box
    fontHeight_Sel = al_get_font_line_height(pstru->selected.box.text.regular);
    fontHeight_nSel = al_get_font_line_height(pstru->menuBox.text.regular);

    pstru -> nWords = 0;
    // Get the bigger word in the array as well as the number of words in it
    while ( menuWords[pstru -> nWords] != NULL )
    {
        tmp = strlen(menuWords[pstru -> nWords]);

        if ( bWordSize < tmp )
        {
            bWordSize = tmp;
            biggerWord = pstru -> nWords;
        }

        (pstru -> nWords)++;
    }

    pstru -> menuBox.height = (1 + pstru -> nWords) * TXT_OFFSET + \
                      fontHeight_Sel + (pstru -> nWords - 1) * fontHeight_nSel;


    sWidth = al_get_text_width(pstru->selected.box.text.regular,
                               menuWords[biggerWord]) + 2 * TXT_OFFSET;
    nWidth = al_get_text_width(pstru->menuBox.text.regular,
                               menuWords[biggerWord]) + 2 * TXT_OFFSET;

    (sWidth > nWidth) ? \
         (pstru -> menuBox.width = sWidth) : (pstru -> menuBox.width = nWidth);

    pstru -> menuBox.corner.x = (SCREEN_WIDTH / 2) - \
                                                  (pstru -> menuBox.width / 2);
    pstru -> menuBox.corner.y = (SCREEN_HEIGHT / 2) - \
                                                 (pstru -> menuBox.height / 2);
}
