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

/**
 * 
 * @file    test_piece_actions.c
 * 
 * @brief   Test piece appearing, gravity, soft drop, shifting and rotating
 * 
 * @details Tests are carried on using CUnit Test Suite
 * 
 * @authors Gino Minnucci                               <gminnucci@itba.edu.ar>
 *          Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    29/11/2019, 09:27
 *
 * @copyright GNU General Public License v3
 */

/// @publicsection
// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

// For PIECE_X, NUM_PIECES, board_cell, coords and board_t
#include "../../src/backend/board/board.h"

// Functions to test
#include "../../src/backend/board/pieces/piece_actions.h"

// === Constants and Macro definitions ===
/*
 * @def H_BOARD
 * @brief Test board height. Should be equal to MBOARD_H defined in board.c
 */
#define H_BOARD 23

/*
 * @def W_BOARD
 * @brief Test board width
 */
#define W_BOARD 10

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes with global level scope ===
// Initializes this test suite
int init_suite (void);
// Cleans suite before exiting
int clean_suite (void);

// Test three basic definitions needed to perform the rest of the tests.
void test1 (void);
// Test each piece initialization
void test2 (void);
// Test each piece gravity.
void test3 (void);

// === Function prototypes for private functions with file level scope ===
// Clears the board and initializes only one piece.
static int conf_onePiece (void);
// Set all the cells in the board to CELL_CLEAR.
static void clearBoard (void);
// Print the board as it is.
static void printBoard (void);

