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
 * @file    gameStats.c
 * 
 * @brief   Draws the stats in the current game screen
 * 
 * @details While playing, functions in this file allow displaying all the 
 * stats in of the current game in the display.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    20/01/2020, 18:45
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

#include "../alcontrol.h"

// Game logic
#include "../../../backend/stats/stats_mgmt.h"

// This file
#include "gameStats.h"

/// @privatesection
// === Constants and Macro definitions ===

// Board cell size
#define CELL_WIDTH          25.0
#define CELL_HEIGHT         CELL_WIDTH
#define CELL_THICKNESS      2.0


// === Enumerations, structures and typedefs ===

// Used with array "words" to access each word

enum wordsarr
{
    STATS,
    LVL,
    ASCORE,
    TSCORE,
    NP,
    LINESC
};

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Draws a piece in a box whose coordinates are given by x and y args.
static void
drawPiece (float x, float y, int piece);

// Draw the current level box with its corresponding information.
static void
level (screenStats_t * stats);

// Draw the lines box with its corresponding information.
static void
lines (screenStats_t * stats);

// Draw the Next Piece box with its corresponding information
static void
nextPiece (screenStats_t * stats);

// Draw the pieces box with its corresponding information
static void
pieces (screenStats_t * stats);

// Draw the score box with its corresponding information
static void
score (screenStats_t * stats);

// === ROM Constant variables with file level scope ===

// Tetrominos background (0 to TETROMINOS) plus the border color (TETROMINOS+1)
static const char * tetrominos_bkgnd[TETROMINOS + 1] = {
    "#00A2E8", "#3F48CC", "#FF7F27", "#FFC90E",
    "#22B14C", "#A349A4", "#ED1C24",
    "#F0F0F0"
};

// Text of each box. Used alogside wordsarr enum
static const char * words[] = {
    "Statistics", "Level", "Actual Score",
    "Top Score", "Next Piece", "Lines Cleared",
    NULL
};
// === Static variables and constant variables with file level scope ===


// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize game stats displayed when using Allegro
 * 
 * @warning This function must be called before using any of the drawStats_*_f
 * functions
 * 
 * @param stats Structure with stats boxes an the stats logic
 * @param logic Game logic
 * 
 * @return Nothing
 */
void
init_scrStats (screenStats_t * stats, board_t * logic)
{
    // Stats structure
    stats -> gStats = (stats_t*) logic -> ask.stats();

    stats -> showNextPiece = true;

    // Level box
    stats -> levelBox.height = GAME_TXT_SIZE + BOX_IN_OFFSET * 2;
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
            stats -> piecesBox.text.color = GAME_TXT_COLOR;

    stats -> levelBox.text.rsize = stats -> scoreBox.text.rsize = \
            stats -> nextPieceBox.text.rsize = \
            stats -> linesBox.text.rsize = \
            stats -> piecesBox.text.rsize = GAME_TXT_SIZE;

    stats -> levelBox.text.bsize = stats -> scoreBox.text.bsize = \
            stats -> nextPieceBox.text.bsize = \
            stats -> linesBox.text.bsize = \
            stats -> piecesBox.text.bsize = GAME_TXT_SIZE_BOLD;

    stats -> levelBox.text.regular = stats -> scoreBox.text.regular = \
            stats -> nextPieceBox.text.regular = \
            stats -> piecesBox.text.regular = \
            stats -> linesBox.text.regular = \
            al_load_font(GAME_TXT_FONT_PATH, stats -> levelBox.text.rsize, 0);

    stats -> levelBox.text.bold = stats -> scoreBox.text.bold = \
            stats -> nextPieceBox.text.bold = \
            stats -> piecesBox.text.bold = \
            stats -> linesBox.text.bold = \
            al_load_font(GAME_TXT_FONT_BOLD_PATH, stats -> levelBox.text.bsize, 0);
}

