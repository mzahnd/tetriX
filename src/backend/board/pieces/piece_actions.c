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
 * @brief   Manage a piece actions like rotation, shifting and dropping.
 * 
 * Using PIECE structure a given piece can be rotated, shifted and droped using
 * soft drop. Also it's coordinates can be retrived for "real time"
 * representation of it in the board.
 * 
 * @note piece_init() <b>must</b> be called before accessing the structure.
 * If not done so, the structure behaviour is undefined.
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

// This file
#include "piece_actions.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

enum actions
{
    SHIFTL_PIECE = 0,
    SHIFTR_PIECE,
    ROTATE_PIECE,
    DROP_PIECE
};

enum plusminus
{
    PLUS = 0,
    MINUS
};

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Initialize piece's type and coordinates in the PIECE structure
static int
init (const char * bag, const int position);

// Perform piece rotation
static void
rotate (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate an I piece
static void
rotate_I (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate a J piece
static void
rotate_J (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate a L piece
static void
rotate_L (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate a S piece
static void
rotate_S (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate a T piece
static void
rotate_T (int board [][BOARD_WIDTH], const int boardHeight);

// Rotate a Z piece
static void
rotate_Z (int board [][BOARD_WIDTH], const int boardHeight);

// Updates piece coordinates with the ones passed as argument
static void
update_coordinates (int c1[COORD_NUM], int c2[COORD_NUM], int c3[COORD_NUM], int c4[COORD_NUM]);

// Update piece position in PIECE structure
static void
update_rotation (void);

// Updates the piece position or status in the board
static void
updatePiece (struct GAMEBOARD * boardStr,
             int board[][BOARD_WIDTH], int boardHeight);

// Given an action from enum actions, verify if it can be performed.
static int
verifyFixedPieces (int board[][BOARD_WIDTH], int boardHeight,
                   const int action);

// Increment or decrement by 1 the piece's coordinates on the given axis
static void
updatePCoordinates (int coord, int pm);
// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static piece_t * currentPiece = NULL;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Piece initialization
 * 
 * Call this function before using PIECE structure. Initializes the structure
 * with the piece in the given @p position of the @p bag.
 * 
 * @param pstruct PIECE structure to initialize.
 * @param bag Bag with set of random pieces generated using random_generator().
 * @param position Piece's position in the bag.
 * @return Success: 0
 * @return Fail: Non 0
 */
int
piece_init (struct PIECE * pstruct,
            const char * bag, const int position)
{
    int exitStatus = 0;

    // Check that no pointer is NULL
    if ( pstruct == NULL )
    {
        fputs("PIECE struct returned NULL.", stderr);
        exitStatus = -1;
    }

    else if ( bag == NULL )
    {
        fputs("BAG returned NULL.", stderr);
        exitStatus = -1;
    }

        // If no NULL pointer is given, initialize the piece
    else
    {
        // Save the pointer to PIECE in a variable to avoid asking it again
        currentPiece = pstruct;

        // Set function pointers
        // Update piece in the board
        pstruct -> update = &updatePiece;
        // Rotate piece
        pstruct -> rotate = &rotate;

        // Initialize the piece in the given position of the bag
        if ( init(bag, position) )
        {
            exitStatus = -1;
        }
    }

    return exitStatus;
}

/// @privatesection
// === Local function definitions ===

/**
 * @details Initialize piece's type and coordinates in the PIECE structure
 * 
 * Clear structure variables, setting rotation and dropping to false and
 * shifting to NONE, and initialize piece's coordinates in the structure.
 * If the board is updated right after this function, the piece won't be 
 * visible as it's initialized in the top two cells so two droppings must occur
 * before the user can see it.
 * 
 * @param bag Pointer to bag array with the piece to initialize.
 * @param position Position of the piece in the bag.
 * @return Success: 0
 * @return Fail: Non 0
 */
static int
init (const char * bag, const int position)
{
    int exitStatus = 0;

    // No rotation
    currentPiece -> rotation.status = false;
    currentPiece -> rotation.position = 1;

    // No soft drop
    currentPiece -> drop = false;

    // No shifting
    currentPiece -> shifting = NONE;

    // Coordinates
    switch ( bag[position] )
    {
            /*
             *     ---------------------
             *  0  |    |    |    |    |
             *     ---------------------
             *  1  | b1 | b2 | b3 | b4 |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_I:
            currentPiece -> type = PIECE_I;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2 + 1;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 1;
            currentPiece -> coord.b2[COORD_Y] = 1;
            currentPiece -> coord.b3[COORD_Y] = 1;
            currentPiece -> coord.b4[COORD_Y] = 1;

            break;

            /*
             *     ---------------------
             *  0  | b1 | b2 | b3 |    |
             *     ---------------------
             *  1  |    |    | b4 |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_J:
            currentPiece -> type = PIECE_J;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 0;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            /*
             *     ---------------------
             *  0  | b1 | b2 | b3 |    |
             *     ---------------------
             *  1  | b4 |    |    |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_L:
            currentPiece -> type = PIECE_L;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2 - 2;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 0;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            /*
             *     ---------------------
             *  0  |    | b1 | b2 |    |
             *     ---------------------
             *  1  |    | b3 | b4 |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_O:
            currentPiece -> type = PIECE_O;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 1;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            /*
             *     ---------------------
             *  0  |    | b1 | b2 |    |
             *     ---------------------
             *  1  | b3 | b4 |    |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_S:
            currentPiece -> type = PIECE_S;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2 - 1;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 1;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            /*
             *     ---------------------
             *  0  | b1 | b2 | b3 |    |
             *     ---------------------
             *  1  |    | b4 |    |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_T:
            currentPiece -> type = PIECE_T;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2 - 1;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 0;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            /*
             *     ---------------------
             *  0  | b1 | b2 |    |    |
             *     ---------------------
             *  1  |    | b3 | b4 |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
        case PIECE_Z:
            currentPiece -> type = PIECE_Z;

            // X coordinates
            currentPiece -> coord.b1[COORD_X] = BOARD_WIDTH / 2 - 2;
            currentPiece -> coord.b2[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b3[COORD_X] = BOARD_WIDTH / 2 - 1;
            currentPiece -> coord.b4[COORD_X] = BOARD_WIDTH / 2;

            // Y coordinates
            currentPiece -> coord.b1[COORD_Y] = 0;
            currentPiece -> coord.b2[COORD_Y] = 0;
            currentPiece -> coord.b3[COORD_Y] = 1;
            currentPiece -> coord.b4[COORD_Y] = 1;
            break;

            // Bad piece
        default:
            exitStatus = -1;
            break;
    }

    return exitStatus;
}

/**
 * @brief Perform piece rotation
 * 
 *  rotation.status must be also set to true
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate (int board [][BOARD_WIDTH], const int boardHeight)
{
    // Verify if board points to NULL for some reason
    if ( board == NULL )
    {
        fputs("Bad board matrix.", stderr);
        return;
    }

    // Rotate piece according to it's type
    switch ( currentPiece -> type )
    {
        case PIECE_I:
            rotate_I(board, boardHeight);
            break;

        case PIECE_J:
            rotate_J(board, boardHeight);
            break;

        case PIECE_L:
            rotate_L(board, boardHeight);
            break;

        case PIECE_O:
            // Nothing to rotate
            update_rotation();
            break;

        case PIECE_S:
            rotate_S(board, boardHeight);
            break;

        case PIECE_T:
            rotate_T(board, boardHeight);
            break;

        case PIECE_Z:
            rotate_Z(board, boardHeight);
            break;

            // Although it should never happen, an error message is printed if
            // a bad piece is written in piece type.
        default:
            fputs("Bad PIECE type for rotation.", stderr);
            break;
    }
}

/**
 * @brief Rotate an I piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_I (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM];
    int new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 2;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 2;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] + 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y] - 1;

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y];

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b3 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b4[COORD_Y] < boardHeight &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b2[COORD_X] ][ new_b2[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 2;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 2;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] - 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y] + 1;

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y];

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] - 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b3 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] >= 0 && new_b4[COORD_X] < BOARD_WIDTH &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b2[COORD_X] ][ new_b2[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Rotate a J piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_J (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM];
    int new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 2;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 2;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < BOARD_WIDTH &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 2;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < boardHeight &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X];
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 2;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] >= 0 &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Rotate a L piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_L (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM], new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X];
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] - 2;


            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 2;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < BOARD_WIDTH &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X];
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 2;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < boardHeight &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 2;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 is not checked as it keeps the same position and will
             * always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] >= 0 &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Rotate a S piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_S (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM], new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X];
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y];

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] - 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y] + 1;

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 2;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] - 1;

            // Verify if can be rotated
            /*
             * Block b1 and b2 are not checked as they keep the same position 
             * and will always be a CELL_MOVING coordinate 
             */
            if ( board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X];
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y];

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] + 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y] - 1;

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 2;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b1 and b4 are not checked as they keep the same position 
             * and will always be a CELL_MOVING coordinate 
             */
            if ( new_b2[COORD_X] < BOARD_WIDTH &&
                 board[ new_b2[COORD_X] ][ new_b2[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b3[COORD_X] ][ new_b3[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Rotate a T piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_T (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM], new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] - 1;

            // Verify if can be rotated
            /*
             * Block b1 is the only checked as the rest keep the same position
             * and will always be a CELL_MOVING coordinate 
             */
            if ( board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] - 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] - 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b1 is the only checked as the rest keep the same position
             * and will always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < BOARD_WIDTH &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b1 is the only checked as the rest keep the same position
             * and will always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] < boardHeight &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 1;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X];
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X] + 1;
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y] + 1;

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b1 is the only checked as the rest keep the same position
             * and will always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] >= 0 &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Rotate a Z piece
 * 
 * @param board Board where the piece must be updated
 * @param boardHeight Board height.
 */
static void
rotate_Z (int board [][BOARD_WIDTH], const int boardHeight)
{
    int new_b1[COORD_NUM], new_b2[COORD_NUM], new_b3[COORD_NUM], new_b4[COORD_NUM];

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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] + 2;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] - 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] + 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] - 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] - 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Block b3 and b4 are not checked as they keep the same position 
             * and will always be a CELL_MOVING coordinate 
             */
            if ( board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b2[COORD_X] ][ new_b2[COORD_Y] ] == CELL_CLEAR )
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
            new_b1[COORD_X] = currentPiece -> coord.b1[COORD_X] - 2;
            new_b1[COORD_Y] = currentPiece -> coord.b1[COORD_Y] + 1;

            new_b2[COORD_X] = currentPiece -> coord.b2[COORD_X] - 1;
            new_b2[COORD_Y] = currentPiece -> coord.b2[COORD_Y];

            new_b3[COORD_X] = currentPiece -> coord.b3[COORD_X];
            new_b3[COORD_Y] = currentPiece -> coord.b3[COORD_Y] + 1;

            new_b4[COORD_X] = currentPiece -> coord.b4[COORD_X] + 1;
            new_b4[COORD_Y] = currentPiece -> coord.b4[COORD_Y];

            // Verify if can be rotated
            /*
             * Piece musn't "fall out" of the board and no other piece should 
             * be fixed on it's new position.
             * Block b2 and b3 are not checked as they keep the same position 
             * and will always be a CELL_MOVING coordinate 
             */
            if ( new_b1[COORD_X] >= 0 &&
                 board[ new_b1[COORD_X] ][ new_b1[COORD_Y] ] == CELL_CLEAR &&
                 board[ new_b4[COORD_X] ][ new_b4[COORD_Y] ] == CELL_CLEAR )
            {
                // If it can be rotated, do it
                update_coordinates(new_b1, new_b2, new_b3, new_b4);
                update_rotation();
            }

            break;

            // If a bad rotation position is set, reset it to avoid another 
            // error
        default:
            update_rotation();
            break;
    }
}

