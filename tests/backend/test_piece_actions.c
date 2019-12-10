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

// For TETROMINO_X, NUM_PIECES, board_cell, coords and board_t
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
// Test each piece shifting
void test4 (void);
// Test each piece rotation
void test5 (void);

// === Function prototypes for private functions with file level scope ===
// Clears the board and initializes only one testPiece.
static int conf_onePiece (void);
// Set all the cells in the board to CELL_CLEAR.
static void clearBoard (void);
// Print the board as it is.
static void printBoard (void);

// Check if there is only a moving piece on the given coordinates
static void
check_onePieceMoving (int b1[COORD_NUM], int b2[COORD_NUM],
                      int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if there is only a fixed piece on the given coordinates
static void
check_onePieceFixed (int b1[COORD_NUM], int b2[COORD_NUM],
                     int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if gravity works on one testPiece at the time.
static void
check_Gravity (int b1[COORD_NUM], int b2[COORD_NUM],
               int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if shifting works on one testPiece at the time. 
static void
check_Shift (int b1[COORD_NUM], int b2[COORD_NUM],
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

// Check if rotation works on tetromino I.
static void
check_rotationI (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino J.
static void
check_rotationJ (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino L.
static void
check_rotationL (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino O.
static void
check_rotationO (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino S.
static void
check_rotationS (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino T.
static void
check_rotationT (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

// Check if rotation works on tetromino Z.
static void
check_rotationZ (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM]);

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
setFixed (int cellType);

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
static char bag[TETROMINOS];
// Position of each piece in the bag
static int position;

// === Global function definitions ===

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
         (NULL == CU_add_test(pSuite, "test3: Gravity", test3)) ||
         (NULL == CU_add_test(pSuite, "test4: Shifting", test4)) ||
         (NULL == CU_add_test(pSuite, "test5: Rotating", test5)) )
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

    // Create a bag with all pieces, in order
    bag[0] = TETROMINO_I;
    bag[1] = TETROMINO_J;
    bag[2] = TETROMINO_L;
    bag[3] = TETROMINO_O;
    bag[4] = TETROMINO_S;
    bag[5] = TETROMINO_T;
    bag[6] = TETROMINO_Z;

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
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_I);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_J);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_L);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_O);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_MOVING, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_I);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_J);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_L);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_O);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_CLEAR, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_J);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_L);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_O);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_I, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_J, CELL_L);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_J, CELL_O);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_J, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_J, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_J, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_L, CELL_O);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_L, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_L, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_L, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_O, CELL_S);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_O, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_O, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_S, CELL_T);
    CU_ASSERT_NOT_EQUAL_FATAL(CELL_S, CELL_Z);

    CU_ASSERT_NOT_EQUAL_FATAL(CELL_T, CELL_Z);
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
    // TETROMINO_I
    onlyPiece_I(&check_onePieceMoving);

    // TETROMINO_J
    onlyPiece_J(&check_onePieceMoving);

    // TETROMINO_L
    onlyPiece_L(&check_onePieceMoving);

    // TETROMINO_O
    onlyPiece_O(&check_onePieceMoving);

    // TETROMINO_S
    onlyPiece_S(&check_onePieceMoving);

    // TETROMINO_T
    onlyPiece_T(&check_onePieceMoving);

    // TETROMINO_Z
    onlyPiece_Z(&check_onePieceMoving);
}

