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

// To generate random pieces
#include "random_generator.h"

// For PIECE
#include "pieces/piece_actions.h"

// For STATS
#include "../stats/stats_mgmt.h"

// For initTimer
#include "timer/boardTimer.h"

// This file
#include "board.h"

/// @privatesection
// === Constants and Macro definitions ===

/**
 * @def HIDDEN_ROWS
 * @brief Rows hidden to the user which will be part of the board
 */
#define HIDDEN_ROWS     3

/**
 * @def MBOARD_H
 * @brief Board Height for the matrix plus the 3 rows hidden to the user
 */
#define MBOARD_H        (BOARD_HEIGHT + HIDDEN_ROWS)

/**
 * @def MBOARD_W
 * @brief Board Width for the matrix.
 */
#define MBOARD_W        (BOARD_WIDTH)

#define INVALID_LINE    -1

/**
 * @def CELL(r,c)
 * @brief Returns the information in the row (r) and column (c) of the board
 */
#define CELL(r,c) ( bStruct.gboard[ ((r) * MBOARD_W) + (c) ] )

/**
 * @def CELL_ADDRESS(r,c)
 * @brief Returns the memory address in the row (r) and column (c) of the board
 */
#define CELL_ADDRESS(r,c) ( &bStruct.gboard[ ((r) * MBOARD_W) + (c) ] )

// === Enumerations, structures and typedefs ===

enum statsUpdates
{
    SOFT,
    NPIECE
};

/**
 * @brief Private variables of the GAMEBOARD structure.
 * 
 * This function has the full board, a PIECE structure to which it constantly 
 * calls and the bag of pieces that are being used in the game.
 * 
 */
typedef struct GAMEBOARD_PRIVATE
{
    /// Public GAMEBOARD structure to which functions address is written
    board_t * public;

    /// Full game board
    grid_t * gboard;

    /// PIECE structure with the functions needed to modify it
    piece_t piece;

    stats_t stats;

    /// Bag of pieces
    int bag[TETROMINOS];

    /// Piece that's being readed from the bag
    int bagPosition;

    /**
     * @brief Last Tetromino of the bag.
     * 
     * Allows creating another bag while using the last tetromino of the 
     * current one. This way, the "next piece" can be always shown, even if the
     * piece in use is the last one of a given bag.
     */
    int lastTetromino;

} board_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Returns the point (0,0) of the public board
static int *
askBoard(void);

// Returns a constant pointer to the current STATS structure.
static const void *
askStats(void);

// Clear a filled row.
static void
clearLine(int lines[BOARD_HEIGHT], int position);

// Clear all cells with moving pieces on the board
static void
clearMoving(void);

// Destroy current board, erasing all the structure's information
static void
destroy(void);

// Tells if the current game should be finished.
static int
endGame(void);

// Fills the bag of pieces
static void
fillBag(void);

// How many rows are complete and which are those.
static int
filledRows(int lines[BOARD_HEIGHT]);

// Initializes a new board.
static int
init(void);

// Rotate the piece in the given direction
static void
rotatePiece(int direction);

// Set the piece's coordinates as CELL_I, ..., CELL_Z in the board
static void
setFixed(int cellType);

// Set the piece's coordinates as CELL_MOVING on the board
static void
setMoving(void);

// Shift the piece in the given direction
static void
shiftPiece(int direction);

// Perform a soft drop of the piece
static void
softDropPiece(void);

// Updates the board accoding the moving piece actions.
static int
updateBoard(int cellType);

// Updates the piece in the board
static void
updatePiece(void);

// @brief Update STATS structure using an action from statsUpdate enum
static void
updateStats(int action);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

/// Board structure used in every function
static board_private_t bStruct;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize a GAMEBOARD structure
 * 
 * @param gameBoardStruct GAMEBOARD structure to initialize
 */
