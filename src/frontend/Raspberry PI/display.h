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

// === Constants and Macro definitions ===
///@def PLAYBOARD_HEIGHT
///@brief Playboard height
#    define MAXH 20

///@def PLAYBOARD_WIDTH
///@brief Playboard width
#    define MAXW 10

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
    ///Principal menu.
    PLAY = 1, MODE, SCORE, SND, EXIT, 
    ///Options on SND.
    ON = 7, OFF, 
    ///Options on MODE.
    EASY = 10, HARD
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
 * @brief Print a game matrix.
 * 
 * It shows on the display the matrix where the game is developed.
 * 
 * @param Game matrix.
 * 
 * @return Nothing
 */
void
printB(int matrix[MAXH][MAXW]); 

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
 * @return Nothing
*/
void
printW(words_t word, int x, int y);

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



#endif /* DISPLAY_H */