/**
 * @brief Test each piece gravity.
 * 
 * The piece is made appear like in test2() and is "left alone" to free fall,
 * testing on every board update if the whole piece is where it's supposed to.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test3 (void)
{
    // TETROMINO_I
    onlyPiece_I(&check_Gravity);

    // TETROMINO_J
    onlyPiece_J(&check_Gravity);

    // TETROMINO_L
    onlyPiece_L(&check_Gravity);

    // TETROMINO_O
    onlyPiece_O(&check_Gravity);

    // TETROMINO_S
    onlyPiece_S(&check_Gravity);

    // TETROMINO_T
    onlyPiece_T(&check_Gravity);

    // TETROMINO_Z
    onlyPiece_Z(&check_Gravity);
}

/**
 * @brief Test each piece shifting
 * 
 * The piece appears at the top of the board and is shifted a little before 
 * letting it drop until it reaches the bottom. On each drop is again shifted
 * and every time it's tested to be where is supposed to in the board. 
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test4 (void)
{
    // TETROMINO_I
    onlyPiece_I(&check_Shift);

    // TETROMINO_J
    onlyPiece_J(&check_Shift);

    // TETROMINO_L
    onlyPiece_L(&check_Shift);

    // TETROMINO_O
    onlyPiece_O(&check_Shift);

    // TETROMINO_S
    onlyPiece_S(&check_Shift);

    // TETROMINO_T
    onlyPiece_T(&check_Shift);

    // TETROMINO_Z
    onlyPiece_Z(&check_Shift);
}

/**
 * @brief Test each piece rotation
 * 
 * The piece appears at the top of the board and is rotated once to the right,
 * then it's dropped once and rotated again to the right, and the process is
 * repeated with each possible position of the piece. When all right rotations
 * have been performed, the piece is dropped once and the same process is 
 * repeated rotating it to the left. After all left cases were tested, that
 * piece's test is finished.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
test5 (void)
{
    // TETROMINO_I
    onlyPiece_I(&check_rotationI);

    // TETROMINO_J
    onlyPiece_J(&check_rotationJ);

    // TETROMINO_L
    onlyPiece_L(&check_rotationL);

    // TETROMINO_O
    onlyPiece_O(&check_rotationO);

    // TETROMINO_S
    onlyPiece_S(&check_rotationS);

    // TETROMINO_T
    onlyPiece_T(&check_rotationT);

    // TETROMINO_Z
    onlyPiece_Z(&check_rotationZ);
}

// === Local function definitions ===

/**
 * @brief Clears the board and initializes only one testPiece.
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

    if ( piece_init(&testPiece, &bStru,
                    &gboard[0][0], H_BOARD, W_BOARD,
                    bag[position]) )
    {
        return -1;
    }

    testPiece.update();
    clearMoving();
    setMoving();

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

    putchar('\n');

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

    // Update board
    clearMoving();
    setMoving();

    // Perform test
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

    // In case the board hasn't been updated
    clearMoving();

    for ( i = 0; i < H_BOARD; i++ )
    {
        for ( j = 0; j < W_BOARD; j++ )
        {
            if ( (i == b1[COORD_Y] && j == b1[COORD_X]) ||
                 (i == b2[COORD_Y] && j == b2[COORD_X]) ||
                 (i == b3[COORD_Y] && j == b3[COORD_X]) ||
                 (i == b4[COORD_Y] && j == b4[COORD_X]) )
            {
                // Fix type for each piece
                switch ( testPiece.type )
                {
                    case TETROMINO_I:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_I);
                        break;

                    case TETROMINO_J:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_J);
                        break;

                    case TETROMINO_L:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_L);
                        break;

                    case TETROMINO_O:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_O);
                        break;

                    case TETROMINO_S:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_S);
                        break;

                    case TETROMINO_T:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_T);
                        break;

                    case TETROMINO_Z:
                        CU_ASSERT_EQUAL(gboard[i][j], CELL_Z);
                        break;
                }

            }

            else
            {
                CU_ASSERT_EQUAL(gboard[i][j], CELL_CLEAR);
            }
        }
    }
}

/**
 * @brief Check if gravity works on one testPiece at the time. 
 * 
 * It's passed as a pointer to functions in test3
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_Gravity (int b1[COORD_NUM], int b2[COORD_NUM],
               int b3[COORD_NUM], int b4[COORD_NUM])
{
    int i, cellType;

    for ( i = 3; i < H_BOARD; i++ )
    {
        cellType = testPiece.update();

        // Last drop (must be fixed after that)
        if ( i == H_BOARD - 1 && testPiece.type != TETROMINO_I )
        {
            setFixed(cellType);
            check_onePieceFixed(b1, b2, b3, b4);
        }

            // TETROMINO I has a special condition on the end because it's
            // dropped leaving an extra space from the bottom
        else if ( i == H_BOARD - 1 && testPiece.type == TETROMINO_I )
        {
            (b1[COORD_Y]) += 1;
            (b2[COORD_Y]) += 1;
            (b3[COORD_Y]) += 1;
            (b4[COORD_Y]) += 1;

            check_onePieceMoving(b1, b2, b3, b4);

            cellType = testPiece.update();

            setFixed(cellType);
            check_onePieceFixed(b1, b2, b3, b4);
        }

            // Piece hasn't reached the bottom
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
 * @brief Check if shifting works on one testPiece at the time. 
 * 
 * It's passed as a pointer to functions in test4
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_Shift (int b1[COORD_NUM], int b2[COORD_NUM],
             int b3[COORD_NUM], int b4[COORD_NUM])
{
    int i, limit_r, limit_l;

    // Depending on the piece, this should be displaced no more than 3 or 4 
    // blocks to the right and 6, 7 or 8 blocks to the left
    switch ( testPiece.type )
    {
        case TETROMINO_I:
            limit_r = W_BOARD / 2 - 2;
            limit_l = W_BOARD - 4;
            break;

        case TETROMINO_J:
        case TETROMINO_L:
        case TETROMINO_S:
        case TETROMINO_T:
            limit_r = W_BOARD / 2 - 1;
            limit_l = W_BOARD - 3;
            break;

        case TETROMINO_O:
            limit_r = W_BOARD / 2 - 1;
            limit_l = W_BOARD - 2;
            break;

        case TETROMINO_Z:
            limit_r = W_BOARD / 2 - 2;
            limit_l = W_BOARD - 3;

        default:
            break;
    }

    // Displace the piece fully to the right
    for ( i = 0; i < limit_r; i++ )
    {
        testPiece.shift(RIGHT);

        (b1[COORD_X]) += 1;
        (b2[COORD_X]) += 1;
        (b3[COORD_X]) += 1;
        (b4[COORD_X]) += 1;

        check_onePieceMoving(b1, b2, b3, b4);
    }

    // It must keep itself in the same position
    for ( i = 0; i < 10; i++ )
    {
        testPiece.shift(RIGHT);
        check_onePieceMoving(b1, b2, b3, b4);
    }

    // Drop it once
    testPiece.update();

    // And check if it's where it's supposed to
    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Displace the piece fully to the left
    for ( i = limit_l; i > 0; i-- )
    {
        testPiece.shift(LEFT);

        (b1[COORD_X]) -= 1;
        (b2[COORD_X]) -= 1;
        (b3[COORD_X]) -= 1;
        (b4[COORD_X]) -= 1;

        check_onePieceMoving(b1, b2, b3, b4);
    }

    // It must keep itself in the same position
    for ( i = 0; i < 10; i++ )
    {
        testPiece.shift(LEFT);
        check_onePieceMoving(b1, b2, b3, b4);
    }

    // Drop it once
    testPiece.update();

    // And check if it's where it's supposed to
    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);
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

    // TETROMINO_I
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
     *  1  |    |    |    |    |
     *     ---------------------
     *  2  | b1 |    |    |    |
     *     ---------------------
     *  3  | b2 | b3 | b4 |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 3;
    b2[COORD_X] = 3;
    b3[COORD_X] = 4;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 3;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

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
     *  1  |    |    |    |    |
     *     ---------------------
     *  2  |    |    | b1 |    |
     *     ---------------------
     *  3  | b4 | b3 | b2 |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 5;
    b2[COORD_X] = 5;
    b3[COORD_X] = 4;
    b4[COORD_X] = 3;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 3;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

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
     *  1  | b1 | b2 |    |    |
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
    b3[COORD_X] = 4;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 2;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

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

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 2;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

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
     *  1  |    | b1 |    |    |
     *     ---------------------
     *  2  | b2 | b3 | b4 |    |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 4;
    b2[COORD_X] = 3;
    b3[COORD_X] = 4;
    b4[COORD_X] = 5;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 3;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

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
     *  1  |    | b1 | b2 |    |
     *     ---------------------
     *  2  |    |    | b3 | b4 |
     *     ---------------------
     *  3  |    |    |    |    |
     *     ---------------------
     *  4  |    |    |    |    |
     *     ---------------------
     */

    // Coordinates
    b1[COORD_X] = 4;
    b2[COORD_X] = 5;
    b3[COORD_X] = 5;
    b4[COORD_X] = 6;

    b1[COORD_Y] = 2;
    b2[COORD_Y] = 2;
    b3[COORD_Y] = 3;
    b4[COORD_Y] = 3;

    // Perform test
    (*test2Perform)(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino I.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationI (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 2;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) -= 2;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) += 2;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) -= 2;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino J.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationJ (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 2;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 2;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 2;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) -= 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) -= 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 2;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino L.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationL (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 2;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 2;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 2;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;


    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 2;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) -= 2;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) -= 2;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 2;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 0;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 2;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino O.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationO (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino S.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationS (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) += 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 2;
    (b3[COORD_Y]) -= 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 0;
    (b3[COORD_X]) -= 1;
    (b4[COORD_X]) += 0;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 2;
    (b3[COORD_Y]) += 1;
    (b4[COORD_Y]) += 0;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino T.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationT (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 1;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 1;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) -= 1;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

/**
 * @brief Check if rotation works on tetromino Z.
 * 
 * It's called from a function in test5.
 * 
 * @param b1 Coordinates of block b1
 * @param b2 Coordinates of block b2
 * @param b3 Coordinates of block b3
 * @param b4 Coordinates of block b4
 * 
 * @return Nothing
 */
static void
check_rotationZ (int b1[COORD_NUM], int b2[COORD_NUM],
                 int b3[COORD_NUM], int b4[COORD_NUM])
{
    // Position 1 to 2
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 3
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 4
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 1
    testPiece.rotate(RIGHT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 1 to 4
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 4 to 3
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 3 to 2
    testPiece.rotate(LEFT);

    (b1[COORD_X]) += 2;
    (b2[COORD_X]) += 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) -= 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) += 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) += 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);

    // Position 2 to 1
    testPiece.rotate(LEFT);

    (b1[COORD_X]) -= 2;
    (b2[COORD_X]) -= 1;
    (b3[COORD_X]) += 0;
    (b4[COORD_X]) += 1;

    (b1[COORD_Y]) += 0;
    (b2[COORD_Y]) -= 1;
    (b3[COORD_Y]) += 0;
    (b4[COORD_Y]) -= 1;

    check_onePieceMoving(b1, b2, b3, b4);

    // Drop it once
    testPiece.update();

    (b1[COORD_Y])++;
    (b2[COORD_Y])++;
    (b3[COORD_Y])++;
    (b4[COORD_Y])++;

    check_onePieceMoving(b1, b2, b3, b4);
}