void
board_init(void * gameBoardStruct)
{
    if(gameBoardStruct == NULL)
    {
        fputs("Invalid argument.", stderr);
        fputs("A GAMEBOARD struct must be passed by reference.", stderr);
    }

    else
    {
        bStruct.public = (board_t *) gameBoardStruct;
        if(init())
        {
            fputs("Board could not be initialized. Drestroying", stderr);
            // Avoid a possible error when trying to destroy a non-initialized
            // piece
            bStruct.piece.destroy = NULL;
            // Destroy the board
            destroy();
        }
    }
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Returns the point (0,0) of the public board
 * 
 * @param None
 * 
 * @return Pointer to the first non-hidden row and column 0 (top-left public 
 * corner) of the board
 */
static grid_t *
askBoard(void)
{
    return CELL_ADDRESS(HIDDEN_ROWS, 0);
}

/**
 * @brief Returns a constant pointer to the current STATS structure.
 * 
 * @param None
 * 
 * @return Pointer to the current game' STATS structure
 */
static const void *
askStats(void)
{
    return &bStruct.stats;
}

/**
 * @brief Clear a filled row.
 * 
 * Clears the given row and drops the board "as it is" once.
 * 
 * @param lines Array with all the filled rows.
 * @param position Position of the row to be cleared in @p lines array.
 * 
 * @return Nothing
 */
static void
clearLine(int lines[BOARD_HEIGHT], int position)
{
    int i, j;

    // Add hidden rows
    int line = lines[position] + HIDDEN_ROWS;

    // Goes from bottom to top starting in the filled line
    for(i = line; i > 0; i--)
    {
        for(j = 0; j < MBOARD_W; j++)
        {
            // If there's a fixed piece or a CELL_CLEAR, copy it
            if(CELL(i - 1, j) > CELL_MOVING &&
               CELL(i, j) > CELL_MOVING)
            {
                CELL(i, j) = CELL(i - 1, j);
            }

                // If the previous grid cell has a CELL_MOVING but not the one it's
                // meant to be filled, ignore the piece and leave the cell as 
                // CELL_CLEAR
            else if(CELL(i - 1, j) == CELL_MOVING &&
                    CELL(i, j) > CELL_MOVING)
            {

                CELL(i, j) = CELL_CLEAR;
            }
        }
    }

    // Update the rest of the lines in the array
    lines[position] = INVALID_LINE;
    line -= HIDDEN_ROWS;

    // Update the upper rows number of the one that's been cleared
    for(i = 0; i < BOARD_HEIGHT; i++)
    {
        (lines[i] != INVALID_LINE && lines[i] < line) ? (lines[i]++) : 0;
    }
}

/**
 * @brief Clear all cells with moving pieces on the board
 *  
 * @param None
 * 
 * @return Nothing
 */
static void
clearMoving(void)
{
    int i, j;

    for(i = 0; i < MBOARD_H; i++)
    {
        for(j = 0; j < MBOARD_W; j++)
        {
            if(CELL(i, j) == CELL_MOVING)
            {

                CELL(i, j) = CELL_CLEAR;
            }
        }
    }
}

/**
 * @brief Destroy current board, erasing all the structure's information
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
destroy(void)
{
    int i;

    // Free game board memory
    free(bStruct.gboard);
    bStruct.gboard = NULL;

    // Clear bag
    for(i = 0; i < TETROMINOS; i++)
    {
        bStruct.bag[i] = TETROMINO_NONE;
    }

    bStruct.bagPosition = 0;
    bStruct.lastTetromino = TETROMINO_NONE;

    // Destroy the piece if it hasn't been already
    if(bStruct.piece.destroy != NULL)
    {
        bStruct.piece.destroy();
        bStruct.piece.destroy = NULL;
    }

    //
    if(bStruct.public -> ask.stats != NULL)
    {
        bStruct.stats.destroy();
        bStruct.public -> ask.stats = NULL;
    }

    // Clear function pointers
    bStruct.public -> ask.FPG = NULL;
    bStruct.public -> ask.board = NULL;
    bStruct.public -> ask.filledRows = NULL;
    bStruct.public -> ask.endGame = NULL;

    bStruct.public -> clear.line = NULL;

    bStruct.public -> piece.rotate = NULL;
    bStruct.public -> piece.shift = NULL;
    bStruct.public -> piece.softDrop = NULL;

    bStruct.public -> update = NULL;

    bStruct.public -> destroy = NULL;
    bStruct.public = NULL;
}

/**
 * @brief Tells if the current game should be finished.
 * 
 * @return True: 1
 * @return False: 0
 */
static int
endGame(void)
{
    int i, j;
    int ans = 0;

    for(i = 0; i < HIDDEN_ROWS; i++)
    {
        for(j = 0; j < MBOARD_W; j++)
        {

            CELL(i, j) > CELL_CLEAR ? ans = 1 : 0;
        }
    }

    return ans;
}

/**
 * @brief Fills the bag of pieces
 * 
 * When called the first time, bagPosition variable in the structure is set to
 * 0. From then on, every time it's called, bagPosition is left as it is and
 * lastTetromino variable is updated.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
fillBag(void)
{
    if(bStruct.bagPosition == TETROMINOS - 1)
    {
        bStruct.lastTetromino = bStruct.bag[TETROMINOS - 1];
    }
    else
    {

        bStruct.bagPosition = 0;
    }

    // Generate set of pieces
    random_generator(bStruct.bag, TETROMINOS);
}

/**
 * @brief How many rows are complete and which are those.
 * 
 * @param lines Array with the number of the rows that are filled.
 * 
 * @return Number of filled rows
 */
static int
filledRows(int lines[BOARD_HEIGHT])
{
    int i, j;

    int fixed, nFill;

    // Clear the array
    for(i = 0; i < BOARD_HEIGHT; i++)
    {
        lines[i] = INVALID_LINE;
    }

    // The whole board is readed looking for the lines
    for(i = HIDDEN_ROWS, nFill = 0; i < MBOARD_H; i++)
    {
        // To make it faster, only first and last column of each row are 
        // checked
        if(CELL(i, 0) > CELL_CLEAR)
        {
            // And when both columns have a fixed block, the rest of the 
            // columns are checked
            for(j = 1, fixed = 2;
                j < MBOARD_W - 1 && fixed > 0;
                j++)
            {
                // Every grid cell must have a fixed block
                (CELL(i, j) > CELL_CLEAR) ? (fixed++) : (fixed = 0);
            }

            (fixed > 0) ? (lines[nFill++] = i - HIDDEN_ROWS) : 0;
        }
    }

    bStruct.stats.update(nFill);

    // Number of filled rows
    return nFill;
}

/**
 * @brief Initializes a new board.
 * 
 * Should be called once at the beggning of a new game.
 * 
 * Calls the random_generator() function to create a new bag of pieces, sets a
 * new piece as PIECE_NONE (it will be later initialized by updatePiece() ) and
 * clears stats.
 * 
 * @param None
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
static int
init(void)
{
    if(init_random_generator())
    {
        return 1;
    }

    // Allocate board
    bStruct.gboard = (grid_t *) calloc(MBOARD_W * MBOARD_H, sizeof (grid_t));

    if(bStruct.gboard == NULL)
    {
        fputs("Could not allocate memory for the board.", stderr);
        return 1;
    }

    if(initStats(&bStruct.stats))
    {
        fputs("Could initialize STATS.", stderr);
        return 1;
    }

    bStruct.public -> ask.stats = &askStats;

    // Generate first set of pieces
    bStruct.bagPosition = 0;
    fillBag();

    // Piece to PIECE_NONE
    bStruct.piece.type = TETROMINO_NONE;

    bStruct.public -> update = &updatePiece;
    bStruct.public -> destroy = &destroy;

    //bStruct.public -> ask.FPG =;
    bStruct.public -> ask.board = &askBoard;
    bStruct.public -> ask.filledRows = &filledRows;
    bStruct.public -> ask.endGame = &endGame;

    bStruct.public -> clear.line = &clearLine;

    // Piece functions
    bStruct.public -> piece.rotate = &rotatePiece;
    bStruct.public -> piece.shift = &shiftPiece;
    bStruct.public -> piece.softDrop = &softDropPiece;

    initTimer(&bStruct.stats);

    return 0;
}

/**
 * @brief Rotate the piece in the given direction
 * 
 * Automatically performs a board update but does not fixes the piece
 * if the ratation cannot be performed.
 * 
 * @param direction LEFT or RIGHT, according to shiftingTypes enum
 * 
 * @return Nothing
 */
static void
rotatePiece(int direction)
{
    if(direction == LEFT || direction == RIGHT)
    {
        // Rotate piece
        bStruct.piece.rotate(direction);

        // Force update
        updateBoard(CELL_MOVING);
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
setMoving(void)
{
    // Block b1
    CELL(bStruct.piece.get.coordinates[b1][COORD_Y], \
        bStruct.piece.get.coordinates[b1][COORD_X]) = CELL_MOVING;

    // Block b2
    CELL(bStruct.piece.get.coordinates[b2][COORD_Y], \
        bStruct.piece.get.coordinates[b2][COORD_X]) = CELL_MOVING;

    // Block b3
    CELL(bStruct.piece.get.coordinates[b3][COORD_Y], \
        bStruct.piece.get.coordinates[b3][COORD_X]) = CELL_MOVING;

    // Block b4
    CELL(bStruct.piece.get.coordinates[b4][COORD_Y], \
        bStruct.piece.get.coordinates[b4][COORD_X]) = CELL_MOVING;
}

/**
 * @brief Set the piece's coordinates as CELL_I, ..., CELL_Z in the board
 *  
 * @param cellType Block that's going to be fixed. (i.e. TETROMINO_J will set
 * the four cells as CELL_J)
 * 
 * @return Nothing
 */
static void
setFixed(int cellType)
{
    if(cellType >= CELL_I)
    {
        // Block b1
        CELL(bStruct.piece.get.coordinates[b1][COORD_Y], \
            bStruct.piece.get.coordinates[b1][COORD_X]) = cellType;

        // Block b2
        CELL(bStruct.piece.get.coordinates[b2][COORD_Y], \
            bStruct.piece.get.coordinates[b2][COORD_X]) = cellType;

        // Block b3
        CELL(bStruct.piece.get.coordinates[b3][COORD_Y], \
            bStruct.piece.get.coordinates[b3][COORD_X]) = cellType;

        // Block b4
        CELL(bStruct.piece.get.coordinates[b4][COORD_Y], \
            bStruct.piece.get.coordinates[b4][COORD_X]) = cellType;
    }

    else
    {
        fputs("Invalid cellType for setFixed()", stderr);
    }
}

/**
 * @brief Shift the piece in the given direction
 *
 * Automatically performs a board update but does not fixes the piece
 * if the shifting cannot be performed.
 * 
 * @param direction LEFT or RIGHT, according to shiftingTypes enum
 * 
 * @return Nothing
 */
static void
shiftPiece(int direction)
{
    if(direction == LEFT || direction == RIGHT)
    {
        // Shift piece
        bStruct.piece.shift(direction);

        // Force update
        updateBoard(CELL_MOVING);
    }
}

/**
 * @brief Perform a soft drop of the piece
 * 
 * Automatically performs a board update.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
softDropPiece(void)
{
    // Soft drop (calls board update)
    bStruct.piece.softDrop();

    // Update stats
    updateStats(SOFT);
}

/**
 * @brief Updates the board accoding the moving piece actions.
 * 
 * If the current piece isn't initialized, calls piece_init(), otherwise, 
 * follows the user input, rotating, shifting or dropping the piece.
 * 
 * @param cellType Blocks type of the piece. Could be CELL_MOVING or >= CELL_I
 * if the piece should be moving or fixed
 * 
 * @return Noting
 */
static int
updateBoard(int cellType)
{
    // Clear moving pieces
    clearMoving();

    // Set the piece as moving or as fixed
    if(cellType == CELL_MOVING)
    {
        setMoving();
    }

    else
    {
        // Fix it and destroy the piece structure
        setFixed(cellType);
        bStruct.piece.destroy();
    }

    return 0;
}

/**
 * @brief Updates the piece in the board
 * 
 * Initializes a new piece from the bag if necessary and, when the bag is 
 * almost empty, creates a new one.
 * If the piece already exists, updates its position (drops it once) and the 
 * board.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
updatePiece(void)
{
    int cellType;

    // No piece is set
    if(bStruct.piece.type == TETROMINO_NONE)
    {
        switch(bStruct.bagPosition)
        {
                // Bag is almost empty
            case TETROMINOS - 2:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                fillBag();

                break;

                // Last piece in the bag
            case TETROMINOS - 1:
                bStruct.piece.type = bStruct.lastTetromino;
                break;

                // More than 2 pieces left in the bag
            default:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                break;
        }

        // Update piece type in stats
        updateStats(NPIECE);

        // Initialize the piece
        piece_init(&bStruct.piece, bStruct.public,
                   CELL_ADDRESS(0, 0), MBOARD_H, MBOARD_W,
                   bStruct.piece.type);

    }

    // Get the cellType
    cellType = bStruct.piece.update();

    // And update the board
    updateBoard(cellType);
}

/**
 * @brief Update STATS structure using an action from statsUpdate enum
 * 
 * Function used to update the stats when a soft drop is performed or a new
 * piece is generated.
 * 
 * @param action
 * 
 * @return Nothing
 */
static void
updateStats(int action)
{
    switch(action)
    {
        case SOFT:
            bStruct.stats.softDrop();
            break;

        case NPIECE:
            bStruct.stats.newPiece(&bStruct.piece.type,
                                   &bStruct.bag[bStruct.bagPosition]);
            break;

        default:
            break;
    }
}