/**
 * @brief Updates piece coordinates with the ones passed as argument
 * 
 * @param c1 Array of size COORD_NUM with new COORD_X and COORD_Y coordinates
 * @param c2 Array of size COORD_NUM with new COORD_X and COORD_Y coordinates
 * @param c3 Array of size COORD_NUM with new COORD_X and COORD_Y coordinates
 * @param c4 Array of size COORD_NUM with new COORD_X and COORD_Y coordinates
 * @return Nothing
 */
static void
update_coordinates (int c1[COORD_NUM], int c2[COORD_NUM],
                    int c3[COORD_NUM], int c4[COORD_NUM])
{
    // X coordinates
    currentPiece -> coord.b1[COORD_X] = c1[COORD_X];
    currentPiece -> coord.b2[COORD_X] = c2[COORD_X];
    currentPiece -> coord.b3[COORD_X] = c3[COORD_X];
    currentPiece -> coord.b4[COORD_X] = c4[COORD_X];

    // Y coordinates
    currentPiece -> coord.b1[COORD_Y] = c1[COORD_Y];
    currentPiece -> coord.b2[COORD_Y] = c2[COORD_Y];
    currentPiece -> coord.b3[COORD_Y] = c3[COORD_Y];
    currentPiece -> coord.b4[COORD_Y] = c4[COORD_Y];
}

