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

// This file
#include "piece_actions.h"

/// @privatesection
// === Constants and Macro definitions ===

/**
 * @def CELL(r,c)
 * @brief Returns the information in the row (r) and column (c) of the board
 */
#define CELL(c,r) ( * (currentPiece.board.r0c0 + \
                    (((r) * currentPiece.board.width) + (c)) ) )

// === Enumerations, structures and typedefs ===

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
 * needed parameters to shift, drop and rotate it, as well as the board
 * where the given piece exists.
 * 
 * @note Public PIECE structure is located in piece_actions.h
 */
typedef struct PRIVATE_PIECE
{
    /// Public structure of this piece
    piece_t * public;

    /// Piece's board information

    struct
    {
        /// Board structure where this piece exists
        board_t * pBoard;
        /// Top left corner of the board. In a 2D array, it will be board[0][0]
        const int * r0c0;
        /// Board height
        int height;
        /// Board width
        int width;
    } board;

    /// Type of piece. Set using definitions in board.h file.
    int type;

    /// Board coordinates of the piece. Use coords enum in board.h
    int move[COORD_NUM];

    /// Rotation status (true or false) and which is the piece's position
    int orientation;

} piece_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Destroy the current piece.
static void
destroy (void);

// Initialize piece's type and coordinates in the PIECE structure
static int
init (const int piece);

// Increment or decrement by 1 the piece's coordinates on the given axis
static void
moveOneCell (int coord, int pm);

// Updates the piece in the board (called from public PIECE)
static int
normalDrop (void);

// Rotate the piece in the desired direction
static void
rotate (int direction);

// Shift the piece in the desired direction
static void
shift (int direction);

// Perform a soft drop
static void
softDrop (void);

// Update piece orientation
static void
updateOrientation (int pm);

// Update coordinates in PIECE structure
static void
updatePublicCoordinates (void);

// Given a piece a new position, check if it can be keeped there or not.
static int
verifyFixedPieces (void);

// === ROM Constant variables with file level scope ===
/// Array with each piece coordinates and orientations.
/// @note A more readable code can be readed from "Tetrominos_Table.txt" file,
/// inside docs folder
const int pieceArr[TETROMINOS][ORIENTATION][BLOCKS][COORD_NUM] = {

    // TETROMINO I
    /*
     *		 Initial position
     *
     *		 0    1    2    3
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
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 1},
            {1, 1},
            {2, 1},
            {3, 1}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 0},
            {2, 1},
            {2, 2},
            {2, 3}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 2},
            {1, 2},
            {2, 2},
            {3, 2}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {1, 1},
            {1, 2},
            {1, 3}
        }
    },

    // TETROMINO J
    /*
     *		 Initial position
     *
     *		 0    1    2    3
     *     ---------------------
     *  0  |    |    |    |    |
     *     ---------------------
     *  1  | b1 |    |    |    |
     *     ---------------------
     *  2  | b2 | b3 | b4 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     */
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 1},
            {0, 2},
            {1, 2},
            {2, 2}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {0, 0},
            {0, 1},
            {0, 2}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 1},
            {2, 0},
            {1, 0},
            {0, 0}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 2},
            {2, 2},
            {2, 1},
            {2, 0}
        }
    },

    // TETROMINO L
    /*
     *		 Initial position
     *
     *		 0    1    2    3
     *     ---------------------
     *  0  |    |    |    |    |
     *     ---------------------
     *  1  |    |    | b1 |    |
     *     ---------------------
     *  2  | b4 | b3 | b2 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     */
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 1},
            {2, 2},
            {1, 2},
            {0, 2}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 2},
            {0, 2},
            {0, 1},
            {0, 0}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 1},
            {0, 0},
            {1, 0},
            {2, 0}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {2, 0},
            {2, 1},
            {2, 2}
        }
    },

    // TETROMINO O
    /*
     *		 Initial position
     *
     *		 0    1    2    3
     *     ---------------------
     *  0  | b1 | b2 |    |    |
     *     ---------------------
     *  1  | b3 | b4 |    |    |
     *     ---------------------
     *  2  |    |    |    |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     */
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {0, 1},
            {1, 1}
        }
    },

    // TETROMINO S
    /*
     *		 Initial position
     *
     *		 0    1    2    3
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
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {2, 0},
            {0, 1},
            {1, 1}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 1},
            {2, 2},
            {1, 0},
            {1, 1}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {2, 0},
            {0, 1},
            {1, 1}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 1},
            {2, 2},
            {1, 0},
            {1, 1}
        }
    },

    // TETROMINO T
    /*
     *		 Initial position
     *
     *		 0    1    2    3
     *     ---------------------
     *  0  |    | b1 |    |    |
     *     ---------------------
     *  1  | b2 | b3 | b4 |    |
     *     ---------------------
     *  2  |    |    |    |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     */
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 0},
            {0, 1},
            {1, 1},
            {2, 1}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 1},
            {1, 0},
            {1, 1},
            {1, 2}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {1, 2},
            {0, 1},
            {1, 1},
            {2, 1}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 1},
            {1, 0},
            {1, 1},
            {1, 2}
        }
    },

    // TETROMINO Z
    /*
     *		 Initial position
     *
     *		 0    1    2    3
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
    {
        /*
         * Position 1
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {1, 1},
            {2, 1}
        },
        /*
         * Position 2
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 0},
            {2, 1},
            {1, 1},
            {1, 2}
        },
        /*
         * Position 3
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {0, 0},
            {1, 0},
            {1, 1},
            {2, 1}
        },
        /*
         * Position 4
         *  {X, Y}
         * b1      b2      b3      b4
         */
        {
            {2, 0},
            {2, 1},
            {1, 1},
            {1, 2}
        }
    }
};

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
            grid_t * board, int boardHeight, int boardWidth,
            const int piece)
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
        currentPiece.board.pBoard = boardStr;

        // Save the top left position of the board and its size
        currentPiece.board.r0c0 = board;
        currentPiece.board.height = boardHeight;
        currentPiece.board.width = boardWidth;

        // Set function pointers
        // Rotate piece
        currentPiece.public -> rotate = &rotate;
        // Shift piece
        currentPiece.public -> shift = &shift;
        // Soft drop
        currentPiece.public -> softDrop = &softDrop;
        // Normal drop
        currentPiece.public -> update = &normalDrop;

        currentPiece.public -> destroy = &destroy;

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
 * @brief Destroy the current piece. 
 * 
 * All structure information is ereased.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
