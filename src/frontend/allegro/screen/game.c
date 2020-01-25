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
 * @file    game.c
 * 
 * @brief   Play the game using Allegro
 * 
 * @details Runs the entire game using Allegro
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

// For drawing boxes
#include "../primitives.h"

// End Game screen
#include "endGame.h"

// This file
#include "game.h"

// Game stats
#include "gameStats.h"

#include "gamePause.h"


/// @privatesection
// === Constants and Macro definitions ===

/**
 * @def MS2S(t)
 * @brief Convert MS to S
 */
#define MS2S(t)             ((t) / 1000.0)

/**
 * @def DRAWSTATS(s)
 * @brief Call a function with prototype drawStats_(s)_f(&screenStats)
 */
#define DRAWSTATS(s)        drawStats_ ## s ## _f (screenStats)

// Display settings
#define FPS                 60.0
#define GAME_BKGND_COLOR    "#000000"
#define BKGND_WIDTH         1920
#define BKGND_HEIGHT        1280

// Board cell size
#define CELL_WIDTH          25.0
#define CELL_HEIGHT         CELL_WIDTH
#define CELL_THICKNESS      2.0

// Selected option
#define SEL_BKGND_COLOR     "#1E1E60"
#define SEL_BORDER_COLOR    "#C5C5E1"
#define SEL_TXT_COLOR       "#FFFF00"
#define SEL_BOX_THICKNESS   5.0

#define NLEVELS             10
#define NROWS               NLEVELS

#define FX_DROP             "res/audio/fx/blop.wav"
#define FX_LINE             "res/audio/fx/line.wav"
#define FX_GOVER            "res/audio/fx/gameover.wav"
// === Enumerations, structures and typedefs ===

// Game status

enum game_status
{
    // Game is being loaded
    INITIAL,
    // Configuration screen (number of rows and level)
    CONFIG = 0,
    // New game
    NEW,
    // User is playing
    PLAYING,
    // Game is paused
    PAUSED
};

// Meant to use in an array storing the number of initial rows and start level

enum rowsAndLevelSelected
{
    // Number of rows
    ROWS = 0,
    // Initial level
    LEVEL,
    // Number of options
    RPL
};

/**
 * @brief General game information and status
 * 
 * Keeps the display, background and event queue pointers, as well as timers,
 * the current game status and the backend logic.
 */
typedef struct
{
    // Allegro structure with sound control and the original display
    allegro_t * alStru;

    // Display
    ALLEGRO_DISPLAY * display;
    // Background
    ALLEGRO_BITMAP * bkgnd;

    // Event queue
    ALLEGRO_EVENT_QUEUE * evq;

    // Timers

    struct
    {
        // Piece
        ALLEGRO_TIMER * piece;
        // Display refresh and general timer
        ALLEGRO_TIMER * main;

        // Piece droping speed
        float piece_speed;
    } timer;

    // Initial configurations

    struct
    {
        bool ready;

        // Option selected
        unsigned char n[RPL];

        // Row or level option selected
        unsigned char rl;

        gbox_t level;
        gbox_t level_sel;

        gbox_t rows;
        gbox_t rows_sel;

    } initial;

    // Selected option in the game
    int status;

    // Quit game
    bool exit;

    // Redraw display
    bool redraw;

    // Restart game
    bool restart;

    // Game Logic
    board_t logic;

} game_t;

/**
 * @brief Board shown in the display
 */
typedef struct
{
    const stats_t * stats;

    gbox_t boardBox;
    gbox_t gridBox[TETROMINOS];

    grid_t * r0c0;
} screenBoard_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX],
           game_t * game, board_t * logic, pause_t * menu);

static void
clearKey (int keyName, unsigned char * keyArr);

static void
destroy (game_t * game);

static void
drawBlock (int x, int y, gbox_t * block);

static void
drawGameBoard (screenBoard_t * board);

static void
drawInitial (game_t * game);

static void
drawScreen (game_t * game);

static void
gameManagement (game_t * game, screenBoard_t * screenBoard,
                screenStats_t * screenStats, pause_t * pMenu);

static void
init_rowLevel (game_t * game);

static void
init_scrBoard (screenBoard_t * board, board_t * logic);