// === Copied from board.c ===

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
 * @param None
 * 
 * @return Nothing
 */
static void
setMoving (void)
{
    gboard[ testPiece.get.coordinates[b1][COORD_Y] ][ \
                        testPiece.get.coordinates[b1][COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.get.coordinates[b2][COORD_Y] ][ \
                        testPiece.get.coordinates[b2][COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.get.coordinates[b3][COORD_Y] ][ \
                        testPiece.get.coordinates[b3][COORD_X] ] = CELL_MOVING;

    gboard[ testPiece.get.coordinates[b4][COORD_Y] ][ \
                        testPiece.get.coordinates[b4][COORD_X] ] = CELL_MOVING;
}

/**
 * @brief Set the piece's coordinates as CELL_FIXED on the board
 *  
 * @param None
 * 
 * @return Nothing
 */
static void
setFixed (int cellType)
{
    if ( cellType >= CELL_I )
    {
        gboard[ testPiece.get.coordinates[b1][COORD_Y] ][ \
                        testPiece.get.coordinates[b1][COORD_X] ] = cellType;

        gboard[ testPiece.get.coordinates[b2][COORD_Y] ][ \
                        testPiece.get.coordinates[b2][COORD_X] ] = cellType;

        gboard[ testPiece.get.coordinates[b3][COORD_Y] ][ \
                        testPiece.get.coordinates[b3][COORD_X] ] = cellType;

        gboard[ testPiece.get.coordinates[b4][COORD_Y] ][ \
                        testPiece.get.coordinates[b4][COORD_X] ] = cellType;
    }

    else
    {
        fputs("Invalid cellType for setFixed()", stderr);
    }
}