destroy (void)
{
    int i, j;

    // Check if it's accidentaly called
    if ( currentPiece.public == NULL )
    {
        fputs("Bad destroy call. Noting to destroy, exiting...", stderr);
        return;
    }

    // Clear public variables and set pointers to NULL
    currentPiece.public -> type = TETROMINO_NONE;
    currentPiece.public -> rotate = NULL;
    currentPiece.public -> shift = NULL;
    currentPiece.public -> softDrop = NULL;
    currentPiece.public -> update = NULL;

    // Clear public piece's coordinates
    for ( i = b1; i < BLOCKS; i++ )
    {
        for ( j = COORD_X; j < COORD_NUM; j++ )
        {
            currentPiece.public -> get.coordinates[i][j] = 0;
        }
    }

    // Clear board information
    currentPiece.board.r0c0 = NULL;
    currentPiece.board.height = 0;
    currentPiece.board.width = 0;
    currentPiece.board.pBoard = NULL;

    // Clear type and position information
    currentPiece.type = TETROMINO_NONE;
    currentPiece.move[COORD_X] = 0;
    currentPiece.move[COORD_Y] = 0;
    currentPiece.orientation = 0;

    currentPiece.public -> destroy = NULL;
    currentPiece.public = NULL;
}

/**
 * @details Initialize piece's type and coordinates in the PIECE structure
 * 
 * Clear piece orientation variable and initialize it in the middle of the 
 * board. Take into account that some pieces wont be visible to the user if the
 * function to update the board is called right after this function as they
 * appear a row or two over the first visible row.
 * 
 * @param piece Piece to initialize. Use tetrominos enum in board.h
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
static int
init (const int piece)
{
    int exitStatus = 0;

    // No rotation
    currentPiece.orientation = 0;

    // Coordinates
    switch ( piece )
    {
        case TETROMINO_I:
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
            currentPiece.public -> type = TETROMINO_I;
            currentPiece.type = TETROMINO_I;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.move[COORD_Y] = 0;

            break;

        case TETROMINO_J:
            /*
             *     ---------------------
             *  0  |    |    |    |    |
             *     ---------------------
             *  1  | b1 |    |    |    |
             *     ---------------------
             *  2  | b2 | b3 | b4 |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
            currentPiece.public -> type = TETROMINO_J;
            currentPiece.type = TETROMINO_J;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.move[COORD_Y] = 0;

            break;

        case TETROMINO_L:
            /*
             *     ---------------------
             *  0  |    |    |    |    |
             *     ---------------------
             *  1  |    |    | b1 |    |
             *     ---------------------
             *  2  | b4 | b3 | b2 |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
            currentPiece.public -> type = TETROMINO_L;
            currentPiece.type = TETROMINO_L;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.move[COORD_Y] = 0;
            break;


        case TETROMINO_O:
            /*
             *     ---------------------
             *  0  | b1 | b2 |    |    |
             *     ---------------------
             *  1  | b3 | b4 |    |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
            currentPiece.public -> type = TETROMINO_O;
            currentPiece.type = TETROMINO_O;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 1;

            // Y coordinates
            currentPiece.move[COORD_Y] = 1;
            break;

        case TETROMINO_S:
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
            currentPiece.public -> type = TETROMINO_S;
            currentPiece.type = TETROMINO_S;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.move[COORD_Y] = 1;
            break;

        case TETROMINO_T:
            /*
             *     ---------------------
             *  0  |    | b1 |    |    |
             *     ---------------------
             *  1  | b2 | b3 | b4 |    |
             *     ---------------------
             *  2  |    |    |    |    |
             *     ---------------------
             *  3  |    |    |    |    |
             *     ---------------------
             */
            currentPiece.public -> type = TETROMINO_T;
            currentPiece.type = TETROMINO_T;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 2;

            // Y coordinates
            currentPiece.move[COORD_Y] = 1;
            break;

        case TETROMINO_Z:
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
            currentPiece.public -> type = TETROMINO_Z;
            currentPiece.type = TETROMINO_Z;

            // X coordinates
            currentPiece.move[COORD_X] = currentPiece.board.width / 2 - 1;

            // Y coordinates
            currentPiece.move[COORD_Y] = 1;
            break;

            // Bad piece
        default:
            exitStatus = -1;
            break;
    }

    return exitStatus;
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
        (currentPiece.move[coord])++;
    }

        // -1 in coord axis
    else if ( pm == MINUS )
    {
        (currentPiece.move[coord])--;
    }
}

