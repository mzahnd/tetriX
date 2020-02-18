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
 * @file    display.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    10/12/2019, 18:44
 * 
 * @copyright GNU General Public License v3
 */

#ifndef DISPLAY_H
#    define DISPLAY_H

// === Libraries and header files ===

#    include "../../backend/board/board.h"

// === Constants and Macro definitions ===

///@def MAX_COORD
///@brief Max value+1 to coord.x or coord.y
#    define MAX 16

///@def COLUMNS
///@brief Number of columns defined for letter matrices.
#    define LCOLS 5

///@def ROWS
///@brief Number of rows defined for letter matrices.
#    define LROWS LCOLS

///@def SPACE
///@brief Space between each letter printed on the board.
#    define SPACE 4

///@def LEFT DISPLACEMENT
///@brief Max displacement that a word can have from the left border
///       of the display to the left(coordinate x value).
#    define LEFT_DISPLACEMENT 14

///@def RIGHT DISPLACEMENT
///@brief Max displacement that a word can have from the right border
///       of the display to the right(coordinate x value).
#    define RIGHT_DISPLACEMENT 2


// === Enumerations, structures and typedefs ===
///
///@brief Matrices for letters.
typedef int letters_t [LROWS][LCOLS];

///
///@brief Arrays of pointers to matrices of letters.
typedef letters_t* (words_t[]);


///
///@brief Results for sameLetter();

enum
{
    NOT_SAME,
    SAME
};

/**
 * @brief  Order in which every word is defined in an array of
 *         words, separated between menus by NULL.
 */
enum order
{
    ///Options on MAIN MENU.
    PLAY = 1, MODE, SCORE, SOUND, EXIT,
    ///Options on SOUND.
    ON = EXIT+2, OFF,
    ///Options on MODE.
    EASY = OFF+2, MEDIUM, HARD
};

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
/**
 * @brief Initialize the TETRIS menu.
 * 
 * It makes a nice presentation of the game.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
initMenu(void);
/**
 * @brief Print letters on the board.
 * 
 * It prints the matrices of letters starting on the coordinate{x,y}
 * the user decides.
 * 
 * @param Pointer to letters_t.
 * @param Coordinate's X value.
 * @param Coordinate's Y value.
 * 
 * @return Nothing
 */
void
printL(letters_t * letter, int x, int y);

/**
 * @brief Print a game matrix.
 * 
 * It shows on the display the matrix where the game is developed.
 * 
 * @param Game matrix.
 * 
 * @return Nothing
 */
void
printG(int * matrix);

/**
 * @brief Print words.
 * 
 * It prints words(made by letters) on the display with a space
 * between them.
 * 
 * @param The word(a pointer to letters' array).
 * @param Coordinate's X value.
 * @param Coordinate's Y value.
 * 
 * @return Coordinate's X value of the last letter of the word(it could be
 *         bigger than the MAX value).
 */
int
printW(words_t word, int x, int y);

/**
 * @brief Print words with an animation.
 * 
 * It prints words(made by letters) on the display with a space between them.
 * It is similar to printW but this one can show bigger words displacing 
 * the letters from right to left.
 * 
 * @param Pointer to letters_t.
 * @param Coordinate's X value.
 * @param Coordinate's Y value.
 * 
 * @return Nothing
 */
void
printWmove(words_t word, int x, int y);

/**
 * @brief Clear a square in the display. 
 * 
 * It clears a part of the display chosen by the user.
 * It makes a square (widthXheight) on the coordinate the user
 * gives it(the origin is on the top left corner).
 * 
 * @param Square's width.
 * @param Square's height.
 * @param Coordinate's X value.
 * @param Coordinate's Y value.
 * 
 * @return Nothing
 */
void
disp_n_clear(int width, int height, int x, int y);

/**
 * @brief Animation to show the elimination of a line.
 * 
 * It shows an animation of a full line disappearing. 
 * 
 * @param An array with each line that must be cleared.
 * @param The number of the column to be cleared.
 * 
 * @return Nothing
 */
void
lineoff(int line [BOARD_WIDTH], int y);

/**
 * @brief Ending animation.
 * 
 * It makes a nice animation of the end.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
theEnd(void);

#endif /* DISPLAY_H */
