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
 * @file    board.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 23:02
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "random_generator.h"

#include "pieces/piece_actions.h"

// This file
#include "board.h"

/// @privatesection
// === Constants and Macro definitions ===
// Board Height for the matrix. The additional 3 rows are hidden to the user
#define MBOARD_H     BOARD_HEIGHT + 3
// Board Width for the matrix.
#define MBOARD_W     BOARD_WIDTH

// === Enumerations, structures and typedefs ===

typedef struct GAMEBOARD_PRIVATE
{
    board_t * public = NULL;

    int gboard[MBOARD_H][MBOARD_W];

    piece_t piece;

    int bag[TETROMINOS];

    int bagPosition;

    int lastTetromino;

} board_private_t;
// === Global variables ===

// === Function prototypes for private functions with file level scope ===

static int *
askBoard (void);

static int
init (int * bag, int size);

// Clear all cells with moving pieces on the board
static void
clearMoving (void);

// Set the piece's coordinates as CELL_FIXED on the board
static void
setFixed (int cellType);

// Set the piece's coordinates as CELL_MOVING on the board
static void
setMoving (void);

// Updates the board accoding the moving piece actions.
static int
updateBoard (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static board_private_t bStruct;

//static stats_t stats;

// === Global function definitions ===
/// @publicsection

/*
 * TODO Complete the actions on this file
 */

/**
 * @brief Initialize a GAMEBOARD structure
 * 
 * @param gameBoardStruct
 */
void
board_init (void * gameBoardStruct)
{
    //int pos;


    if ( gameBoardStruct == NULL )
    {
        fputs("Invalid argument.", stderr);
        fputs("A GAMEBOARD struct must be passed by reference.", stderr);
    }

    else
    {
        bStruct.public = (board_t *) gameBoardStruct;
    }

    init();

    /*while ( 1 )
    {
        //

        // Empty bag? New set of pieces


        // Change stats

    }*/
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Initializes a new board.
 * 
 * Should be called once at the beggning of a new game.
 * 
 * Calls the random_generator() function to create a new bag of pieces, sets a
 * new piece as PIECE_NONE (it will be later initialized by updateBoard() ) and
 * clears stats.
 * 
 * @param bag Bag where the new set of pieces will be saved
 * @param size Bag's size
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */

static int
init (void)
{
    if ( init_random_generator() )
    {
        return 1;
    }

    // Generate first set of pieces
    bStruct.bagPosition = 0;
    fillBag();

    // Piece to PIECE_NONE
    bStruct.piece.type = TETROMINO_NONE;

    //bStruct -> destroy = ;

    //bStruct -> ask.FPG =;
    bStruct.public -> ask.board = &askBoard;

    //bStruct -> clear.line = &;

    /*bStruct.public -> piece.clear.moving = &clearMoving;

    bStruct.public -> piece.set.moving = &setMoving;
    bStruct.public -> piece.set.fixed = &setFixed;*/

    bStruct.public -> piece.action.rotate = &rotatePiece;
    bStruct.public -> piece.action.shift = &shiftPiece;
    bStruct.public -> piece.action.softDrop = &softDropPiece;

    // Stats
    /*
    stats.score.actual = 0;
    stats.next = PIECE_NONE;
    stats.pieces.I = 0;
    stats.pieces.J = 0;
    stats.pieces.L = 0;
    stats.pieces.O = 0;
    stats.pieces.S = 0;
    stats.pieces.T = 0;
    stats.pieces.Z = 0;

    stats.status = 0;

    /// FIXME
    stats.score.top = 1000;
    stats.level = 0;
    stats.lines = 0;
     */

    return 0;
}

static int *
askBoard (void)
{
    return &bStruct.gboard[3][0];
}

static int
endGame (void)
{
    int i, j;
    int ans = 0;

    for ( i = 0; i < MBOARD_H - BOARD_HEIGHT; i++ )
    {
        for ( j = 0; j < MBOARD_W; j++ )
        {
            bStruct.gboard[i][j] > CELL_CLEAR ? ans = 1 : 0;
        }
    }

    return ans;
}

static void
fillBag (void)
{
    (bStruct.bagPosition == TETROMINOS - 2) ? \
            bStruct.lastTetromino = bStruct.bag[TETROMINOS - 1] : 0;

    // Generate set of pieces
    random_generator(bStruct.bag, size);

    bStruct.bagPosition = 0;
    bStruct.lastTetromino = bStruct.bag[TETROMINOS - 1];
}

static int
filledLines (int arr[BOARD_HEIGHT])
{
    int i, j;

    int fixed, nFill;

    for ( i = MBOARD_H - BOARD_HEIGHT, nFill = 0; i < MBOARD_H; i++ )
    {

        if ( bStruct.gboard[i][0] > CELL_CLEAR )
        {

            for ( j = 1, fixed = 1; j < MBOARD_W && fixed > 0; j++ )
            {
                bStruct.gboard[i][j] > CELL_CLEAR ? fixed++ : fixed = 0;
            }

            fixed > 0 ? (arr[nFill++] = i - (MBOARD_H - BOARD_HEIGHT)) : 0;
        }
    }

    return nFill;
}

/**
 * @brief Updates the board accoding the moving piece actions.
 * 
 * If the current piece isn't initialized, calls piece_init(), otherwise, 
 * follows the user input, rotating, shifting or dropping the piece.
 * 
 * @return 
 */
static int
updateBoard (void)
{
    int cellType;

    if ( bStruct.piece.type == TETROMINO_NONE )
    {
        switch ( bStruct.bagPosition )
        {

            case TETROMINOS - 2:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                fillBag();
                break;

            case TETROMINOS - 1:
                bStruct.piece.type = bStruct.lastTetromino;
                break;

            default:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                break;
        }

        piece_init(&bStruct.piece, bStruct.public,
                   &bStruct.gboard[0][0], MBOARD_H, MBOARD_W,
                   bStruct.piece.type);

    }

    bStruct.piece.update(&cellType);

    clearMoving();

    if ( cellType == CELL_MOVING )
    {

        setMoving();
    }

    else
    {
        setFixed(cellType);
    }

    return 0;
}

static void
rotatePiece (int direction)
{
    if ( direction == LEFT || direction == RIGHT )
    {
        bStruct.piece.rotate(direction);

        // Force update
        updateBoard();
    }
}

static void
shiftPiece (int direction)
{
    if ( direction == LEFT || direction == RIGHT )
    {
        bStruct.piece.shift(direction);

        // Force update
        updateBoard();
    }
}

static void
softDropPiece (void)
{
    bStruct.piece.softDrop();

    // Force update
    updateBoard();
}

/**
 * @brief Clear all cells with moving pieces on the board
 *  
 * @param None
 * 
 * @return Nothing
 */
static void
clearMoving (void)
{
    int i, j;

    for ( i = 0; i < MBOARD_H; i++ )
    {
        for ( j = 0; j < MBOARD_W; j++ )
        {
            if ( bStruct.gboard[i][j] == CELL_MOVING )
            {
                bStruct.gboard[i][j] = CELL_CLEAR;
            }
        }
    }
}

/**
 * @brief Set the piece's coordinates as CELL_MOVING on the board
 *  
 * @param None
 * 
 * @return Nothing
 */
static void
setMoving (void)
{
    bStruct.gboard[ bStruct.piece.get.coordinates[b1][COORD_Y] ][ \
                    bStruct.piece.get.coordinates[b1][COORD_X] ] = CELL_MOVING;

    bStruct.gboard[ bStruct.piece.get.coordinates[b2][COORD_Y] ][ \
                    bStruct.piece.get.coordinates[b2][COORD_X] ] = CELL_MOVING;

    bStruct.gboard[ bStruct.piece.get.coordinates[b3][COORD_Y] ][ \
                    bStruct.piece.get.coordinates[b3][COORD_X] ] = CELL_MOVING;

    bStruct.gboard[ bStruct.piece.get.coordinates[b4][COORD_Y] ][ \
                    bStruct.piece.get.coordinates[b4][COORD_X] ] = CELL_MOVING;
}

/**
 * @brief Set the piece's coordinates as fixed (CELL_X) on the board
 *  
 * @param cellType Block that's going to be fixed. (i.e. TETROMINO_J will set
 * the four cells as CELL_J)
 * 
 * @return Nothing
 */
static void
setFixed (int cellType)
{
    if ( cellType >= CELL_I )
    {
        bStruct.gboard[ bStruct.piece.get.coordinates[b1][COORD_Y] ][ \
                       bStruct.piece.get.coordinates[b1][COORD_X] ] = cellType;

        bStruct.gboard[ bStruct.piece.get.coordinates[b2][COORD_Y] ][ \
                       bStruct.piece.get.coordinates[b2][COORD_X] ] = cellType;

        bStruct.gboard[ bStruct.piece.get.coordinates[b3][COORD_Y] ][ \
                       bStruct.piece.get.coordinates[b3][COORD_X] ] = cellType;

        bStruct.gboard[ bStruct.piece.get.coordinates[b4][COORD_Y] ][ \
                       bStruct.piece.get.coordinates[b4][COORD_X] ] = cellType;
    }

    else
    {
        fputs("Invalid cellType for setFixed()", stderr);
    }
}