/**
 * @brief Updates the piece in the board (called from public PIECE)
 * 
 * @param None
 * 
 * @return Nothing
 */
static int
normalDrop (void)
{
    int cellType = CELL_MOVING;

    // Drop the piece one position
    moveOneCell(COORD_Y, PLUS);

    // Check if no other already fixed piece is on this one's path
    if ( verifyFixedPieces() > 0 )
    {
        // Another piece is blocking this one
        // Restore the previous position
        moveOneCell(COORD_Y, MINUS);

        // Update public coordinates to avoid an error when updating the board
        updatePublicCoordinates();

        // Clear the piece as moving from the board
        //currentPiece.board.pBoard -> piece.clear.moving();

        // And fix it according to its type
        switch ( currentPiece.type )
        {
            case TETROMINO_I:
                cellType = CELL_I;
                break;

            case TETROMINO_J:
                cellType = CELL_J;
                break;

            case TETROMINO_L:
                cellType = CELL_L;
                break;

            case TETROMINO_O:
                cellType = CELL_O;
                break;

            case TETROMINO_S:
                cellType = CELL_S;
                break;

            case TETROMINO_T:
                cellType = CELL_T;
                break;

            case TETROMINO_Z:
                cellType = CELL_Z;
                break;

            default:
                cellType = CELL_MOVING;
                break;
        }

        //currentPiece.board.pBoard -> piece.set.fixed(cellType);

        //destroy();

    }

    else
    {
        // No other piece is blocking this one, so update the public coords
        updatePublicCoordinates();
    }

    return cellType;
}

/**
 * @brief Rotate the piece in the desired direction
 * 
 * Rotating the piece to the left equals to decrementing its orientations once,
 * rotating it to the right, it's the opposite.
 * 
 * @param direction Direction in which the piece should be rotated 
 * (LEFT or RIGHT)
 * 
 * @return Nothing
 */
