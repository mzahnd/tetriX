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
 * @file    piece_actions.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    28/11/2019, 18:31
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// For NUM_PIECES and PIECE_X definitions
#include "../board.h"

// This file
#include "piece_actions.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
static void
init (const char * bag, const int position);

static void
rotate (void);

static void
rotate_I (void);

static void
rotate_J (void);

static void
rotate_L (void);

static void
rotate_S (void);

static void
rotate_T (void);

static void
rotate_Z (void);

static void
shift (int * direction);

static void
update_board (void);

static void
update_coordinates (int c1[2], int c2[2], int c3[2], int c4[2]);

static void
update_rotation (void);
// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static piece_t * currentPiece = NULL;

static char *** gboard = NULL;

static int boardHeight;

static int boardWidth;

// === Global function definitions ===
/// @publicsection

int
piece_init (struct PIECE * pstruct,
            const char * bag, const int position,
            char *** board, const int board_h, const int board_w)
{
    currentPiece = pstruct;

    pstruct -> rotate = &rotate;

    pstruct -> shift = &shift;

    pstruct -> update_board = &update_board;

    boardHeight = board_h;

    boardWidth = board_w;

    gboard = board;

    init(bag, position);

    return 0;
}

/// @privatesection
// === Local function definitions ===

static void
init (const char * bag, const int position)
{
    if ( gboard == NULL )
    {
        fputs("PIECE structure bad initialization.", stderr);
        return;
    }

    currentPiece -> rotation.status = false;
    currentPiece -> rotation.position = 1;

    currentPiece -> drop = false;

    currentPiece -> shifting = NONE;

    // Coordinates
    switch ( *(bag + position) )
    {
            /*
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * | b1 | b2 | b3 | b4 |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_I:
            currentPiece -> type = PIECE_I;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2 - 2;
            currentPiece -> coord.current.b2[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b3[0] = boardWidth / 2;
            currentPiece -> coord.current.b4[0] = boardWidth / 2 + 1;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 1;
            currentPiece -> coord.current.b2[1] = 1;
            currentPiece -> coord.current.b3[1] = 1;
            currentPiece -> coord.current.b4[1] = 1;

            break;

            /*
             * ---------------------
             * | b1 | b2 | b3 |    |
             * ---------------------
             * |    |    | b4 |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_J:
            currentPiece -> type = PIECE_J;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b2[0] = boardWidth / 2;
            currentPiece -> coord.current.b3[0] = boardWidth / 2 + 1;
            currentPiece -> coord.current.b4[0] = boardWidth / 2 + 1;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 0;
            currentPiece -> coord.current.b4[1] = 1;
            break;

            /*
             * ---------------------
             * | b1 | b2 | b3 |    |
             * ---------------------
             * | b4 |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_L:
            currentPiece -> type = PIECE_L;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b2[0] = boardWidth / 2;
            currentPiece -> coord.current.b3[0] = boardWidth / 2 + 1;
            currentPiece -> coord.current.b4[0] = boardWidth / 2 - 1;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 0;
            currentPiece -> coord.current.b4[1] = 1;
            break;

            /*
             * ---------------------
             * |    | b1 | b2 |    |
             * ---------------------
             * |    | b3 | b4 |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_O:
            currentPiece -> type = PIECE_O;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2;
            currentPiece -> coord.current.b2[0] = boardWidth / 2 + 1;
            currentPiece -> coord.current.b3[0] = boardWidth / 2;
            currentPiece -> coord.current.b4[0] = boardWidth / 2 + 1;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 1;
            currentPiece -> coord.current.b4[1] = 1;
            break;

            /*
             * ---------------------
             * |    | b1 | b2 |    |
             * ---------------------
             * | b3 | b4 |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_S:
            currentPiece -> type = PIECE_S;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2;
            currentPiece -> coord.current.b2[0] = boardWidth / 2 + 1;
            currentPiece -> coord.current.b3[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b4[0] = boardWidth / 2;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 1;
            currentPiece -> coord.current.b4[1] = 1;
            break;

            /*
             * ---------------------
             * | b1 | b2 | b3 |    |
             * ---------------------
             * |    | b4 |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_T:
            currentPiece -> type = PIECE_T;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b2[0] = boardWidth / 2;
            currentPiece -> coord.current.b3[0] = boardWidth / 2 + 1;
            currentPiece -> coord.current.b4[0] = boardWidth / 2;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 0;
            currentPiece -> coord.current.b4[1] = 1;
            break;

            /*
             * ---------------------
             * | b1 | b2 |    |    |
             * ---------------------
             * |    | b3 | b4 |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             * |    |    |    |    |
             * ---------------------
             */
        case PIECE_Z:
            currentPiece -> type = PIECE_Z;

            // X coordinates
            currentPiece -> coord.current.b1[0] = boardWidth / 2 - 1;
            currentPiece -> coord.current.b2[0] = boardWidth / 2;
            currentPiece -> coord.current.b3[0] = boardWidth / 2;
            currentPiece -> coord.current.b4[0] = boardWidth / 2 + 1;

            // Y coordinates
            currentPiece -> coord.current.b1[1] = 0;
            currentPiece -> coord.current.b2[1] = 0;
            currentPiece -> coord.current.b3[1] = 1;
            currentPiece -> coord.current.b4[1] = 1;
            break;

        default:
            break;
    }
}