/**
 * @brief Draw the current level box with its corresponding information.
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
void
drawStats_level_f (screenStats_t * stats)
{
    level(stats);
}

/**
 * @brief Draw the lines box with its corresponding information.
 * 
 * Display how many lines have been cleared
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
void
drawStats_lines_f (screenStats_t * stats)
{
    lines(stats);
}

/**
 * @brief Draw the Next Piece box with its corresponding information
 * 
 * When showNextPiece is set to true, the next piece is shown to the user.
 * Otherwhise, the box is shown empty.
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
void
drawStats_nextPiece_f (screenStats_t * stats)
{
    nextPiece(stats);
}

/**
 * @brief Draw the pieces box with its corresponding information
 * 
 * Prints how many pieces of each kind have been dropped
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
void
drawStats_pieces_f (screenStats_t * stats)
{
    pieces(stats);
}

/**
 * @brief Draw the score box with its corresponding information
 * 
 * Shows the top and actual game scores
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
void
drawStats_score_f (screenStats_t * stats)
{
    score(stats);
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Draws a piece in a box whose coordinates are given by x and y args.
 * 
 * Given x and y coordinates, top left corner of a box, draws a piece inside
 * it. The box is an 4x8 rectangle, being each cell of size CELL_HEIGHT/2
 * 
 * @param x Top left X coordinate
 * @param y Top left Y coordinate
 * @param piece Piece to draw. Uses piecesType enum ( in board.h ) 
 * 
 * @return Nothing
 */
