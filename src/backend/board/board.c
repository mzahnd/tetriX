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

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

static int *
askBoard (void);

static int
init (char * bag, int size);

// Clear all cells with moving pieces on the board
static void
clearMoving (void);

// Set the piece's coordinates as CELL_FIXED on the board
static void
setFixed (void);

// Set the piece's coordinates as CELL_MOVING on the board
static void
setMoving (void);

// Updates the board accoding the moving piece actions.
static int
updateBoard (char * bag, int position);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static board_t * bStruct = NULL;

static int gboard[MBOARD_H][MBOARD_W];

static piece_t piece;

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
    char bag[NUM_PIECES]; //, tmp_bag[NUM_PIECES];

    if ( gameBoardStruct == NULL )
    {
        fputs("Invalid argument.", stderr);
        fputs("A GAMEBOARD struct must be passed by reference", stderr);
    }

    else
    {
        bStruct = (board_t *) gameBoardStruct;
    }

    init(bag, NUM_PIECES);

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
init (char * bag, int size)
{
    if ( init_random_generator() )
    {
        return 1;
    }

    // Generate first set of pieces
    random_generator(bag, size);

    // Piece to PIECE_NONE
    piece.type = PIECE_NONE;

    //bStruct -> destroy = ;
    //bStruct -> ask.FPG =;
    bStruct -> ask.board = &askBoard;
    //bStruct -> clear.line = &;

    bStruct -> piece.clear.moving = &clearMoving;
    bStruct -> piece.set.moving = &setMoving;
    bStruct -> piece.set.fixed = &setFixed;

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
    return &gboard[3][0];
}

/**
 * @brief Updates the board accoding the moving piece actions.
 * 
 * If the current piece isn't initialized, calls piece_init(), otherwise, 
 * follows the user input, rotating, shifting or dropping the piece.
 * 
 * @param bag
 * @param position
 * @return 
 */
static int
updateBoard (char * bag, int position)
{
    // Piece not initialized
    if ( piece.type == PIECE_NONE )
    {
        piece_init(&piece, bag, position);
    }

    // User asked to rotate the piece
    if ( piece.rotation.status == true )
    {
        piece.rotate(gboard, MBOARD_H);
    }

    // User asked soft drop
    if ( piece.drop == true )
    {
        // Update the piece in the board
        piece.update(bStruct, gboard, MBOARD_H);

        // FIXME Add some timer here (FPG dependent)

        piece.drop = false;
    }

    // User asked to shift the piece (either left or right)
    /*if ( piece.shift != NONE )
    {
        piece.shift(gboard, BOARD_HEIGHT);
    }*/

    // Update the piece in the board
    piece.update(bStruct, gboard, MBOARD_H);

    return 0;
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
            if ( gboard[i][j] == CELL_MOVING )
            {
                gboard[i][j] = CELL_CLEAR;
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
    gboard[ piece.coord.b1[COORD_Y] ][ \
                        piece.coord.b1[COORD_X] ] = CELL_MOVING;

    gboard[ piece.coord.b2[COORD_Y] ][ \
                        piece.coord.b2[COORD_X] ] = CELL_MOVING;

    gboard[ piece.coord.b3[COORD_Y] ][ \
                        piece.coord.b3[COORD_X] ] = CELL_MOVING;

    gboard[ piece.coord.b4[COORD_Y] ][ \
                        piece.coord.b4[COORD_X] ] = CELL_MOVING;
}

/**
 * @brief Set the piece's coordinates as CELL_FIXED on the board
 *  
 * @param None
 * 
 * @return Nothing
 */
static void
setFixed (void)
{
    gboard[ piece.coord.b1[COORD_Y] ][ \
                        piece.coord.b1[COORD_X] ] = CELL_FIXED;

    gboard[ piece.coord.b2[COORD_Y] ][ \
                        piece.coord.b2[COORD_X] ] = CELL_FIXED;

    gboard[ piece.coord.b3[COORD_Y] ][ \
                        piece.coord.b3[COORD_X] ] = CELL_FIXED;

    gboard[ piece.coord.b4[COORD_Y] ][ \
                        piece.coord.b4[COORD_X] ] = CELL_FIXED;
}