static void
manageEvents (game_t * game, board_t * boardLogic, pause_t * menu);

static void
playFX (char * path, game_t * game);

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

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Starts the game while running in Allegro mode
 * 
 * @param alStru Higher structure with the display information as well as the
 * sound control
 * 
 * @return AL_OK if everything is fine
 * @return AL_ERROR if something failed
 */
int
alg_game (allegro_t * alStru)
{
    // Structures
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

    // Initialize game structure variables
    game.alStru = alStru;
    // Clear exit flag in case it hasn't been cleared before
    game.alStru -> exit = false;

    game.display = alStru -> screen.display;
    game.bkgnd = NULL;
    game.evq = NULL;
    game.timer.piece_speed = 1000.0;
    game.timer.piece = NULL;
    game.timer.main = NULL;
    game.status = INITIAL;
    game.exit = false;
    game.redraw = true;
    game.restart = false;

    game.initial.ready = false;
    game.initial.n[LEVEL] = 0;
    game.initial.n[ROWS] = 0;
    game.initial.rl = LEVEL;

    // Init game logic
    board_init(&(game.logic));

    if ( game.logic.init == false )
    {
        fputs("Error initializing boardLogic.", stderr);
        return AL_ERROR;
    }

    // Initialize rows and levels
    init_rowLevel(&game);
    // Initialize screen board
    init_scrBoard(&screenBoard, &(game.logic));
    // Initialize screen stats
    init_scrStats(&screenStats, &(game.logic));
    // Initialize pause menu
    init_pause(&pMenu, alStru);

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

    // Start events timers
    al_start_timer(game.timer.main);

    while ( game.initial.ready == false )
    {
        (game.redraw == true) ? \
            (drawInitial(&game)) : \
            (manageEvents(&game, NULL, &pMenu));
    }

    game.logic.set.startRows(game.initial.n[ROWS]);
    game.logic.set.startLevel(game.initial.n[LEVEL]);

    // Start a new game
    game.status = NEW;

    // == Pieces Timer ==
    // Init
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

    // Register event
    al_register_event_source(game.evq,
                             al_get_timer_event_source(game.timer.piece));

    // Start pieces timer
    al_start_timer(game.timer.piece);

    // Draw and display window
    while ( game.exit == false )
    {
        gameManagement(&game, &screenBoard, &screenStats, &pMenu);
    }

    // Stop music
    if ( game.alStru -> samples.music.enabled() &&
         game.alStru -> samples.music.status() )
    {
        (game.alStru -> samples.music.stop());
    }

    // Destroy game structure and exit
    destroy(&game);

    return AL_OK;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Manages in game drawing and actions
 * 
 * @param game Current game structure
 * @param screenBoard Screen board
 * @param screenStats Stats to draw in screen 
 * @param pMenu Pause menu structure
 * 
 * @return Nothing
 */
static void
gameManagement (game_t * game, screenBoard_t * screenBoard,
                screenStats_t * screenStats, pause_t * pMenu)
{
    // Check which line is filled
    int filled, lines[BOARD_HEIGHT];

    // Draw game screen
    drawScreen(game);

    // Draw board
    drawGameBoard(screenBoard);

    // Draw Stats
    DRAWSTATS(pieces);
    DRAWSTATS(level);
    DRAWSTATS(score);

    screenStats -> showNextPiece = pMenu -> get.nextPiece;
    DRAWSTATS(nextPiece);

    DRAWSTATS(lines);

    // Game is paused
    if ( game -> status == PAUSED )
    {
        (pMenu -> draw.beingDrawn == MAINMENU) ?
                (pMenu -> draw.main()) : (pMenu -> draw.options());
    }

    // Play music
    if ( game -> alStru -> samples.music.enabled() &&
         !(game -> alStru -> samples.music.status()) )
    {
        game -> alStru -> samples.music.play();
    }

    else if ( !(game -> alStru -> samples.music.enabled()) &&
              (game -> alStru -> samples.music.status()) )
    {
        game -> alStru -> samples.music.stop();
    }

    // Flip
    al_flip_display();
    game -> redraw = false;

    // Game should be restarted
    if ( game -> restart == true )
    {
        // Restart
        restartGame(game, screenBoard, screenStats);
        pMenu -> selected.n = 0;
    }

        // Check if a row has been filled
    else if ( (filled = game -> logic.ask.filledRows(lines)) > 0 )
    {
        int i;
        // Remove line(s) from board
        for ( i = 0; i < filled; i++ )
        {
            game -> logic.clear.line(lines, i);
        }
        game -> redraw = true;

        // Play a sound FX when a line has been cleared
        playFX(FX_LINE, game);

        // Restart pieces timer
        restartPiecesTimer(game);
    }

        // Check End Game
    else if ( game -> logic.ask.endGame() )
    {
        // Stop timers
        al_stop_timer(game -> timer.piece);

        // Play sound FX
        playFX(FX_GOVER, game);

        // Inform a game over to internal structure but not to allegro_t
        // This way data from this game will be destroyed after showing the
        // end game screen but player will be able to play again
        game -> alStru -> exit = false;
        game -> exit = true;

        // Stop music
        if ( game -> alStru -> samples.music.enabled() &&
             game -> alStru -> samples.music.status() )
        {
            (game -> alStru -> samples.music.stop());
        }

        // Print End Game screen
        alg_endGame(game -> alStru, (stats_t *) game -> logic.ask.stats());

    }

    // Game goes on
    while ( game -> redraw == false && game -> exit == false )
    {
        manageEvents(game, &(game -> logic), pMenu);
    }
}

/**
 * @brief Restart the game and start a new one
 * 
 * @param game Old game structure (now current)
 * @param board Screen board
 * @param stats Screen stats
 */
static void
restartGame (game_t * game, screenBoard_t * board, screenStats_t * stats)
{
    // Destroy old game logic
    game -> logic.destroy();

    // Init game logic
    board_init(&(game -> logic));

    if ( game -> logic.init == false )
    {
        fputs("Error initializing boardLogic.", stderr);
        game -> exit = true;
    }

    // Use the same start rows and level as chosen before
    game -> logic.set.startRows(game -> initial.n[ROWS]);
    game -> logic.set.startLevel(game -> initial.n[LEVEL]);

    // Init the new screen board and stats
    init_scrBoard(board, &(game -> logic));
    init_scrStats(stats, &(game -> logic));

    // Redraw and reset status
    game -> redraw = true;
    game -> restart = false;
    game -> status = NEW;

    // Restart pieces timer
    restartPiecesTimer(game);
}

/**
 * @brief Check if a valid key has been pressed
 * 
 * @param key Array with the keys pressed
 * @param game Current game structure
 * @param logic Game logic
 * @param menu Pause menu structure
 * 
 * @return Nothing
 */
static void
checkKeys (unsigned char key[ALLEGRO_KEY_MAX],
           game_t * game, board_t * logic, pause_t * pMenu)
{
    // Counters
    int i, j;
    // Used to check if a key is in "Play" and "Pause" modes
    bool bothModes = false;

    // Counter to make pieces less sensitive
    static unsigned char counter[ALLEGRO_KEY_MAX];

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
    if ( game -> status == NEW )
    {
        memset(counter, 0, sizeof (counter));
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
                // Config mode
            case CONFIG:
                game -> initial.ready = true;
                break;

                // New or playing mode
            case NEW:
            case PLAYING:
                game -> status = PAUSED;

                break;

                // Game paused
            case PAUSED:
                // Main menu
                if ( pMenu -> draw.beingDrawn == MAINMENU )
                {
                    switch ( pMenu -> selected.n )
                    {
                        case RESUME:
                            game -> status = PLAYING;
                            break;

                        case RESTART:
                            game -> restart = true;
                            break;

                        case OPTIONS:
                            pMenu -> draw.beingDrawn = OPTIONSMENU;
                            pMenu -> selected.n = 0;
                            break;

                        case EXIT:
                            game -> alStru -> exit = true;
                            game -> exit = true;
                            break;

                        default:
                            break;
                    }
                }

                    // Options menu
                else if ( pMenu -> draw.beingDrawn == OPTIONSMENU )
                {
                    switch ( pMenu -> selected.n )
                    {
                        case MUSIC:
                            game->alStru->samples.music.invertStatus();
                            break;

                        case FX:
                            game->alStru->samples.fx.invertStatus();
                            break;

                        case NP:
                            (pMenu -> get.nextPiece == true) ?
                                    (pMenu -> get.nextPiece = false) :
                                    (pMenu -> get.nextPiece = true);
                            break;

                        case OPRET:
                            pMenu -> draw.beingDrawn = MAINMENU;
                            pMenu -> selected.n = 0;
                            break;

                        default:
                            break;
                    }
                }

        }
    }

    // Quit game
    if ( key[ALLEGRO_KEY_Q] == KEY_READY )
    {
        validKey(ALLEGRO_KEY_Q, key, counter, game);

        // Avoid bug in initial screen
        game -> initial.ready = true;
        game -> alStru -> exit = true;
        // Exit game
        game -> exit = true;
    }

    // Key management
    switch ( game -> status )
    {
            // Config mode
        case CONFIG:
            if ( key[ALLEGRO_KEY_UP] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_UP, key, counter, game);

                (game -> initial.rl == 0) ? \
            (game -> initial.rl)++ : (game -> initial.rl)--;

            }

            if ( key[ALLEGRO_KEY_DOWN] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_DOWN, key, counter, game);

                (game -> initial.rl == 0) ? \
            (game -> initial.rl)++ : (game -> initial.rl)--;

            }

            if ( key[ALLEGRO_KEY_LEFT] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_LEFT, key, counter, game);

                switch ( game -> initial.rl )
                {
                    case ROWS:
                        (game -> initial.n[ROWS] > 0) ? \
                                (game -> initial.n[ROWS])-- : \
                                (game -> initial.n[ROWS] = NROWS - 1);
                        break;

                    case LEVEL:
                        (game -> initial.n[LEVEL] > 0) ? \
                                (game -> initial.n[LEVEL])-- : \
                                (game -> initial.n[LEVEL] = NLEVELS - 1);
                        break;

                    default:
                        fputs("Game initial.n is bigger than 1.", stderr);
                        break;
                }

            }

            if ( key[ALLEGRO_KEY_RIGHT] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_RIGHT, key, counter, game);

                switch ( game -> initial.rl )
                {
                    case ROWS:
                        (game -> initial.n[ROWS] < (NROWS - 1)) ? \
                                (game -> initial.n[ROWS])++ : \
                                (game -> initial.n[ROWS] = 0);
                        break;

                    case LEVEL:
                        (game -> initial.n[LEVEL] < (NLEVELS - 1)) ? \
                                (game -> initial.n[LEVEL])++ : \
                                (game -> initial.n[LEVEL] = 0);
                        break;

                    default:
                        fputs("Game initial.n is bigger than 1.", stderr);
                        break;
                }

            }
            break;

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

                (game -> logic.ask.movingPiece()) ?
                        playFX(FX_DROP, game) : 0;
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
            break;

            // Pause mode
        case PAUSED:

            if ( key[ALLEGRO_KEY_UP] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_UP, key, counter, game);

                (pMenu -> selected.n == 0) ? \
                (pMenu -> selected.n = pMenu -> get.nWords - 1) : \
                (pMenu -> selected.n)--;

            }

            if ( key[ALLEGRO_KEY_DOWN] == KEY_READY )
            {
                validKey(ALLEGRO_KEY_DOWN, key, counter, game);

                (pMenu -> selected.n == pMenu -> get.nWords - 1) ? \
                    (pMenu -> selected.n = 0) : (pMenu -> selected.n)++;
            }

            // Clear unnecesary keys used only in "play mode"
            for ( i = 0; i < (sizeof (playKeys) / sizeof (char)); i++ )
            {
                for ( j = 0; (j < (sizeof (pauseKeys) / sizeof (char))) &&
                      (bothModes == false); j++ )
                {
                    (playKeys[i] == pauseKeys[j]) ? (bothModes = true) : 0;
                }

                (bothModes == false) ?
                        clearKey(playKeys[i], key) : (bothModes = false);
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Draw the initial game screen. Lets user pick an initial level and row
 * 
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
drawInitial (game_t * game)
{
    int i;
    int levelNumberSize = al_get_text_width(game->initial.level_sel.text.bold,
                                            "0");
    int rowsNumberSize = al_get_text_width(game->initial.rows_sel.text.bold,
                                           "0");

    // Background
    al_clear_to_color(al_color_html(GAME_BKGND_COLOR));

    if ( game -> status == INITIAL )
    {
        game -> bkgnd = al_load_bitmap("res/images/game/initial.png");

        if ( game -> bkgnd == NULL )
        {
            fputs("Error loading background.", stderr);
        }

        game -> status = CONFIG;
    }

    if ( game -> bkgnd != NULL )
    {
        al_draw_scaled_bitmap(game -> bkgnd,
                              0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              0);
    }

    // Boxes
    // Level
    al_draw_rounded_rectangle(game -> initial.level.corner.x,
                              game -> initial.level.corner.y,
                              game -> initial.level.corner.x + \
                                                   game -> initial.level.width,
                              game -> initial.level.corner.y + \
                                                  game -> initial.level.height,
                              BOX_ROUND_X, BOX_ROUND_Y,
                              al_color_html(game-> initial.level.color.border),
                              game -> initial.level.thickness);

    // Rows
    al_draw_rounded_rectangle(game -> initial.rows.corner.x,
                              game -> initial.rows.corner.y,
                              game -> initial.rows.corner.x + \
                                                   game -> initial.rows.width,
                              game -> initial.rows.corner.y + \
                                                  game -> initial.rows.height,
                              BOX_ROUND_X, BOX_ROUND_Y,
                              al_color_html(game -> initial.rows.color.border),
                              game -> initial.rows.thickness);

    // Boxes heading
    al_draw_text(game -> initial.level.text.bold,
                 al_color_html(game -> initial.level.text.color),
                 SCREEN_WIDTH / 2,
                 game -> initial.level.corner.y - GAME_TXT_OFFSET - GAME_TXT_SIZE_BOLD,
                 ALLEGRO_ALIGN_CENTRE, "Initial Level");

    al_draw_text(game -> initial.rows.text.bold,
                 al_color_html(game -> initial.rows.text.color),
                 SCREEN_WIDTH / 2,
                 game -> initial.rows.corner.y - GAME_TXT_OFFSET - GAME_TXT_SIZE_BOLD,
                 ALLEGRO_ALIGN_CENTRE, "Initial Rows");

    // Draw numbers inside the level box
    for ( i = 0; i < NLEVELS; i++ )
    {
        // Selected number
        if ( i == game -> initial.n[LEVEL] )
        {
            al_draw_textf(game -> initial.level_sel.text.bold,
                          al_color_html(game -> initial.level_sel.text.color),
                          game -> initial.level.corner.x + 2 * GAME_TXT_OFFSET + \
                          levelNumberSize / 2 + \
                          (game -> initial.level.width - \
                            2 * GAME_TXT_OFFSET) / 10 * i,
                          game -> initial.level.corner.y + \
                          game -> initial.level.height / 2 - \
                          game -> initial.level_sel.text.bsize / 2,
                          ALLEGRO_ALIGN_CENTRE, "%d", i);

            // If the cursor is selecting it, also draw a box around the number
            if ( game -> initial.rl == LEVEL )
            {
                al_draw_rectangle(game -> initial.level.corner.x + \
                              i * (game -> initial.level.width - \
                              2 * GAME_TXT_OFFSET) / 10 + GAME_TXT_OFFSET,
                                  game -> initial.level.corner.y + \
                              game -> initial.level.height / 2 - \
                              game -> initial.level_sel.text.bsize / 2 - \
                              GAME_TXT_OFFSET / 2,
                                  game -> initial.level.corner.x + \
                              i * (game -> initial.level.width - \
                              2 * GAME_TXT_OFFSET) / 10 + 3 * GAME_TXT_OFFSET + \
                              levelNumberSize,
                                  game -> initial.level.corner.y + \
                              game -> initial.level.height / 2 + \
                              game -> initial.level_sel.text.bsize / 2 + \
                              GAME_TXT_OFFSET / 2,
                                  al_color_html( \
                                       game -> initial.level_sel.color.border),
                                  game -> initial.level_sel.thickness);
            }
        }

            // Not selected number
        else
        {
            al_draw_textf(game -> initial.level.text.regular,
                          al_color_html(game -> initial.level.text.color),
                          game -> initial.level.corner.x + 2 * GAME_TXT_OFFSET + \
                          levelNumberSize / 2 + \
                          (game -> initial.level.width - 2 * GAME_TXT_OFFSET) / 10 * i,
                          game -> initial.level.corner.y + \
                          game -> initial.level.height / 2 - \
                          game -> initial.level_sel.text.bsize / 2,
                          ALLEGRO_ALIGN_CENTRE, "%d", i);
        }
    }

    // Draw numbers inside the rows box
    for ( i = 0; i < NROWS; i++ )
    {
        // Selected number
        if ( i == game -> initial.n[ROWS] )
        {
            al_draw_textf(game -> initial.rows_sel.text.bold,
                          al_color_html(game -> initial.rows_sel.text.color),
                          game -> initial.rows.corner.x + 2 * GAME_TXT_OFFSET + \
                          rowsNumberSize / 2 + \
                          (game -> initial.rows.width - \
                            2 * GAME_TXT_OFFSET) / 10 * i,
                          game -> initial.rows.corner.y + \
                          game -> initial.rows.height / 2 - \
                          game -> initial.rows_sel.text.bsize / 2,
                          ALLEGRO_ALIGN_CENTRE, "%d", i);

            // If the cursor is selecting it, also draw a box around the number
            if ( game -> initial.rl == ROWS )
            {
                al_draw_rectangle(game -> initial.rows.corner.x + \
                              i * (game -> initial.rows.width - \
                              2 * GAME_TXT_OFFSET) / 10 + GAME_TXT_OFFSET,
                                  game -> initial.rows.corner.y + \
                              game -> initial.rows.height / 2 - \
                              game -> initial.rows_sel.text.bsize / 2 - \
                              GAME_TXT_OFFSET / 2,
                                  game -> initial.rows.corner.x + \
                              i * (game -> initial.rows.width - \
                              2 * GAME_TXT_OFFSET) / 10 + 3 * GAME_TXT_OFFSET + \
                              levelNumberSize,
                                  game -> initial.rows.corner.y + \
                              game -> initial.rows.height / 2 + \
                              game -> initial.rows_sel.text.bsize / 2 + \
                              GAME_TXT_OFFSET / 2,
                                  al_color_html( \
                                       game -> initial.rows_sel.color.border),
                                  game -> initial.rows_sel.thickness);
            }
        }

            // Not selected number
        else
        {

            al_draw_textf(game -> initial.rows.text.regular,
                          al_color_html(game -> initial.rows.text.color),
                          game -> initial.rows.corner.x + 2 * GAME_TXT_OFFSET + \
                          rowsNumberSize / 2 + \
                          (game -> initial.rows.width - \
                            2 * GAME_TXT_OFFSET) / 10 * i,
                          game -> initial.rows.corner.y + \
                          game -> initial.rows.height / 2 - \
                          game -> initial.rows_sel.text.bsize / 2,
                          ALLEGRO_ALIGN_CENTRE, "%d", i);
        }
    }

    // Redrawing is not necesary
    game -> redraw = false;

    // Flip
    al_flip_display();
}

/**
 * @brief Draw the game screen (without stats and the game board)
 * 
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
drawScreen (game_t * game)
{
    // Draw background
    al_clear_to_color(al_color_html(GAME_BKGND_COLOR));

    if ( game -> status == NEW )
    {
        game -> bkgnd = al_load_bitmap("res/images/game/background.png");

        if ( game -> bkgnd == NULL )
        {

            fputs("Error loading background.", stderr);
            return;
        }
    }

    al_draw_scaled_bitmap(game -> bkgnd,
                          0, 0, BKGND_WIDTH, BKGND_HEIGHT,
                          0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          0);
}

/**
 * @brief Draw the current game board in the screen
 * 
 * @param board Structure with the box settings to draw
 * 
 * @return Nothing
 */
static void
drawGameBoard (screenBoard_t * board)
{
    int x, y;
    grid_t current;

    // Draw board box
    primitive_drawBox(&(board -> boardBox));

    // Draw pieces inside the board
    for ( y = 0; y < BOARD_HEIGHT; y++ )
    {
        for ( x = 0; x < BOARD_WIDTH; x++ )
        {
            // Get current cell block type
            current = GET_CELL(y, x, board -> r0c0);

            // Moving piece
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

                // Fixed pieces
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

/**
 * @brief Draw a single piece block in the given coordinates
 * 
 * @param x Top left X coordinate
 * @param y Top left Y coordinate
 * @param block Box with the block settings
 * 
 * @return Nothing
 */
static void
drawBlock (int x, int y, gbox_t * block)
{
    // Draw background
    al_draw_filled_rounded_rectangle(block -> corner.x + x * CELL_WIDTH,
                                     block -> corner.y + y * CELL_HEIGHT,
                                     block -> corner.x + x * CELL_WIDTH + \
                                                            block -> width,
                                     block -> corner.y + y * CELL_HEIGHT + \
                                                            block -> height,
                                     BOX_ROUND_X, BOX_ROUND_Y,
                                     al_color_html(block -> color.bkgnd));

    // Draw border
    al_draw_rounded_rectangle(block -> corner.x + x * CELL_WIDTH,
                              block -> corner.y + y * CELL_HEIGHT,
                              block -> corner.x + x * CELL_WIDTH + \
                                                            block -> width,
                              block -> corner.y + y * CELL_HEIGHT + \
                                                            block -> height,
                              BOX_ROUND_X, BOX_ROUND_Y,
                              al_color_html(block -> color.border),
                              (block -> thickness));
}

/**
 * @brief Destroy current game structure
 * 
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
destroy (game_t * game)
{

    game -> logic.destroy();
    al_destroy_timer(game -> timer.main);
    al_destroy_event_queue(game -> evq);
    al_destroy_bitmap(game -> bkgnd);
}

/**
 * @brief Initialize the screen board (where the game board is printed)
 * 
 * @param board Screen Board structure where the board will be printed
 * @param logic Current game logic
 * 
 * @return Nothing
 */
static void
init_scrBoard (screenBoard_t * board, board_t * logic)
{
    // Counter
    int i;

    // Box settings
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

    // Get top left coordinate of the board
    board -> r0c0 = logic -> ask.board();

    // Get game stats
    board -> stats = (stats_t*) logic -> ask.stats();
}

/**
 * @brief Manage all in-game events from here
 * 
 * This includes display, keyboard and timers events
 * 
 * @param game Current game structure
 * @param boardLogic Backend logic
 * @param pMenu Pause menu structure
 */
static void
manageEvents (game_t * game, board_t * boardLogic, pause_t * pMenu)
{
    // Current event
    ALLEGRO_EVENT event;

    // Keys array
    static unsigned char key[ALLEGRO_KEY_MAX];

    static bool key_init = false;

    // Clear keys when a new game starts or before selecting the initial rows 
    // and level
    if ( key_init == false || game -> status == NEW )
    {
        memset(key, 0, sizeof (key));
        key_init = true;
    }

    // Wait for next event
    al_wait_for_event(game -> evq, &event);

    // Check events
    switch ( event.type )
    {
            // Display
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            game -> exit = true;
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
            if ( event.timer.source == game -> timer.main )
            {
                checkKeys(key, game, boardLogic, pMenu);

            }

                // Piece timer
            else if ( event.timer.source == game -> timer.piece &&
                      game -> status != PAUSED )
            {
                // Set the game as playing before the first board update
                if ( game -> status == NEW )
                {
                    game -> status = PLAYING;
                }

                // Call for a board update
                boardLogic -> update();

                // Play a sound FX
                (game -> logic.ask.movingPiece()) ?
                        playFX(FX_DROP, game) : 0;

                // Redraw screen
                game -> redraw = true;
            }
            break;

        default:
            break;
    }

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
          game_t * game)
{
    // Clear key
    clearKey(keyName, keyArr);
    // Increase counter of this key
    counterArr[keyName]++;
    // Redraw screen
    game -> redraw = true;
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
 * @brief Restart current game pieces' timer
 * 
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
restartPiecesTimer (game_t * game)
{
    // Get piece speed
    game -> timer.piece_speed = askTimeLimit();

    // Restart pieces timer
    al_stop_timer(game -> timer.piece);

    al_set_timer_speed(game -> timer.piece, MS2S(game -> timer.piece_speed));

    al_start_timer(game -> timer.piece);
}

/**
 * @brief Initialize the screen where initial rows and level are choosen
 * 
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
init_rowLevel (game_t * game)
{
    int numberSize;

    // == Boxes ==
    // = Background =
    // Not selected
    game -> initial.level.color.bkgnd = \
    game -> initial.rows.color.bkgnd = BOX_BKGND_COLOR;

    // Selected
    game -> initial.level_sel.color.bkgnd = \
    game -> initial.rows_sel.color.bkgnd = SEL_BKGND_COLOR;

    // = Border =
    // Not selected
    game -> initial.level.color.border = \
    game -> initial.rows.color.border = BOX_BORDER_COLOR;

    // Selected
    game -> initial.level_sel.color.border = \
    game -> initial.rows_sel.color.border = SEL_BORDER_COLOR;

    // Thickness (not selected)
    game -> initial.level.thickness = \
    game -> initial.rows.thickness = BOX_THICKNESS;

    // Thickness (selected)
    game -> initial.level_sel.thickness = \
    game -> initial.rows_sel.thickness = SEL_BOX_THICKNESS;

    // == Text ==
    // = Regular size =
    game -> initial.level.text.rsize = \
    game -> initial.rows.text.rsize = \
    game -> initial.level_sel.text.rsize = \
    game -> initial.rows_sel.text.rsize = GAME_TXT_SIZE;

    // = Bold size =
    game -> initial.level.text.bsize = \
    game -> initial.rows.text.bsize = \
    game -> initial.level_sel.text.bsize = \
    game -> initial.rows_sel.text.bsize = GAME_TXT_SIZE_BOLD;

    // = Regular font =
    game -> initial.level.text.regular = \
    game -> initial.rows.text.regular = \
    game -> initial.level_sel.text.regular = \
    game -> initial.rows_sel.text.regular = al_load_font(GAME_TXT_FONT_PATH, \
                                          game -> initial.level.text.rsize, 0);

    // = Bold font =
    game -> initial.level.text.bold = \
    game -> initial.rows.text.bold = \
    game -> initial.level_sel.text.bold = \
    game -> initial.rows_sel.text.bold = al_load_font(GAME_TXT_FONT_BOLD_PATH, \
                                          game -> initial.level.text.bsize, 0);

    // = Color =
    // Not selected
    game -> initial.level.text.color = \
    game -> initial.rows.text.color = GAME_TXT_COLOR;

    // Selected
    game -> initial.level_sel.text.color = \
    game -> initial.rows_sel.text.color = SEL_TXT_COLOR;

    // Width of a number
    numberSize = al_get_text_width(game -> initial.level_sel.text.bold, "0");

    // Box height
    game -> initial.level.height = game -> initial.rows.height = \
     GAME_TXT_OFFSET * 2 + GAME_TXT_SIZE_BOLD * 2;

    // Box width
    game -> initial.level.width = numberSize * NLEVELS + GAME_TXT_OFFSET * 22;

    game -> initial.rows.width = numberSize * NROWS + GAME_TXT_OFFSET * 22;

    // X and Y coordinates
    game -> initial.level.corner.x = game -> initial.rows.corner.x = \
    (SCREEN_WIDTH / 2) - (game -> initial.rows.width / 2);

    game -> initial.level.corner.y = (SCREEN_HEIGHT / 3) - \
                                            (game -> initial.level.height / 2);

    game -> initial.rows.corner.y = (2 * SCREEN_HEIGHT / 3) - \
                                            (game -> initial.level.height / 2);
}

/**
 * @brief Play a sound FX
 * 
 * @param path Path to the sound FX to be played
 * @param game Current game structure
 * 
 * @return Nothing
 */
static void
playFX (char * path, game_t * game)
{
    if ( game -> alStru->samples.fx.enabled() )
    {
        game -> alStru ->samples.fx.play(path);
    }
}