static void
rotate (void)
{
    if ( gboard == NULL )
    {
        fputs("PIECE structure not initialized.", stderr);
        return;
    }

    switch ( currentPiece -> type )
    {
        case PIECE_I:
            rotate_I();
            break;

        case PIECE_J:
            rotate_J();
            break;

        case PIECE_L:
            rotate_L();
            break;

        case PIECE_O:
            // Nothing to rotate
            update_rotation();
            break;

        case PIECE_S:
            rotate_S();
            break;

        case PIECE_T:
            rotate_T();
            break;

        case PIECE_Z:
            rotate_Z();
            break;

        default:
            break;
    }
}

static void
rotate_I (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    | b1 |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    | b2 |    |
             * ---------------------  --->  ---------------------
             * | b1 | b2 | b3 | b4 |        |    |    | b3 |    |
             * ---------------------        --------------------- 
             * |    |    |    |    |        |    |    | b4 |    |
             * ---------------------        ---------------------
             */
        case 1:
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 2;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 2;

            new_b2[0] = currentPiece -> coord.current.b2[0] + 1;
            new_b2[1] = currentPiece -> coord.current.b2[1] - 1;

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1];

            new_b4[0] = currentPiece -> coord.current.b4[0] - 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] + 1;

            // Verify if can be rotated
            if ( new_b4[1] < boardHeight &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b2[0] ][ new_b2[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    | b1 |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    | b2 |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * |    |    | b3 |    |        | b1 | b2 | b3 | b4 |
             * ---------------------        ---------------------
             * |    |    | b4 |    |        |    |    |    |    |
             * ---------------------        ---------------------
             */
        case 2:
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 2;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 2;

            new_b2[0] = currentPiece -> coord.current.b2[0] - 1;
            new_b2[1] = currentPiece -> coord.current.b2[1] + 1;

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1];

            new_b4[0] = currentPiece -> coord.current.b4[0] + 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] - 1;

            // Verify if can be rotated
            if ( new_b1[0] >= 0 && new_b4[0] < boardWidth &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b2[0] ][ new_b2[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
rotate_J (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        |    | b1 |    |    |
             * ---------------------  --->  ---------------------
             * | b1 | b2 | b3 |    |        |    | b2 |    |    |
             * ---------------------        --------------------- 
             * |    |    | b4 |    |        | b4 | b3 |    |    |
             * ---------------------        ---------------------
             */
        case 1:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 2;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b1 |    |    |        | b4 |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b2 |    |    |        | b3 | b2 | b1 |    |
             * ---------------------        --------------------- 
             * | b4 | b3 |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             */
        case 2:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 2;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( new_b1[0] < boardWidth &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * | b4 |    |    |    |        |    | b3 | b4 |    |
             * ---------------------  --->  ---------------------
             * | b3 | b2 | b1 |    |        |    | b2 |    |    |
             * ---------------------        --------------------- 
             * |    |    |    |    |        |    | b1 |    |    |
             * ---------------------        ---------------------
             */
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 2;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( new_b1[0] < boardHeight &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;


            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b3 | b4 |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b2 |    |    |        | b1 | b2 | b3 |    |
             * ---------------------        --------------------- 
             * |    | b1 |    |    |        |    |    | b4 |    |
             * ---------------------        ---------------------
             */
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0];
            new_b4[1] = currentPiece -> coord.current.b4[1] + 2;

            // Verify if can be rotated
            if ( new_b1[0] >= 0 &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
rotate_L (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        | b4 | b1 |    |    |
             * ---------------------  --->  ---------------------
             * | b1 | b2 | b3 |    |        |    | b2 |    |    |
             * ---------------------        --------------------- 
             * | b4 |    |    |    |        |    | b3 |    |    |
             * ---------------------        ---------------------
             */
        case 1:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0];
            new_b4[1] = currentPiece -> coord.current.b4[1] - 2;


            // Verify if can be rotated
            if ( *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * | b4 | b1 |    |    |        |    |    | b4 |    |
             * ---------------------  --->  ---------------------
             * |    | b2 |    |    |        | b3 | b2 | b1 |    |
             * ---------------------        --------------------- 
             * |    | b3 |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             */
        case 2:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 2;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( new_b1[0] < boardWidth &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    | b4 |    |        |    | b3 |    |    |
             * ---------------------  --->  ---------------------
             * | b3 | b2 | b1 |    |        |    | b2 |    |    |
             * ---------------------        --------------------- 
             * |    |    |    |    |        |    | b1 | b4 |    |
             * ---------------------        ---------------------
             */
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0];
            new_b4[1] = currentPiece -> coord.current.b4[1] + 2;

            // Verify if can be rotated
            if ( new_b1[0] < boardHeight &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;


            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b3 |    |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b2 |    |    |        | b1 | b2 | b3 |    |
             * ---------------------        --------------------- 
             * |    | b1 | b4 |    |        | b4 |    |    |    |
             * ---------------------        ---------------------
             */
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 2;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( new_b1[0] >= 0 &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
rotate_S (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        | b3 |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b1 | b2 |    |        | b4 | b1 |    |    |
             * ---------------------        --------------------- 
             * | b3 | b4 |    |    |        |    | b2 |    |    |
             * ---------------------        ---------------------
             */
        case 1:
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0];
            new_b1[1] = currentPiece -> coord.current.b1[1];

            new_b2[0] = currentPiece -> coord.current.b2[0] - 1;
            new_b2[1] = currentPiece -> coord.current.b2[1] + 1;

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1] - 2;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] - 1;

            // Verify if can be rotated
            if ( *gboard[ new_b3[0] ][ new_b3[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * | b3 |    |    |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * | b4 | b1 |    |    |        |    | b1 | b2 |    |
             * ---------------------        --------------------- 
             * |    | b2 |    |    |        | b3 | b4 |    |    |
             * ---------------------        ---------------------
             */
        case 2:
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0];
            new_b1[1] = currentPiece -> coord.current.b1[1];

            new_b2[0] = currentPiece -> coord.current.b2[0] + 1;
            new_b2[1] = currentPiece -> coord.current.b2[1] - 1;

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1] + 2;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] + 1;

            // Verify if can be rotated
            if ( new_b2[0] < boardWidth &&
                 *gboard[ new_b2[0] ][ new_b2[1] ] == 0 &&
                 *gboard[ new_b3[0] ][ new_b3[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
rotate_T (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        |    | b1 |    |    |
             * ---------------------  --->  ---------------------
             * | b1 | b2 | b3 |    |        | b4 | b2 |    |    |
             * ---------------------        --------------------- 
             * |    | b4 |    |    |        |    | b3 |    |    |
             * ---------------------        ---------------------
             */
        case 1:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] - 1;

            // Verify if can be rotated
            if ( *gboard[ new_b1[0] ][ new_b1[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b1 |    |    |        |    | b4 |    |    |
             * ---------------------  --->  ---------------------
             * | b4 | b2 |    |    |        | b3 | b2 | b1 |    |
             * ---------------------        --------------------- 
             * |    | b3 |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             */
        case 2:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] - 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] - 1;

            // Verify if can be rotated
            if ( new_b1[0] < boardWidth &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b4 |    |    |        |    | b3 |    |    |
             * ---------------------  --->  ---------------------
             * | b3 | b2 | b1 |    |        |    | b2 | b4 |    |
             * ---------------------        --------------------- 
             * |    |    |    |    |        |    | b1 |    |    |
             * ---------------------        ---------------------
             */
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] + 1;

            // Verify if can be rotated
            if ( new_b1[0] < boardHeight &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;


            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    | b3 |    |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b2 | b4 |    |        | b1 | b2 | b3 |    |
             * ---------------------        --------------------- 
             * |    | b1 |    |    |        |    | b4 |    |    |
             * ---------------------        ---------------------
             */
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 1;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0];
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0] + 1;
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 1;
            new_b4[1] = currentPiece -> coord.current.b4[1] + 1;

            // Verify if can be rotated
            if ( new_b1[0] >= 0 &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
rotate_Z (void)
{
    int new_b1[2], new_b2[2], new_b3[2], new_b4[2];

    switch ( currentPiece -> rotation.position )
    {
            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    | b1 |    |
             * ---------------------  --->  ---------------------
             * | b1 | b2 |    |    |        |    | b3 | b2 |    |
             * ---------------------        --------------------- 
             * |    | b3 | b4 |    |        |    | b4 |    |    |
             * ---------------------        ---------------------
             */
        case 1:
        case 3:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] + 2;
            new_b1[1] = currentPiece -> coord.current.b1[1] - 1;

            new_b2[0] = currentPiece -> coord.current.b2[0] + 1;
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1] - 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] - 1;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b2[0] ][ new_b2[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }
            break;

            /*
             * ---------------------        ---------------------
             * |    |    |    |    |        |    |    |    |    |
             * ---------------------        ---------------------
             * |    |    | b1 |    |        |    |    |    |    |
             * ---------------------  --->  ---------------------
             * |    | b3 | b2 |    |        | b1 | b2 |    |    |
             * ---------------------        --------------------- 
             * |    | b4 |    |    |        |    | b3 | b4 |    |
             * ---------------------        ---------------------
             */
        case 2:
        case 4:
            // Possible new coordinates
            new_b1[0] = currentPiece -> coord.current.b1[0] - 2;
            new_b1[1] = currentPiece -> coord.current.b1[1] + 1;

            new_b2[0] = currentPiece -> coord.current.b2[0] - 1;
            new_b2[1] = currentPiece -> coord.current.b2[1];

            new_b3[0] = currentPiece -> coord.current.b3[0];
            new_b3[1] = currentPiece -> coord.current.b3[1] + 1;

            new_b4[0] = currentPiece -> coord.current.b4[0] + 1;
            new_b4[1] = currentPiece -> coord.current.b4[1];

            // Verify if can be rotated
            if ( new_b1[0] >= 0 &&
                 *gboard[ new_b1[0] ][ new_b1[1] ] == 0 &&
                 *gboard[ new_b4[0] ][ new_b4[1] ] == 0 )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

        default:
            break;
    }
}

static void
shift (void)
{
    if ( gboard == NULL )
    {
        fputs("PIECE structure not initialized.", stderr);
        return;
    }

    if ( currentPiece -> shifting == LEFT &&
         currentPiece -> coord.current.b1[0] > 0 &&
         currentPiece -> coord.current.b2[0] > 0 &&
         currentPiece -> coord.current.b3[0] > 0 &&
         currentPiece -> coord.current.b4[0] > 0 )
    {
        currentPiece -> coord.current.b1[0] -= 1;
        currentPiece -> coord.current.b2[0] -= 1;
        currentPiece -> coord.current.b3[0] -= 1;
        currentPiece -> coord.current.b4[0] -= 1;
    }

    else if ( currentPiece -> shifting == RIGHT &&
              currentPiece -> coord.current.b1[0] < boardWidth &&
              currentPiece -> coord.current.b2[0] < boardWidth &&
              currentPiece -> coord.current.b3[0] < boardWidth &&
              currentPiece -> coord.current.b4[0] < boardWidth )
    {
        currentPiece -> coord.current.b1[0] += 1;
        currentPiece -> coord.current.b2[0] += 1;
        currentPiece -> coord.current.b3[0] += 1;
        currentPiece -> coord.current.b4[0] += 1;
    }

    currentPiece -> shifting = NONE;
}

static void
update_board (void)
{
    int board_new_b1, board_new_b2, board_new_b3, board_new_b4;

    if ( gboard == NULL )
    {
        fputs("PIECE structure not initialized.", stderr);
        return;
    }

    // Update piece position in board (after rotating or shifting)
    if ( currentPiece -> coord.old.b1[0] != INACTIVE_COORD )
    {
        // Clear old piece position
        *gboard[ currentPiece -> coord.old.b1[0] ][ \
                currentPiece -> coord.old.b1[1] ] = CLEAR;
        *gboard[ currentPiece -> coord.old.b2[0] ][ \
                currentPiece -> coord.old.b2[1] ] = CLEAR;
        *gboard[ currentPiece -> coord.old.b3[0] ][ \
                currentPiece -> coord.old.b3[1] ] = CLEAR;
        *gboard[ currentPiece -> coord.old.b4[0] ][ \
                currentPiece -> coord.old.b4[1] ] = CLEAR;

        // And write the new one
        *gboard[ currentPiece -> coord.current.b1[0] ][ \
            currentPiece -> coord.current.b1[1] ] = MOVING;
        *gboard[ currentPiece -> coord.current.b2[0] ][ \
            currentPiece -> coord.current.b2[1] ] = MOVING;
        *gboard[ currentPiece -> coord.current.b3[0] ][ \
            currentPiece -> coord.current.b3[1] ] = MOVING;
        *gboard[ currentPiece -> coord.current.b4[0] ][ \
            currentPiece -> coord.current.b4[1] ] = MOVING;

        currentPiece -> coord.old.b1[0] = INACTIVE_COORD;
    }

        // Move piece one position in Y axis
    else
    {
        board_new_b1 = *gboard[ currentPiece -> coord.current.b1[0] ][ \
            currentPiece -> coord.current.b1[1] + 1 ];

        board_new_b2 = *gboard[ currentPiece -> coord.current.b2[0] ][ \
            currentPiece -> coord.current.b2[1] + 1 ];

        board_new_b3 = *gboard[ currentPiece -> coord.current.b3[0] ][ \
            currentPiece -> coord.current.b3[1] + 1 ];

        board_new_b4 = *gboard[ currentPiece -> coord.current.b4[0] ][ \
            currentPiece -> coord.current.b4[1] + 1 ];

        if ( board_new_b1 != FIXED &&
             board_new_b2 != FIXED &&
             board_new_b3 != FIXED &&
             board_new_b4 != FIXED )
        {
            *gboard[currentPiece -> coord.current.b1[0] ][ \
                    currentPiece -> coord.current.b1[1]] = CLEAR;

            *gboard[currentPiece -> coord.current.b2[0] ][ \
                    currentPiece -> coord.current.b2[1]] = CLEAR;

            *gboard[currentPiece -> coord.current.b3[0] ][ \
                    currentPiece -> coord.current.b3[1]] = CLEAR;

            *gboard[currentPiece -> coord.current.b4[0] ][ \
                    currentPiece -> coord.current.b4[1]] = CLEAR;

            *gboard[currentPiece -> coord.current.b1[0] ][ \
                    currentPiece -> coord.current.b1[1] + 1] = MOVING;

            *gboard[currentPiece -> coord.current.b2[0] ][ \
                    currentPiece -> coord.current.b2[1] + 1] = MOVING;

            *gboard[currentPiece -> coord.current.b3[0] ][ \
                    currentPiece -> coord.current.b3[1] + 1] = MOVING;

            *gboard[currentPiece -> coord.current.b4[0] ][ \
                    currentPiece -> coord.current.b4[1] + 1] = MOVING;

            currentPiece -> coord.current.b1[1] += 1;
            currentPiece -> coord.current.b2[1] += 1;
            currentPiece -> coord.current.b3[1] += 1;
            currentPiece -> coord.current.b4[1] += 1;
        }

        else
        {
            *gboard[currentPiece -> coord.current.b1[0] ][ \
                    currentPiece -> coord.current.b1[1] ] = FIXED;

            *gboard[currentPiece -> coord.current.b2[0] ][ \
                    currentPiece -> coord.current.b2[1] ] = FIXED;

            *gboard[currentPiece -> coord.current.b3[0] ][ \
                    currentPiece -> coord.current.b3[1] ] = FIXED;

            *gboard[currentPiece -> coord.current.b4[0] ][ \
                    currentPiece -> coord.current.b4[1] ] = FIXED;

            currentPiece -> coord.current.b1[0] = INACTIVE_COORD;
        }
    }
}

static void
update_coordinates (int c1[2], int c2[2], int c3[2], int c4[2])
{

    // X coordinates
    currentPiece -> coord.old.b1[0] = currentPiece -> coord.current.b1[0];
    currentPiece -> coord.old.b2[0] = currentPiece -> coord.current.b2[0];
    currentPiece -> coord.old.b3[0] = currentPiece -> coord.current.b3[0];
    currentPiece -> coord.old.b4[0] = currentPiece -> coord.current.b4[0];

    // Y coordinates
    currentPiece -> coord.old.b1[1] = currentPiece -> coord.current.b1[1];
    currentPiece -> coord.old.b2[1] = currentPiece -> coord.current.b2[1];
    currentPiece -> coord.old.b3[1] = currentPiece -> coord.current.b3[1];
    currentPiece -> coord.old.b4[1] = currentPiece -> coord.current.b4[1];

    // X coordinates
    currentPiece -> coord.current.b1[0] = c1[0];
    currentPiece -> coord.current.b2[0] = c2[0];
    currentPiece -> coord.current.b3[0] = c3[0];
    currentPiece -> coord.current.b4[0] = c4[0];

    // Y coordinates
    currentPiece -> coord.current.b1[1] = c1[1];
    currentPiece -> coord.current.b2[1] = c2[1];
    currentPiece -> coord.current.b3[1] = c3[1];
    currentPiece -> coord.current.b4[1] = c4[1];
}

static void
update_rotation (void)
{
    if ( currentPiece -> rotation.position < 4 )
    {
        currentPiece -> rotation.position += 1;
    }

    else
    {
        currentPiece -> rotation.position = 1;
    }

    currentPiece -> rotation.status = false;
}
