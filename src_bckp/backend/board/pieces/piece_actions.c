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

// For BOARD_WIDTH definition in this header and PIECE_X in the .c file
#include "../board.h"

// This file
#include "piece_actions.h"

/// @privatesection
// === Constants and Macro definitions ===

/**
 * @def CELL(r,c)
 * @brief Returns the information in the row (r) and column (c) of the board
 */
#define CELL(r,c) ( *( (currentPiece.board.r0c0) + \
                    ( ((r) * currentPiece.board.width) + (c) ) ) )

// === Enumerations, structures and typedefs ===

/**
 * @brief Actions the piece can perform
 */
enum actions
{
    SHIFTL_PIECE = 0,
    SHIFTR_PIECE,
    ROTATE_PIECE,
    DROP_PIECE
};

/**
 * @brief A name for each block in the piece
 */
enum block
{
    b1 = 0,
    b2,
    b3,
    b4
};

/**
 * @brief Used to indicate a function that it must add (+) or substract (-)
 */
enum plusminus
{
    PLUS = 0,
    MINUS
};

/**
 * @brief Private variables of a piece object.
 * 
 * This structure let's the piece's related functions modify it. Contains the
 * needed parameters to shift it, drop it and rotate it, as well as the board
 * where the given piece exists.
 * 
 * @note When setting a value to @verbatim true @endverbatim or 
 * @verbatim false @endverbatim make sure to be using stdbool.h
 * 
 * @note Public PIECE structure is located in piece_actions.h
 */
typedef struct PRIVATE_PIECE
{
    /// Public structure of this piece
    piece_t * public;

    /// Board structure where this piece exists
    board_t * pieceBoard;

    /// Type of piece. Set using definitions in board.h file.
    char type;

    /// Piece's board information

    struct
    {
        /// Top left corner of the board. In a 2D array, it will be board[0][0]
        const int * r0c0;
        /// Board height
        int height;
        /// Board width
        int width;
    } board;

    /// Board coordinates of the piece. Use coords enum in board.h
    int coord[4][COORD_NUM];

    /// LEFT or RIGHT: perform shifting ; NONE: don't perform any shifting
    int shifting;

    /// Rotation status (true or false) and which is the piece's position

    struct
    {
        /// True: user asked for piece rotation ; False: don't rotate the piece
        int status;
        /// Piece position (from 1 to 4). Do not manually edit this variable
        int position;
    } rotation;

} piece_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Initialize piece's type and coordinates in the PIECE structure
static int
init (const char piece);

// Shift the piece in the desired direction
static void
shift (int direction);

// Perform a soft drop
static void
softDrop (void);

// Updates the piece in the board (called from public PIECE)
static void
update (void);

// Increment or decrement by 1 the piece's coordinates on the given axis
static void
moveOneCell (int coord, int pm);

// Updates the piece position or status (moving to fixed) in the board
static void
updatePiece (void);

// Update coordinates in PIECE structure
static void
updatePublicCoordinates (void);

