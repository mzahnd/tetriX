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
 * @brief   Game logic.
 * 
 * @details The whole game logic can (and should be) controlled from here.
 * 
 * Board creation and reading, stats reading (uses stats_mgmt.h), line clearing
 * and pieces shifting and rotation are controlled using this file.
 * 
 * Macros for getting what is in a particular grid, as well as the different
 * types of tetrominos available and their code when are fixed and the board 
 * size are also defined here.
 * 
 * At the beggining of a new game board_init() must be called with a pointer to
 * the structure board_t that will be used during the whole game. After 
 * ending the game (or displaying the top score), the board_t must be destroyed
 * to avoid unexpected behaviour.
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

// To generate random pieces and rows
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
#define INVALID_PIECE   -1

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

/// Update stats because a ... was performed or there is a new piece

enum statsUpdates
{
    /// Soft drop
    SOFT,
    /// New piece
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
askBoard (void);

// Returns a constant pointer to the current STATS structure.
static const void *
askStats (void);

// Clear a filled row.
static void
clearLine (int lines[BOARD_HEIGHT], int position);

// Clear all cells with moving pieces on the board
static void
clearMoving (void);

// Destroy current board, erasing all the structure's information
static void
destroy (void);

// Tells if the current game should be finished.
static int
endGame (void);

// Fills the bag of pieces
static void
fillBag (void);

// How many rows are complete and which are those.
static int
filledRows (int lines[BOARD_HEIGHT]);

// Game mode to be played
//void gameMode (int mode);

// Initializes a new board.
static int
init (void);

// Tells if there's a moving piece currently in the board
static int
movingPieceInBoard (void);

// Rotate the piece in the given direction
static void
rotatePiece (int direction);

// Set the piece's coordinates as CELL_I, ..., CELL_Z in the board
static void
setFixed (int cellType);

// Set the piece's coordinates as CELL_MOVING on the board
static void
setMoving (void);

// Shift the piece in the given direction
static void
shiftPiece (int direction);

// Perform a soft drop of the piece
static void
softDropPiece (void);

// Level in which the game starts
static void
startLevel (unsigned char n);

// Amount of rows to start in the board
static void
startRows (unsigned char n);

// Updates the board accoding the moving piece actions.
static int
updateBoard (int cellType);

// Updates the piece in the board
static void
updatePiece (void);

// @brief Update STATS structure using an action from statsUpdate enum
static void
updateStats (int action);

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
board_init (void * gameBoardStruct)
{
    if ( gameBoardStruct == NULL )
    {
        fputs("Invalid argument.", stderr);
        fputs("A GAMEBOARD struct must be passed by reference.", stderr);
    }

    else
    {
        bStruct.public = (board_t *) gameBoardStruct;

        // Hold as uninitialized until init() finishes
        bStruct.public -> init = false;

        if ( !init() )
        {
            // Properly initialized
            bStruct.public -> init = true;
        }
        else
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
askBoard (void)
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
askStats (void)
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
clearLine (int lines[BOARD_HEIGHT], int position)
{
    int i, j;

    // Add hidden rows
    int line = lines[position] + HIDDEN_ROWS;

    // Goes from bottom to top starting in the filled line
    for ( i = line; i > 0; i-- )
    {
        for ( j = 0; j < MBOARD_W; j++ )
        {
            // If there's a fixed piece or a CELL_CLEAR, copy it
            if ( CELL(i - 1, j) > CELL_MOVING &&
                 CELL(i, j) > CELL_MOVING )
            {
                CELL(i, j) = CELL(i - 1, j);
            }

                // If the previous grid cell has a CELL_MOVING but not the one it's
                // meant to be filled, ignore the piece and leave the cell as 
                // CELL_CLEAR
            else if ( CELL(i - 1, j) == CELL_MOVING &&
                      CELL(i, j) > CELL_MOVING )
            {

                CELL(i, j) = CELL_CLEAR;
            }
        }
    }

    // Update the rest of the lines in the array
    lines[position] = INVALID_LINE;
    line -= HIDDEN_ROWS;

    // Update the upper rows number of the one that's been cleared
    for ( i = 0; i < BOARD_HEIGHT; i++ )
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
clearMoving (void)
{
    int i, j;

    for ( i = 0; i < MBOARD_H; i++ )
    {
        for ( j = 0; j < MBOARD_W; j++ )
        {
            if ( CELL(i, j) == CELL_MOVING )
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
destroy (void)
{
    int i;

    // Free game board memory
    free(bStruct.gboard);
    bStruct.gboard = NULL;

    // Clear bag
    for ( i = 0; i < TETROMINOS; i++ )
    {
        bStruct.bag[i] = TETROMINO_NONE;
    }

    bStruct.bagPosition = 0;
    bStruct.lastTetromino = TETROMINO_NONE;

    // Destroy the piece if it hasn't been already
    if ( bStruct.piece.destroy != NULL )
    {
        bStruct.piece.destroy();
        bStruct.piece.destroy = NULL;
    }

    // Remove stats structure
    if ( bStruct.public -> ask.stats != NULL )
    {
        bStruct.stats.destroy();
        bStruct.public -> ask.stats = NULL;
    }

    // Clear function pointers
    bStruct.public -> ask.board = NULL;
    bStruct.public -> ask.endGame = NULL;
    bStruct.public -> ask.filledRows = NULL;
    bStruct.public -> ask.movingPiece = NULL;

    bStruct.public -> clear.line = NULL;

    bStruct.public -> piece.rotate = NULL;
    bStruct.public -> piece.shift = NULL;
    bStruct.public -> piece.softDrop = NULL;

    //bStruct.public -> set.gameMode = NULL;
    bStruct.public -> set.startLevel = NULL;
    bStruct.public -> set.startRows = NULL;

    bStruct.public -> update = NULL;

    bStruct.public -> init = false;

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
endGame (void)
{
    int i, j;
    int ans = 0;
    bool found = false;

    if ( bStruct.public -> init == true )
    {
        for ( j = 0; j < MBOARD_W; j++ )
        {
            CELL(HIDDEN_ROWS, j) ? found = true : 0;
        }

        if ( found == true )
        {
            for ( i = 0; i < HIDDEN_ROWS; i++ )
            {
                for ( j = 0; j < MBOARD_W; j++ )
                {
                    CELL(i, j) > CELL_CLEAR ? ans = 1 : 0;
                }
            }
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
fillBag (void)
{
    if ( bStruct.bagPosition == TETROMINOS - 1 )
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
filledRows (int lines[BOARD_HEIGHT])
{
    int i, j;

    int fixed, nFill;

    // Clear the array
    for ( i = 0; i < BOARD_HEIGHT; i++ )
    {
        lines[i] = INVALID_LINE;
    }

    // The whole board is readed looking for the lines
    for ( i = HIDDEN_ROWS, nFill = 0; i < MBOARD_H; i++ )
    {
        // To make it faster, only the first column of each row is checked at
        // the beginning
        if ( CELL(i, 0) > CELL_CLEAR )
        {
            // And when both columns have a fixed block, the rest of the 
            // columns are checked
            for ( j = 1, fixed = 2;
                  j < MBOARD_W && fixed > 0;
                  j++ )
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
 * @brief Game mode to be played
 * 
 * @param mode From enum gModes
 * 
 * @return Nothing
 */
/*void
gameMode (int mode)
{
    switch ( mode )
    {
        case BACKWARDS:
            break;

        case CRAZY:
            break;

        case NORMAL:
        default:
            break;
    }
}*/

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
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
init (void)
{

#ifndef TRUERANDOM
    if ( init_random_generator() )
    {
        return EXIT_FAILURE;
    }
#endif

    // Allocate board
    bStruct.gboard = (grid_t *) calloc(MBOARD_W * MBOARD_H, sizeof (grid_t));

    if ( bStruct.gboard == NULL )
    {
        fputs("Could not allocate memory for the board.", stderr);
        return EXIT_FAILURE;
    }

    if ( initStats(&bStruct.stats) )
    {
        fputs("Could initialize STATS.", stderr);
        return EXIT_FAILURE;
    }

#ifdef DEBUG
    // Print board
    int row, col;
    printf("Virgin board\n");
    for ( row = 0; row < MBOARD_H; row++ )
    {
        for ( col = 0; col < MBOARD_W; col++ )
        {
            if ( CELL(row, col) == CELL_MOVING )
            {
                printf("%d ", CELL(row, col));
            }
            else
            {
                printf(" %d ", CELL(row, col));
            }
        }
        putchar('\n');
    }
    putchar('\n');
#endif

    bStruct.public -> ask.stats = &askStats;

    // Generate first set of pieces
    bStruct.bagPosition = 0;
    bStruct.lastTetromino = INVALID_PIECE;
    fillBag();

    // Piece to PIECE_NONE
    bStruct.piece.type = TETROMINO_NONE;

    bStruct.public -> update = &updatePiece;
    bStruct.public -> destroy = &destroy;

    bStruct.public -> ask.board = &askBoard;
    bStruct.public -> ask.endGame = &endGame;
    bStruct.public -> ask.filledRows = &filledRows;
    bStruct.public -> ask.movingPiece = &movingPieceInBoard;

    bStruct.public -> clear.line = &clearLine;

    // Piece functions
    bStruct.public -> piece.rotate = &rotatePiece;
    bStruct.public -> piece.shift = &shiftPiece;
    bStruct.public -> piece.softDrop = &softDropPiece;

    // Options to set for the current game
    //bStruct.public -> set.gameMode = &gameMode;
    bStruct.public -> set.startLevel = &startLevel;
    bStruct.public -> set.startRows = &startRows;

    //initTimer(&bStruct.stats);

    return EXIT_SUCCESS;
}

/**
 * @brief Tells if there's a moving piece currently in the visible board
 * 
 * @return True: 1
 * @return False: 0
 */
static int
movingPieceInBoard (void)
{
    // Answer and counters
    int ans = 0, i, j;

    if ( bStruct.piece.init == true &&
         bStruct.stats.piece.current != TETROMINO_NONE )
    {
        //CELL(r,c)
        for ( i = HIDDEN_ROWS; ans == 0 && i < MBOARD_H; i++ )
        {
            for ( j = 0; ans == 0 && j < BOARD_WIDTH; j++ )
            {
                (CELL(i, j) == CELL_MOVING) ? (ans = 1) : 0;
            }
        }
    }

    return ans;
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
rotatePiece (int direction)
{
    if ( (direction == LEFT || direction == RIGHT) &&
         (bStruct.piece.init == true) )
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
setMoving (void)
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
setFixed (int cellType)
{
    if ( cellType >= CELL_I )
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
shiftPiece (int direction)
{
    if ( (direction == LEFT || direction == RIGHT) &&
         (bStruct.piece.init == true) )
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
softDropPiece (void)
{
    if ( bStruct.piece.init == true )
    {
        // Soft drop (calls board update)
        bStruct.piece.softDrop();

        // Update stats
        updateStats(SOFT);
    }
}

/**
 * @brief Level in which the game starts
 * 
 * @param n Level number between 0 and 9
 * 
 * @return Nothing
 */
static void
startLevel (unsigned char n)
{
    (n <= 9) ? (bStruct.stats.level = n) : (bStruct.stats.level = 0);
}

/**
 * @brief Amount of rows to start in the board
 * 
 * @param n Number between 0 and 9
 * 
 * @return Nothing
 */
static void
startRows (unsigned char n)
{
    if ( n > 0 && n <= 9 )
    {
        int i = 1, j;
        int row[MBOARD_W];

        while ( i <= n )
        {
            random_row(row, MBOARD_W);

            // CELL(r,c)
            for ( j = 0; j < MBOARD_W; j++ )
            {
                CELL(MBOARD_H - i, j) = row[j];
            }

            i++;
        }
    }
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
 * @return Success: EXIT_SUCCESS
 * @return Failure: EXIT_FAILURE
 */
static int
updateBoard (int cellType)
{
    // Clear moving pieces
    clearMoving();

    // Set the piece as moving or as fixed
    if ( cellType == CELL_MOVING )
    {
        setMoving();
    }

    else
    {
        // Fix it and destroy the piece structure
        setFixed(cellType);
        bStruct.piece.destroy();
    }

    return EXIT_SUCCESS;
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
updatePiece (void)
{
    int cellType;

    // No piece is set
    if ( bStruct.piece.type == TETROMINO_NONE )
    {
        switch ( bStruct.bagPosition )
        {
                // Bag is almost empty
            case TETROMINOS - 2:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                fillBag();
                break;

                // Last piece in the bag
            case TETROMINOS - 1:
                bStruct.piece.type = bStruct.lastTetromino;
                // Reset bagPosition
                bStruct.bagPosition = 0;
                break;

                // More than 2 pieces left in the bag
            default:
                bStruct.piece.type = bStruct.bag[bStruct.bagPosition++];
                bStruct.lastTetromino = INVALID_PIECE;
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
updateStats (int action)
{
    switch ( action )
    {
        case SOFT:
            bStruct.stats.softDrop();
            break;

        case NPIECE:
            // When using the last position in the bag, read lastTetromino as 
            // the next piece. Otherwise, read the following piece in the bag
            (bStruct.bagPosition == TETROMINOS - 1) ? \
                                  bStruct.stats.newPiece(&bStruct.piece.type, \
                                                    &bStruct.lastTetromino) : \
                                  bStruct.stats.newPiece(&bStruct.piece.type, \
                                            &bStruct.bag[bStruct.bagPosition]);
            break;

        default:
            break;
    }
}