static void
drawPiece (float x, float y, int piece)
{
    // x , y are (0,0) -top left corner- of a 4x8 CELL_HEIGHT/2 rectangle
    // Counters
    int i, j;

    // Pieces blocks
    int block[BLOCKS][COORD_NUM] = {};

    // Box grid where the piece will be drawn
    float pieceGrid[BLOCKS][ 2 * BLOCKS ][COORD_NUM] = {};

    // Displacement of the given coordinates to center the piece properly
    float modCoord[COORD_NUM] = {x, y};

    // Calculate the grid coordinates
    for ( i = 0; i < BLOCKS; i++ )
    {
        for ( j = 0; j < (2 * BLOCKS); j++ )
        {
            // Coordinates X and Y of the grid
            pieceGrid[i][j][COORD_X] = modCoord[COORD_X];
            pieceGrid[i][j][COORD_Y] = modCoord[COORD_Y];

            // Next cell X coordinate (in the same row)
            modCoord[COORD_X] += (CELL_WIDTH / 2);
        }

        // Continue at the begining of the next row
        modCoord[COORD_X] = x;
        modCoord[COORD_Y] += (CELL_HEIGHT / 2);
    }

    // Calculate each pieces coordinates
    switch ( piece )
    {
        case TETROMINO_I:
            // It's a rectangle. All blocks are one next to the other
            for ( i = b1; i < BLOCKS; i++ )
            {
                block[i][COORD_X] = pieceGrid[1][i * 2][COORD_X];
                block[i][COORD_Y] = pieceGrid[1][i * 2][COORD_Y];
            }

            break;

        case TETROMINO_J:
            j = 1;

            // Top block
            block[b4][COORD_X] = pieceGrid[0][j][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j][COORD_Y];

            // Rectangle
            for ( i = b1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }
            break;

        case TETROMINO_L:
            // Rectangle
            for ( i = b1, j = 1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            // Top block
            block[b4][COORD_X] = pieceGrid[0][j - 2][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j - 2][COORD_Y];
            break;

        case TETROMINO_O:
            // Top rectangle
            for ( i = b1, j = 2; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            // Bottom rectangle
            for ( i = (BLOCKS / 2), j = 2; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            break;

        case TETROMINO_S:
            // Top rectangle
            for ( i = b1, j = 3; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            // Bottom rectangle
            for ( i = (BLOCKS / 2), j = 1; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            break;

        case TETROMINO_T:
            // Bottom rectangle
            for ( i = b1, j = 1; i < b4; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }

            // Top block
            block[b4][COORD_X] = pieceGrid[0][j - 4][COORD_X];
            block[b4][COORD_Y] = pieceGrid[0][j - 4][COORD_Y];
            break;

        case TETROMINO_Z:
            // Top rectangle
            for ( i = b1, j = 1; i < (BLOCKS / 2); i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[0][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[0][j][COORD_Y];
            }

            // Bottom rectangle
            for ( i = (BLOCKS / 2), j = 3; i < BLOCKS; i++, j += 2 )
            {
                block[i][COORD_X] = pieceGrid[2][j][COORD_X];
                block[i][COORD_Y] = pieceGrid[2][j][COORD_Y];
            }
            break;

        default:
            break;
    }

    // Draw the piece block by block
    for ( i = 0; i < BLOCKS; i++ )
    {
        // Draw the background
        al_draw_filled_rounded_rectangle(block[i][COORD_X],
                                         block[i][COORD_Y],
                                         block[i][COORD_X] + CELL_WIDTH,
                                         block[i][COORD_Y] + CELL_HEIGHT,
                                         BOX_ROUND_X, BOX_ROUND_Y,
                                         al_color_html(tetrominos_bkgnd[piece]));

        // Draw the border
        al_draw_rounded_rectangle(block[i][COORD_X],
                                  block[i][COORD_Y],
                                  block[i][COORD_X] + CELL_WIDTH,
                                  block[i][COORD_Y] + CELL_HEIGHT,
                                  BOX_ROUND_X, BOX_ROUND_Y,
                                  al_color_html(tetrominos_bkgnd[TETROMINOS]),
                                  CELL_THICKNESS);
    }
}

/**
 * @brief Draw the current level box with its corresponding information.
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
static void
level (screenStats_t * stats)
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

/**
 * @brief Draw the lines box with its corresponding information.
 * 
 * Display how many lines have been cleared
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
static void
lines (screenStats_t * stats)
{

    float fontHeight_B = al_get_font_line_height(stats->linesBox.text.bold);

    stats -> linesBox.corner.x = stats -> scoreBox.corner.x;
    stats -> linesBox.corner.y = stats -> nextPieceBox.corner.y + \
                                 stats -> nextPieceBox.height + BOX_OFFSET;

    stats -> linesBox.width = stats -> scoreBox.width;
    stats -> linesBox.height = 2 * fontHeight_B + 3 * GAME_TXT_OFFSET;

    // Draw level information in its box
    al_draw_text(stats -> linesBox.text.bold,
                 al_color_html(stats -> linesBox.text.color),
                 stats -> linesBox.width / 2 + stats -> linesBox.corner.x,
                 stats -> linesBox.corner.y + GAME_TXT_OFFSET,
                 ALLEGRO_ALIGN_CENTRE, words[LINESC]);

    al_draw_textf(stats->linesBox.text.regular,
                  al_color_html(stats -> linesBox.text.color),
                  stats->linesBox.width / 2 + stats -> linesBox.corner.x,
                  fontHeight_B + 2 * GAME_TXT_OFFSET + stats->linesBox.corner.y,
                  ALLEGRO_ALIGN_CENTRE,
                  "%03d", stats -> gStats -> lines.cleared);

    drawBox(&(stats -> linesBox));
}

/**
 * @brief Draw the Next Piece box with its corresponding information
 * 
 * When showNextPiece is set to true, the next piece is shown to the user.
 * Otherwhise, the box is shown empty.
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
static void
nextPiece (screenStats_t * stats)
{
    float fontHeight_B = al_get_font_line_height(stats->nextPieceBox.text.bold);
    float coord[COORD_NUM] = {};

    // == Next Piece box ==
    stats -> nextPieceBox.corner.x = stats -> scoreBox.corner.x;
    stats -> nextPieceBox.corner.y = stats -> scoreBox.height + 2 * BOX_OFFSET;

    stats -> nextPieceBox.width = stats -> scoreBox.width;

    coord[COORD_X] = stats->nextPieceBox.width / 2 + \
                                                stats -> nextPieceBox.corner.x;
    coord[COORD_Y] = stats->nextPieceBox.corner.y + GAME_TXT_OFFSET;
    // Draw next piece information in its box
    al_draw_text(stats->nextPieceBox.text.bold,
                 al_color_html(stats -> nextPieceBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[NP]);

    if ( stats->gStats->piece.next != TETROMINO_NONE &&
         (stats ->showNextPiece == true) )
    {

        coord[COORD_X] = (stats->nextPieceBox.width / 2) + \
                               stats -> nextPieceBox.corner.x - CELL_WIDTH * 2;
        coord[COORD_Y] = stats->nextPieceBox.corner.y + 2.5 * GAME_TXT_OFFSET + \
                            fontHeight_B;

        drawPiece(coord[COORD_X], coord[COORD_Y], stats->gStats->piece.next);
    }

    stats -> nextPieceBox.height = 2 * CELL_HEIGHT + fontHeight_B + \
                                                           4 * GAME_TXT_OFFSET;
    drawBox(&(stats -> nextPieceBox));
}

/**
 * @brief Draw the pieces box with its corresponding information
 * 
 * Prints how many pieces of each kind have been dropped
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
static void
pieces (screenStats_t * stats)
{
    int i;
    float coord[COORD_NUM] = {};

    // == Draw pieces box ==
    // Information
    for ( i = TETROMINO_I; i < TETROMINOS; i++ )
    {

        coord[COORD_X] = stats -> piecesBox.corner.x + GAME_TXT_OFFSET;
        coord[COORD_Y] = stats->piecesBox.corner.y + \
                                     i * GAME_TXT_OFFSET + 2.5 * i * CELL_HEIGHT + \
                  1.5 * al_get_font_line_height(stats->piecesBox.text.regular);

        drawPiece(coord[COORD_X], coord[COORD_Y], i);

        coord[COORD_X] += CELL_WIDTH * 4 + GAME_TXT_OFFSET;

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
                  stats -> piecesBox.corner.y + GAME_TXT_OFFSET,
                  ALLEGRO_ALIGN_CENTRE,
                  words[STATS]);

    // Draw box
    drawBox(&(stats -> piecesBox));
}

/**
 * @brief Draw the score box with its corresponding information
 * 
 * Shows the top and actual game scores
 * 
 * @param stats Structure previously passed to init_scrStats()
 * 
 * @return Nothing
 */
static void
score (screenStats_t * stats)
{

    float fontHeight = al_get_font_line_height(stats->scoreBox.text.regular);
    float fontHeight_B = al_get_font_line_height(stats->scoreBox.text.bold);
    float coord[COORD_NUM] = {};

    // == Draw score box ==
    // Actual score
    coord[COORD_X] = (stats->scoreBox.width / 2) + stats -> scoreBox.corner.x;
    coord[COORD_Y] = stats->scoreBox.corner.y + GAME_TXT_OFFSET;

    al_draw_text(stats->scoreBox.text.bold,
                 al_color_html(stats -> scoreBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[ASCORE]);

    coord[COORD_Y] += GAME_TXT_OFFSET + fontHeight_B;

    al_draw_textf(stats->scoreBox.text.regular,
                  al_color_html(stats -> scoreBox.text.color),
                  coord[COORD_X], coord[COORD_Y],
                  ALLEGRO_ALIGN_LEFT,
                  "%06d", stats -> gStats -> score.actual);

    // Top score
    coord[COORD_Y] += GAME_TXT_OFFSET + fontHeight;

    al_draw_text(stats->scoreBox.text.bold,
                 al_color_html(stats -> scoreBox.text.color),
                 coord[COORD_X], coord[COORD_Y],
                 ALLEGRO_ALIGN_CENTRE,
                 words[TSCORE]);

    coord[COORD_Y] += GAME_TXT_OFFSET + fontHeight_B;

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