// Given an action using actions enum, verify if it can be performed.
static int
verifyFixedPieces (const int action);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
/// Piece to be working with
static piece_private_t currentPiece;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Piece initialization
 * 
 * Call this function before using PIECE structure. Initializes the structure
 * with the given piece.
 * 
 * @param pstruct PIECE structure to initialize.
 * @param boardStr GAMEBOARD structure of the board with the piece
 * @param board Top left corner of the board with the piece (in a 2D array it
 * will be equivalent to board[0][0])
 * @param boardHeight Board Height
 * @param boardWidth Board Width
 * @param piece Piece to initialize
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
int
piece_init (struct PIECE * pstruct, struct GAMEBOARD * boardStr,
            int * board, int boardHeight, int boardWidth,
            const char piece)
{
    int exitStatus = -1;

    // Check that no pointer is NULL
    if ( pstruct == NULL )
    {
        fputs("PIECE struct returned NULL.", stderr);
    }

    else if ( boardStr == NULL )
    {
        fputs("GAMEBOARD struct returned NULL", stderr);
    }

    else if ( board == NULL )
    {
        fputs("BOARD returned NULL", stderr);
    }

        // Check that board dimensions aren't 0 or negative
    else if ( boardHeight < 1 || boardWidth < 1 )
    {
        fputs("Invalid boardHeigth or boardWidth", stderr);
    }

        // If no NULL pointer is given, initialize the piece
    else
    {
        // Save the pointer to PIECE in a variable to avoid asking it again
        currentPiece.public = pstruct;

        // Save the pointer to GAMEBOARD in a variable to avoid asking it again
        currentPiece.pieceBoard = boardStr;

        // Save the top left position of the board and its size
        currentPiece.board.r0c0 = board;
        currentPiece.board.height = boardHeight;
        currentPiece.board.width = boardWidth;

        // Set function pointers
        // Rotate piece
        //currentPiece.public -> rotate = &rotate;
        // Shift piece
        currentPiece.public -> shift = &shift;
        currentPiece.public -> softDrop = &softDrop;
        // Update piece in the board
        currentPiece.public -> update = &update;

        // Initialize the piece in the given position of the bag
        if ( !init(piece) )
        {
            exitStatus = 0;
            updatePublicCoordinates();
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
init (const char piece)
{
    int exitStatus = 0;

    // No rotation
    currentPiece.rotation.status = false;
    currentPiece.rotation.position = 1;

    // No shifting
    currentPiece.shifting = NONE;

    // Coordinates
    switch ( piece )
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
            currentPiece.public -> type = PIECE_I;
            currentPiece.type = PIECE_I;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2 + 1;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 1;
            currentPiece.coord[b2][COORD_Y] = 1;
            currentPiece.coord[b3][COORD_Y] = 1;
            currentPiece.coord[b4][COORD_Y] = 1;

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
            currentPiece.public -> type = PIECE_J;
            currentPiece.type = PIECE_J;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 0;
            currentPiece.coord[b4][COORD_Y] = 1;
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
            currentPiece.public -> type = PIECE_L;
            currentPiece.type = PIECE_L;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 0;
            currentPiece.coord[b4][COORD_Y] = 1;
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
            currentPiece.public -> type = PIECE_O;
            currentPiece.type = PIECE_O;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 1;
            currentPiece.coord[b4][COORD_Y] = 1;
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
            currentPiece.public -> type = PIECE_S;
            currentPiece.type = PIECE_S;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2 - 1;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 1;
            currentPiece.coord[b4][COORD_Y] = 1;
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
            currentPiece.public -> type = PIECE_T;
            currentPiece.type = PIECE_T;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2 - 1;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 0;
            currentPiece.coord[b4][COORD_Y] = 1;
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
            currentPiece.public -> type = PIECE_Z;
            currentPiece.type = PIECE_Z;

            // X coordinates
            currentPiece.coord[b1][COORD_X] = currentPiece.board.width / 2 - 2;
            currentPiece.coord[b2][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b3][COORD_X] = currentPiece.board.width / 2 - 1;
            currentPiece.coord[b4][COORD_X] = currentPiece.board.width / 2;

            // Y coordinates
            currentPiece.coord[b1][COORD_Y] = 0;
            currentPiece.coord[b2][COORD_Y] = 0;
            currentPiece.coord[b3][COORD_Y] = 1;
            currentPiece.coord[b4][COORD_Y] = 1;
            break;

            // Bad piece
        default:
            exitStatus = -1;
            break;
    }

    return exitStatus;
}

/**
 * @brief Shift the piece in the desired direction
 * 
 * @param direction Direction to shift the piece (LEFT or RIGHT, according to
 * shiftingTypes enum).
 * 
 * @return Nothing
 */
static void
shift (int direction)
{
    if ( direction == LEFT || direction == RIGHT )
    {
        // Shift the piece in the desired direction
        currentPiece.shifting = direction;
        updatePiece();
    }

    // Restore shifting to NONE
    currentPiece.shifting = NONE;

    // Update public coordinates
    updatePublicCoordinates();
}

/**
 * @brief Perform a soft drop
 * 
 * Drops the piece one row so, when called from a function which updates the
 * board constantly, it seems like a double drop at the same time.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
softDrop (void)
{
    // Drop once
    updatePiece();

    // Update public coordinates
    updatePublicCoordinates();
}

/**
 * @brief Updates the piece in the board (called from public PIECE)
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
update (void)
{
    updatePiece();
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
moveOneCell (int coord, int pm)
{
    // +1 in coord axis
    if ( pm == PLUS )
    {
        (currentPiece.coord[b1][coord]) += 1;
        (currentPiece.coord[b2][coord]) += 1;
        (currentPiece.coord[b3][coord]) += 1;
        (currentPiece.coord[b4][coord]) += 1;
    }

        // -1 in coord axis
    else if ( pm == MINUS )
    {
        (currentPiece.coord[b1][coord]) -= 1;
        (currentPiece.coord[b2][coord]) -= 1;
        (currentPiece.coord[b3][coord]) -= 1;
        (currentPiece.coord[b4][coord]) -= 1;
    }
}

/**
 * @brief Updates the piece position or status (moving to fixed) in the board
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
updatePiece (void)
{
    int perform = -1;

    // Piece doesn't have to be rotated or shifted
    if ( currentPiece.shifting == NONE &&
         currentPiece.rotation.status == false )
    {
        // Can dropping be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(DROP_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved +1 on Y axis
            moveOneCell(COORD_Y, PLUS);
        }
    }

        // Piece should be shifted left
    else if ( currentPiece.shifting == LEFT )
    {
        // Can shifting be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(SHIFTL_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved -1 on X axis
            moveOneCell(COORD_X, MINUS);
        }

    }

        // Piece should be shifted to the right
    else if ( currentPiece.shifting == RIGHT )
    {
        // Can shifting be performed? (0 if true, 1 if false, -1 if error)
        perform = verifyFixedPieces(SHIFTR_PIECE);

        // If true, update the piece's coordinates
        if ( !perform )
        {
            // Moved +1 on X axis
            moveOneCell(COORD_X, PLUS);
        }
    }

        // Piece was be rotated (in the structure) and board must be updated
    else if ( currentPiece.rotation.status == true )
    {
        // Can rotation be performed? Always yes (0) as it's been checked on
        // piece_actions.c
        perform = 0;
    }

    // Update public coordinates so pieceBoard functions work
    // properly
    updatePublicCoordinates();

    switch ( perform )
    {
        case -1:
            fputs("Invalid parameter passed to verifyFixedPieces()", stderr);
            break;

            // Piece can be rotated, shifted or dropped and no other piece or
            // border is on its way
        case 0:
            // Clear cells with moving blocks
            currentPiece.pieceBoard -> piece.clear.moving();

            // Set new cells with moving blocks
            currentPiece.pieceBoard -> piece.set.moving();
            break;

            // Piece can't be shifted or dropped as there's another
            // one (or a border) on its way
        case 1:
            // When shifting the piece isn't possible, it should not be 
            // blocked as no dropping occurs at the same time
            if ( currentPiece.shifting == NONE )
            {
                // Clear cells with moving blocks
                currentPiece.pieceBoard -> piece.clear.moving();
                currentPiece.pieceBoard -> piece.set.fixed();

                // Clear piece as it's been fixed
                currentPiece.public -> type = PIECE_NONE;
                currentPiece.type = PIECE_NONE;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Update coordinates in PIECE structure
 * 
 * Uses the private coordinates (those in PRIVATE_PIECE structure) to update
 * the public ones.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
updatePublicCoordinates (void)
{
    currentPiece.public -> get.coordinates[b1][COORD_X] = \
            currentPiece.coord[b1][COORD_X];
    currentPiece.public -> get.coordinates[b1][COORD_Y] = \
            currentPiece.coord[b1][COORD_Y];

    currentPiece.public -> get.coordinates[b2][COORD_X] = \
            currentPiece.coord[b2][COORD_X];
    currentPiece.public -> get.coordinates[b2][COORD_Y] = \
            currentPiece.coord[b2][COORD_Y];

    currentPiece.public -> get.coordinates[b3][COORD_X] = \
            currentPiece.coord[b3][COORD_X];
    currentPiece.public -> get.coordinates[b3][COORD_Y] = \
            currentPiece.coord[b3][COORD_Y];

    currentPiece.public -> get.coordinates[b4][COORD_X] = \
            currentPiece.coord[b4][COORD_X];
    currentPiece.public -> get.coordinates[b4][COORD_Y] = \
            currentPiece.coord[b4][COORD_Y];
}

/**
 * @brief Given an action using actions enum, verify if it can be performed.
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
verifyFixedPieces (const int action)
{
    // Unless proven otherwise, action cannot be performed
    int ans = 1;

    // To make this code more readable, the piece's coordinates are copied in
    // shorter named variables
    int b1_x = currentPiece.coord[b1][COORD_X];
    int b2_x = currentPiece.coord[b2][COORD_X];
    int b3_x = currentPiece.coord[b3][COORD_X];
    int b4_x = currentPiece.coord[b4][COORD_X];

    int b1_y = currentPiece.coord[b1][COORD_Y];
    int b2_y = currentPiece.coord[b2][COORD_Y];
    int b3_y = currentPiece.coord[b3][COORD_Y];
    int b4_y = currentPiece.coord[b4][COORD_Y];

    switch ( action )
    {
            // Shift piece to the left
        case SHIFTL_PIECE:
            // There musn't be a fixed piece at the left nor shall the piece 
            // be at the leftmost border.
            if ( CELL(b1_y, b1_x - 1) != CELL_FIXED &&
                 CELL(b2_y, b2_x - 1) != CELL_FIXED &&
                 CELL(b3_y, b3_x - 1) != CELL_FIXED &&
                 CELL(b4_y, b4_x - 1) != CELL_FIXED &&

                 b1_x - 1 >= 0 &&
                 b2_x - 1 >= 0 &&
                 b3_x - 1 >= 0 &&
                 b4_x - 1 >= 0 )

            {
                ans = 0;
            }
            break;

            // Shift piece to the left
        case SHIFTR_PIECE:
            // There musn't be a fixed piece at the right nor shall the piece
            // be at the rightmost border.
            if ( CELL(b1_y, b1_x + 1) != CELL_FIXED &&
                 CELL(b2_y, b2_x + 1) != CELL_FIXED &&
                 CELL(b3_y, b3_x + 1) != CELL_FIXED &&
                 CELL(b4_y, b4_x + 1) != CELL_FIXED &&

                 b1_x + 1 < currentPiece.board.width &&
                 b2_x + 1 < currentPiece.board.width &&
                 b3_x + 1 < currentPiece.board.width &&
                 b4_x + 1 < currentPiece.board.width )

            {
                ans = 0;
            }
            break;

            // Drop the piece one position
        case DROP_PIECE:
            // There musn't be a fixed piece under the current piece nor 
            // shall the piece be at the bottom of the board.
            if ( CELL(b1_y + 1, b1_x) != CELL_FIXED &&
                 CELL(b2_y + 1, b2_x) != CELL_FIXED &&
                 CELL(b3_y + 1, b3_x) != CELL_FIXED &&
                 CELL(b4_y + 1, b4_x) != CELL_FIXED &&

                 b1_y + 1 < currentPiece.board.height &&
                 b2_y + 1 < currentPiece.board.height &&
                 b3_y + 1 < currentPiece.board.height &&
                 b4_y + 1 < currentPiece.board.height )

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