/**
 * @brief Update piece position in PIECE structure
 * 
 * @param None
 * @return Nothing
 */
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

    // FIXME
    currentPiece -> rotation.status = false;
}

/**
 * @brief Updates the piece position or status in the board
 * 
 * If called with piece.shifting set to NONE and piece.rotation.status to 
 * false, the piece is just dropped one grid cell or fixed if another piece or
 * the bottom of the board is found at the bottom.
 * 
 * When called with a shifting, it's moved one position to
 * the right or the left if no other pieces (or boarders) are found.
 * 
 * Finally, if called with rotation status set to true, the same logic than
 * trying to drop the piece one position is carried, although, no falling 
 * occurs. This is because the rotation logic is performed in function on
 * piece_actions.c file, as every piece must be rotated differently.
 * 
 * @param None
 * 
 * @return Nothing
 * 
 * @note When a piece is fixed, it's piece.type is set to PIECE_NONE so the 
 * next one can be initialized
 */
static void
updatePiece (struct GAMEBOARD * boardStr,
             int board[][BOARD_WIDTH], int boardHeight)
{
    int perform = -1;

    // Piece doesn't have to be rotated or shifted
    if ( currentPiece -> shifting == NONE &&
         currentPiece -> rotation.status == false )
    {
        // Can dropping be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(board, boardHeight, DROP_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved +1 on Y axis
            updatePCoordinates(COORD_Y, PLUS);
        }
    }

        // Piece should be shifted left
    else if ( currentPiece -> shifting == LEFT )
    {
        // Can shifting be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(board, boardHeight, SHIFTL_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved -1 on X axis
            updatePCoordinates(COORD_X, MINUS);
        }

    }

        // Piece should be shifted to the right
    else if ( currentPiece -> shifting == RIGHT )
    {
        // Can shifting be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(board, boardHeight, SHIFTR_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved +1 on X axis
            updatePCoordinates(COORD_X, PLUS);
        }
    }

        // Piece was be rotated (in the structure) and board must be updated
    else if ( currentPiece -> rotation.status == true )
    {
        // Can rotation be performed? Always yes (0) as it's been checked on
        // piece_actions.c
        perform = 0;
    }

    switch ( perform )
    {
        case -1:
            fputs("Invalid parameter verify_fixed_pieces()", stderr);
            break;

            // Piece can be rotated, shifted or dropped and no other piece or
            // border is on its way
        case 0:
            // Clear cells with moving blocks
            boardStr -> piece.clear.moving();

            // Set new cells with moving blocks
            boardStr -> piece.set.moving();
            break;

            // Piece can't be shifted or dropped as there's another
            // one (or a border) on its way
        case 1:
            // When shifting the piece it isn't possible, it should not be 
            // blocked as no dropping occurs at the same time
            if ( currentPiece -> shifting == NONE )
            {
                // Clear cells with moving blocks
                boardStr -> piece.clear.moving();
                boardStr -> piece.set.fixed();

                // Clear piece as it's been fixed
                currentPiece -> type = PIECE_NONE;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Given an action from enum actions, verify if it can be performed.
 * 
 * If a shifting action is given, borders and other fixed pieces are checked;
 * when a dropping action is passed, verify the bottom border and other fixed
 * pieces as well. 
 * 
 * @param action Action from enum actions
 * @return -1 if an invalid action was passed
 * @return 0 if action can be performed
 * @return 1 if action cannot be performed
 */
static int
verifyFixedPieces (int board[][BOARD_WIDTH], int boardHeight, const int action)
{
    // Unless proven otherwise, action cannot be performed
    int ans = 1;

    // To make this code more readable, the piece's coordinates are copied in
    // shorter named variables
    int b1_x = currentPiece -> coord.b1[COORD_X];
    int b2_x = currentPiece -> coord.b2[COORD_X];
    int b3_x = currentPiece -> coord.b3[COORD_X];
    int b4_x = currentPiece -> coord.b4[COORD_X];

    int b1_y = currentPiece -> coord.b1[COORD_Y];
    int b2_y = currentPiece -> coord.b2[COORD_Y];
    int b3_y = currentPiece -> coord.b3[COORD_Y];
    int b4_y = currentPiece -> coord.b4[COORD_Y];

    switch ( action )
    {
            // Shift piece to the left
        case SHIFTL_PIECE:
            // There musn't be a fixed piece at the left nor shall the piece 
            // be at the leftmost border.
            if ( board[b1_y][b1_x - 1] != CELL_FIXED &&
                 board[b2_y][b2_x - 1] != CELL_FIXED &&
                 board[b3_y][b3_x - 1] != CELL_FIXED &&
                 board[b4_y][b4_x - 1] != CELL_FIXED &&

                 b1_x - 1 > 0 &&
                 b2_x - 1 > 0 &&
                 b3_x - 1 > 0 &&
                 b4_x - 1 > 0 )

            {
                ans = 0;
            }
            break;

            // Shift piece to the left
        case SHIFTR_PIECE:
            // There musn't be a fixed piece at the right nor shall the piece
            // be at the rightmost border.
            if ( board[b1_y][b1_x + 1] != CELL_FIXED &&
                 board[b2_y][b2_x + 1] != CELL_FIXED &&
                 board[b3_y][b3_x + 1] != CELL_FIXED &&
                 board[b4_y][b4_x + 1] != CELL_FIXED &&

                 b1_x + 1 < BOARD_WIDTH &&
                 b2_x + 1 < BOARD_WIDTH &&
                 b3_x + 1 < BOARD_WIDTH &&
                 b4_x + 1 < BOARD_WIDTH )

            {
                ans = 0;
            }
            break;

            // Drop the piece one position
        case DROP_PIECE:
            // There musn't be a fixed piece under the current piece nor 
            // shall the piece be at the bottom of the board.
            if ( board[b1_y + 1][b1_x] != CELL_FIXED &&
                 board[b2_y + 1][b2_x] != CELL_FIXED &&
                 board[b3_y + 1][b3_x] != CELL_FIXED &&
                 board[b4_y + 1][b4_x] != CELL_FIXED &&

                 b1_y + 1 < boardHeight &&
                 b2_y + 1 < boardHeight &&
                 b3_y + 1 < boardHeight &&
                 b4_y + 1 < boardHeight )

            {
                ans = 0;
            }
            break;

            // Rotation is checked in functions on piece_actions.c file
        case ROTATE_PIECE:
            break;

            // Invalid parameter is given
        default:
            ans = -1;
            break;
    }

    return ans;
}

/**
 * @brief Increment or decrement by 1 the piece's coordinates on the given axis
 *  
 * @param coord COORD_X for X axis or COORD_Y for Y axis
 * @param pm PLUS to increment or MINUS to decrement
 * 
 * @return Nothing
 */
static void
updatePCoordinates (int coord, int pm)
{
    // +1 in coord axis
    if ( pm == PLUS )
    {
        (currentPiece -> coord.b1[coord]) += 1;
        (currentPiece -> coord.b2[coord]) += 1;
        (currentPiece -> coord.b3[coord]) += 1;
        (currentPiece -> coord.b4[coord]) += 1;
    }

        // -1 in coord axis
    else if ( pm == MINUS )
    {
        (currentPiece -> coord.b1[coord]) -= 1;
        (currentPiece -> coord.b2[coord]) -= 1;
        (currentPiece -> coord.b3[coord]) -= 1;
        (currentPiece -> coord.b4[coord]) -= 1;
    }
}