// Check if there is only a moving piece on the given coordinates
static void
check_onePieceMoving (int b1[COORD_NUM], int b2[COORD_NUM],
                      int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if gravity works on one piece. 
static void
check_Gravity (int b1[COORD_NUM], int b2[COORD_NUM],
               int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if there is only a fixed piece on the given coordinates
static void
check_onePieceFixed (int b1[COORD_NUM], int b2[COORD_NUM],
                     int b3[COORD_NUM], int b4[COORD_NUM]);

// Initialize and test a single I piece on the board.
static void
onlyPiece_I (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single J piece on the board.
static void
onlyPiece_J (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single L piece on the board.
static void
onlyPiece_L (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single O piece on the board.
static void
onlyPiece_O (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single S piece on the board.
static void
onlyPiece_S (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single T piece on the board.
static void
onlyPiece_T (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

// Initialize and test a single Z piece on the board.
static void
onlyPiece_Z (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]));

/*
 * clearMoving, setFixed, setMoving functions are copy-pasted from board.c file
 * as this test is only meant to test pieces, not board functions but this are
 * required by testPiece.update()
 */
// Clear all cells with moving pieces on the board
static void
clearMoving (void);

// Set the piece's coordinates as CELL_FIXED on the board
static void
setFixed (void);

// Set the piece's coordinates as CELL_MOVING on the board
static void
setMoving (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// Board structure
static board_t bStru;
// Board
static int gboard[H_BOARD][W_BOARD];

// Piece structure where the piece to test is loaded
static piece_t testPiece;

// Bag with pieces
static char bag[NUM_PIECES];
// Position of each piece in the bag
static int position;

// === Global function definitions ===

/*
 * TODO Test piece rotation and shifting
 */

/**
 * @brief Test suites initialization, starting and clearing
 * 
 * Code automatically generated by NetBeans
 * 
 * @param None
 * @return CU_get_error()
 */
int
main ()
{
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if ( CUE_SUCCESS != CU_initialize_registry() )
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("test_random_gen", init_suite, clean_suite);
    if ( NULL == pSuite )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ( (NULL == CU_add_test(pSuite, "test1: CELL definitions", test1)) ||
         (NULL == CU_add_test(pSuite, "test2: Initializing a piece", test2)) ||
         (NULL == CU_add_test(pSuite, "test3: Gravity", test3)) )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

// === Suite definitions ===

/**
 * @brief Initializes this test suite
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
int
init_suite (void)
{
    board_init(&bStru);

    // Replace function pointer to another in this file as we're performing
    // tests on pieces, not on board
    bStru.piece.clear.moving = &clearMoving;
    bStru.piece.set.moving = &setMoving;
    bStru.piece.set.fixed = &setFixed;

    // Create a bag with all pieces, in order
    bag[0] = PIECE_I;
    bag[1] = PIECE_J;
    bag[2] = PIECE_L;
    bag[3] = PIECE_O;
    bag[4] = PIECE_S;
    bag[5] = PIECE_T;
    bag[6] = PIECE_Z;

    // Clear board
    clearBoard();

    return 0;
}

/*
 * @brief Cleans suite before exiting
 * 
 * @return Success: 0
 * @return Fail: Non 0
 */
int
clean_suite (void)
{
    return 0;
}

// === Tests definitions ===

/**
 * @brief Test three basic definitions needed to perform the rest of the tests.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test1 ()
{
    // This three must be different in order to be able to difference cells on 
    // the board
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_CLEAR);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_FIXED);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_FIXED, CELL_CLEAR);
}

/**
 * @brief Test each piece initialization
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test2 ()
{
    // PIECE_I
    onlyPiece_I(&check_onePieceMoving);

    // PIECE_J
    onlyPiece_J(&check_onePieceMoving);

    // PIECE_L
    onlyPiece_L(&check_onePieceMoving);

    // PIECE_O
    onlyPiece_O(&check_onePieceMoving);

    // PIECE_S
    onlyPiece_S(&check_onePieceMoving);

    // PIECE_T
    onlyPiece_T(&check_onePieceMoving);

    // PIECE_Z
    onlyPiece_Z(&check_onePieceMoving);
}

/**
 * @brief Test each piece gravity.
 * 
 * The piece is made appear like in test2() and is "left" to free fall, testing
 * on every board update if the whole piece is where it's supposed to.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test3 (void)
{
    // PIECE_I
    onlyPiece_I(&check_Gravity);

    // PIECE_J
    onlyPiece_J(&check_Gravity);

    // PIECE_L
    onlyPiece_L(&check_Gravity);

    // PIECE_O
    onlyPiece_O(&check_Gravity);

    // PIECE_S
    onlyPiece_S(&check_Gravity);

    // PIECE_T
    onlyPiece_T(&check_Gravity);

    // PIECE_Z
    onlyPiece_Z(&check_Gravity);
}

// === Local function definitions ===

/**
 * @brief Clears the board and initializes only one piece.
 * 
 * This function is meant to be called before performing a test with just one
 * piece on the board.
 * 
 * @param None
 * @return Success: 0
 * @return Fail: Non 0
 */
static int
conf_onePiece (void)
{
    clearBoard();

    if ( piece_init(&testPiece, bag, position) )
    {
        return -1;
    }

    testPiece.update(&bStru, gboard, H_BOARD);
    return 0;
}

/**
 * @brief Set all the cells in the board to CELL_CLEAR.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
clearBoard (void)
{
    int i, j;

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
        {
            gboard[i][j] = CELL_CLEAR;
        }
    }
}

/**
 * @brief Print the board as it is.
 * 
 * This function can be called from wherever it's needed to see the board 
 * status while a test is being performed. By default, is never called as all
 * test are passed.
 * 
 * Consider this function as a "Debug" tool.
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
printBoard (void)
{
    int i, j;

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
        {
            if ( gboard[i][j] < 0 || gboard[i][j] > 9 )
            {
                printf(" %d ", gboard[i][j]);
            }

            else
            {
                printf("  %d ", gboard[i][j]);
            }
        }
        putchar('\n');
    }
    putchar('\n');
}

/**
 * @brief Check if there is only a moving piece on the given coordinates
 * 
 * It's passed as a pointer to function on test2 and check_Gravity() also 
 * depends on this function
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * @return Nothing
 */
static void
check_onePieceMoving (int b1[COORD_NUM], int b2[COORD_NUM],
                      int b3[COORD_NUM], int b4[COORD_NUM])
{
    int i, j;

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
        {
            if ( (i == b1[COORD_Y] && j == b1[COORD_X]) ||
                 (i == b2[COORD_Y] && j == b2[COORD_X]) ||
                 (i == b3[COORD_Y] && j == b3[COORD_X]) ||
                 (i == b4[COORD_Y] && j == b4[COORD_X]) )
            {
                CU_ASSERT_EQUAL(gboard[i][j], CELL_MOVING);
            }

            else
            {
                CU_ASSERT_EQUAL(gboard[i][j], CELL_CLEAR);
            }
        }
    }
}

/**
 * @brief Check if gravity works on one piece. 
 * 
 * It's passed as a pointer to function test3
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * @return Nothing
 */
static void
check_Gravity (int b1[COORD_NUM], int b2[COORD_NUM],
               int b3[COORD_NUM], int b4[COORD_NUM])
{
    int i;

    for ( i = 3; i <= H_BOARD; i++ )
    {
        testPiece.update(&bStru, gboard, H_BOARD);

        if ( i == H_BOARD )
        {
            check_onePieceFixed(b1, b2, b3, b4);
        }
        else
        {
            (b1[COORD_Y]) += 1;
            (b2[COORD_Y]) += 1;
            (b3[COORD_Y]) += 1;
            (b4[COORD_Y]) += 1;

            check_onePieceMoving(b1, b2, b3, b4);
        }
    }
}

/**
 * @brief Check if there is only a fixed piece on the given coordinates
 * 
 * Function check_Gravity() depends on this one
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * @return Nothing
 */
static void
check_onePieceFixed (int b1[COORD_NUM], int b2[COORD_NUM],
                     int b3[COORD_NUM], int b4[COORD_NUM])
{
    int i, j;

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
        {
            if ( (i == b1[COORD_Y] && j == b1[COORD_X]) ||
                 (i == b2[COORD_Y] && j == b2[COORD_X]) ||
                 (i == b3[COORD_Y] && j == b3[COORD_X]) ||
                 (i == b4[COORD_Y] && j == b4[COORD_X]) )
            {
                CU_ASSERT_EQUAL(gboard[i][j], CELL_FIXED);
            }

            else
            {
                CU_ASSERT_EQUAL(gboard[i][j], CELL_CLEAR);
            }
        }
    }
}

/**
 * @brief Initialize and test a single I piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_I (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    // PIECE_I
    position = 0;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  |    |    |    |    |
     *     ---------------------
     *  2  | b1 | b2 | b3 | b4 |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 4;
    b3[COORD_X] = 5;
    b4[COORD_X] = 6;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 2;
    b3[COORD_Y] = 2;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single J piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_J (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece cordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    // Piece J
    position = 1;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  | b1 | b2 | b3 |    |
     *     ---------------------
     *  2  |    |    | b4 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 4;
    b3[COORD_X] = 5;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 1;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single L piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_L (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    position = 2;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  | b1 | b2 | b3 |    |
     *     ---------------------
     *  2  | b4 |    |    |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 4;
    b3[COORD_X] = 5;
    b4[COORD_X] = 3;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 1;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single O piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_O (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    position = 3;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  |    | b1 | b2 |    |
     *     ---------------------
     *  2  |    | b3 | b4 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 4;
    b2[COORD_X] = 5;
    b3[COORD_X] = 4;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 2;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single S piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_S (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    position = 4;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  |    | b1 | b2 |    |
     *     ---------------------
     *  2  | b3 | b4 |    |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 4;
    b2[COORD_X] = 5;
    b3[COORD_X] = 3;
    b4[COORD_X] = 4;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 2;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single T piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_T (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    position = 5;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  | b1 | b2 | b3 |    |
     *     ---------------------
     *  2  |    | b4 |    |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 4;
    b3[COORD_X] = 5;
    b4[COORD_X] = 4;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 1;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Initialize and test a single Z piece on the board.
 * 
 * The piece is initialized on the board and then it is performed the test
 * on @p test2Perform function.
 * 
 * @param test2Perform Pointer to function with prototype 
 * <b> void function(int b1[COORD_NUM], int b2[COORD_NUM], int b3[COORD_NUM], 
 * int b4[COORD_NUM]) </b>
 * @return Nothing
 */
static void
onlyPiece_Z (void (*test2Perform)(int b1[COORD_NUM], int b2[COORD_NUM],
                                  int b3[COORD_NUM], int b4[COORD_NUM]))
{
    // Piece coordinates
    int b1[COORD_NUM], b2[COORD_NUM];
    int b3[COORD_NUM], b4[COORD_NUM];

    position = 6;
    conf_onePiece();

    /*
     *       3    4    5    6
     *     ---------------------
     *  1  | b1 | b2 |    |    |
     *     ---------------------
     *  2  |    | b3 | b4 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 4;
    b3[COORD_X] = 4;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 1;
    b2[COORD_Y] = 1;
    b3[COORD_Y] = 2;
    b4[COORD_Y] = 2;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

// === Copied from board.c ===

/**
 * @brief Clear all cells with moving pieces on the board
 * 
 * Extracted as it is from board.c
 *  
 * @param None
 *
 * @return Nothing
 */
static void
clearMoving (void)
{
    int i, j;

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
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
 * Extracted as it is from board.c
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
setMoving (void)
{
    gboard[ testPiece.coord.b1[COORD_Y] ][ \
                        testPiece.coord.b1[COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.coord.b2[COORD_Y] ][ \
                        testPiece.coord.b2[COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.coord.b3[COORD_Y] ][ \
                        testPiece.coord.b3[COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.coord.b4[COORD_Y] ][ \
                        testPiece.coord.b4[COORD_X] ] = CELL_MOVING;
}

/**
 * @brief Set the piece's coordinates as CELL_FIXED on the board
 *
 * Extracted as it is from board.c
 *
 * @param None
 * 
 * @return Nothing
 */
static void
setFixed (void)
{
    gboard[ testPiece.coord.b1[COORD_Y] ][ \
                        testPiece.coord.b1[COORD_X] ] = CELL_FIXED;

    gboard[ testPiece.coord.b2[COORD_Y] ][ \
                        testPiece.coord.b2[COORD_X] ] = CELL_FIXED;

    gboard[ testPiece.coord.b3[COORD_Y] ][ \
                        testPiece.coord.b3[COORD_X] ] = CELL_FIXED;

    gboard[ testPiece.coord.b4[COORD_Y] ][ \
                        testPiece.coord.b4[COORD_X] ] = CELL_FIXED;
}