static void
rotate (int direction)
{
    switch ( direction )
    {
        case LEFT:
            // Decrement orientation once
            updateOrientation(MINUS);

            // If conflicts occur with another piece, revert changes
            if ( verifyFixedPieces() )
            {
                updateOrientation(PLUS);
            }
            break;

        case RIGHT:
            // Increment orientation once
            updateOrientation(PLUS);

            // If conflicts occur with another piece, revert changes
            if ( verifyFixedPieces() )
            {
                updateOrientation(MINUS);
            }
            break;

        default:
            break;
    }

    // Update public coordinates
    updatePublicCoordinates();
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
    switch ( direction )
    {
        case LEFT:
            moveOneCell(COORD_X, MINUS);

            // If the piece can't be left in the new position, reset it
            if ( verifyFixedPieces() )
            {
                moveOneCell(COORD_X, PLUS);
            }

            break;

        case RIGHT:
            moveOneCell(COORD_X, PLUS);

            // If the piece can't be left in the new position, reset it
            if ( verifyFixedPieces() )
            {
                moveOneCell(COORD_X, MINUS);
            }

            break;

        default:
            break;
    }

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
    currentPiece.board.pBoard -> update();
}

/**
 * @brief Update piece orientation
 * 
 * @param pm PLUS or MINUS. Change the piece orientation to the next or
 * previous one in the array
 * 
 * @return Nothing
 */
static void
updateOrientation (int pm)
{
    switch ( pm )
    {
            // Previous
        case MINUS:
            // If the first one is reached, go back to the last
            if ( currentPiece.orientation == 0 )
            {
                currentPiece.orientation = ORIENTATION - 1;
            }
            else
            {
                currentPiece.orientation--;
            }
            break;

            // Next
        case PLUS:
            // If the last one is reached, go back to the first
            if ( currentPiece.orientation == ORIENTATION - 1 )
            {
                currentPiece.orientation = 0;
            }
            else
            {
                currentPiece.orientation++;
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Update coordinates in PIECE structure
 * 
 * Uses the private coordinates, which are gathered adding movements to the
 * origin position of the piece, to update the public ones.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
updatePublicCoordinates (void)
{
    // Block b1
    // X
    currentPiece.public -> get.coordinates[b1][COORD_X] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b1][COORD_X] + currentPiece.move[COORD_X];
    // Y
    currentPiece.public -> get.coordinates[b1][COORD_Y] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b1][COORD_Y] + currentPiece.move[COORD_Y];

    // Block b2
    // X
    currentPiece.public -> get.coordinates[b2][COORD_X] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b2][COORD_X] + currentPiece.move[COORD_X];
    // Y
    currentPiece.public -> get.coordinates[b2][COORD_Y] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b2][COORD_Y] + currentPiece.move[COORD_Y];

    // Block b3
    // X
    currentPiece.public -> get.coordinates[b3][COORD_X] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b3][COORD_X] + currentPiece.move[COORD_X];
    // Y
    currentPiece.public -> get.coordinates[b3][COORD_Y] = \
               pieceArr[currentPiece.type][currentPiece.orientation]\
                [b3][COORD_Y] + currentPiece.move[COORD_Y];

    // Block b4
    // X
    currentPiece.public -> get.coordinates[b4][COORD_X] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b4][COORD_X] + currentPiece.move[COORD_X];
    // Y
    currentPiece.public -> get.coordinates[b4][COORD_Y] = \
                pieceArr[currentPiece.type][currentPiece.orientation]\
                [b4][COORD_Y] + currentPiece.move[COORD_Y];
}

/**
 * @brief Given a piece a new position, check if it can be keeped there or not.
 * 
 * Verify if no superposition is made after performing an action with a piece,
 * like rotating, shifting or dropping.
 * 
 * @return Error: -1
 * @return Success: 0
 * @return Fail: 1
 */
static int
verifyFixedPieces (void)
{
    //
    int i;
    int count = 0, ans = -1;
    int x, y;

    for ( i = 0; i < BLOCKS; i++ )
    {
        // X coordinate of the block given by i counter plus how many times it
        // was moved in this axis
        x = pieceArr[currentPiece.type][currentPiece.orientation][ \
                i][COORD_X] + currentPiece.move[COORD_X];

        // Y coordinate of the block given by i counter plus how many times it
        // was moved in this axis
        y = pieceArr[currentPiece.type][currentPiece.orientation][ \
                i][COORD_Y] + currentPiece.move[COORD_Y];

        // Does it get out of the board?
        if ( (x < currentPiece.board.width) &&
             (y < currentPiece.board.height) &&
             x >= 0 && y >= 0 )
        {
            // If not, does the desired board cell have a fixed block?
            // count++ if no fixed block is found
            (CELL(x, y) > CELL_CLEAR) ? (count = -1) : (count++);

        }
        else
        {
            count = -1;
        }
    }

    // When all blocks of the TETROMINO have it's path clear, return 0
    (count == BLOCKS) ? (ans = 0) : (ans = 1);

    return ans;
}
