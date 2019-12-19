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
 * @file    display.c
 * 
 * @brief   Global functions are explained in the Header File.
 *          
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    09/12/2019, 19:31
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

///RPI libraries
#include "libs/disdrv.h"
#include "libs/joydrv.h"

///Joystick functions.
#include "joystick.h"

///Defines and enums used from the backend.
#include "../../backend/board/board.h"

// This file
#include "display.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===
/**
 * @brief Matrices 5x5 to represent every letter.
 * 
 */
///Letter T
letters_t tt = {
    {1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0}
};
///Letter E
letters_t ee = {
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0}
};
///Letter R
letters_t rr = {
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0}
};
///Letter I
letters_t ii = {
    {1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0}
};
///Letter S
letters_t ss = {
    {1, 1, 1, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 0, 0}
};
///Letter P
letters_t pp = {
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0}
};
///Letter L
letters_t ll = {
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0}
};
///Letter A
letters_t aa = {
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0}
};
///Letter Y
letters_t yy = {
    {1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0}
};
///Letter M
letters_t mm = {
    {1, 1, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1}
};
///Letter O
letters_t oo = {
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0}
};
///Letter D
letters_t dd = {
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0}
};
///Letter C
letters_t cc = {
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0}
};
///Letter N
letters_t nn = {
    {1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0}
};
///Letter F
letters_t ff = {
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0}
};
///Letter X
letters_t xx = {
    {1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 1, 0, 0}
};
///Letter H
letters_t hh = {
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0}
};
///Letter U
letters_t uu = {
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 1, 0, 0}
};

/*
pie_t lll = {
    {1, 1, 1, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0},
    {0, 0, 1, 1, 0}
};

pie_t sss = {
    {0, 1, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1},
    {0, 1, 0, 1, 0}
};

pie_t ccc = {
    {1, 1, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

pie_t iii = {
    {1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 1, 0, 0, 0}
};

pie_t yyy = {
    {0, 1, 1, 1, 0},
    {0, 0, 1, 0, 0},
    {1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1},
    {1, 0, 0, 0, 1}
};
 */

// === Function prototypes for private functions with file level scope ===

/**
 * @brief Checks if it is the same letter.
 * 
 * It analyzes if every element of two letters(matrices) are
 * the same or not, to check if both letters are the same.
 * 
 * @param A pointer to the first letter.
 * @param A pointer to the second letter.
 * 
 * @return NOT_SAME(0) 
 * @return SAME(1)
 */
int
sameLetter(letters_t * letter1, letters_t * letter2);

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

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===




/// @publicsection

void
disp_n_clear(int width, int height, int x, int y)
{
    int i, j;
    dcoord_t point;

    ///It goes threw cols&rows turning off the leds until it reaches
    ///the end of the square or the border of the display.
    for(i = 0; ((y + i) < MAX)&&(i < height); i++)
    {
        ///Y value will be the origin plus the actual row.
        point.y = (i + y);

        for(j = 0; (j + x) < MAX && (j < width); j++)
        {
            ///X value will be the origin plus the actual column.
            point.x = (j + x);
            ///It turns off the point.
            disp_write(point, D_OFF);
        }
    }
}

int
printW(words_t word, int x, int y)
{
    int k;
    /**
     * It goes threw the array of letter pointers printing the letters
     * until it is at the end of the array or at the border of the display.
     * Between the letters it puts a Space that changes if the letter is 
     * shorter(width).
     */
    for(k = 0; (x < MAX)&&(y < MAX)&&(word[k] != NULL); k++)
    {
        ///Prints the letter.
        printL(word[k], x, y);
        ///If it is letter M it needs more space.
        if(sameLetter(word[k], &mm))
        {
            x += 2;
        }
            ///If it is a shorter letter, it doesn't as much space as the others
        else if(((*(word[k]))[4][2] == 0)&&(!sameLetter(word[k], &pp)))
        {
            x -= 1;
        }
        ///It adds the space between letters.    
        x += SPACE;
    }
    ///It returns with the value of the last point of the letter.(In case it is
    ///a long word this number is bigger than the MAX)
    return x;
}

void
printWmove(words_t word, int xo, int y)
{
    ///X is the coordinate of the last letter of the word.
    int x;
    ///Next is a flag to know if the last coordinate x of the last letter
    ///has an acceptable displacement.
    int next;
    ///Jmove takes surf() value(so as to avoid calling the function 
    ///so many times)
    int jmove;

    ///It doesn't continue until the joystick  is in the center(as in rpcontrol)
    while(surf() != CENTER)
    {
        joy_update();
    }

    jmove = surf();

    ///Unless the user moves the joystick to the left, right or press
    ///the switch button, it show the full word going from one side
    ///to the other.
    while(jmove != PRESSED && jmove != LEFT && jmove != RIGHT)
    {
        ///It clears the display every time there is a new printW.
        disp_n_clear(MAX, MAX / 2, 0, MAX / 2);

        ///It gets the displacement after printing the word.(If it a long word,
        ///X will be huge)
        x = printW(word, xo, y);

        ///If it is not as displaced to the left as it should
        ///It fix origin one place to the left.
        if(x > LEFT_DISPLACEMENT && next == 0)
        {
            xo--;
        }
        ///If it is totally displaced to the left, it starts displacing the
        ///word to the right until it is totally displaced. 
        else if(xo <= RIGHT_DISPLACEMENT)
        {
            ///This flag blocks the first condition so as to displace the word
            ///totally to the right and then start again.
            next++;
            xo++;
        }
        ///If it was totally displaced to the right, it starts again
        ///(to the left).
        else
        {
            next = 0;
        }
        ///It shows the printed word.
        disp_update();
        ///Delay
        usleep(200000);
        ///Analyzes the user's joystick instruction.
        joy_update();
        jmove = surf();
    }
}

void
printG(int *matrix)
{
    int i, j;
    dcoord_t point;
    ///Pointer goes to the first element that first shows.
    //matrix=matrix+(BOARD_WIDTH*HIDDEN_ROWS);

    ///It goes threw cols&rows of the game board printing the 
    ///objects. It starts from the 4° column because the maximum it can show
    ///is a matrix with 16 columns of height and this one has 20.
    for(i = 0; i < BOARD_HEIGHT; i++)
    {
        point.y = i;

        for(j = 0; j < MAX; j++)
        {
            point.x = j;

            /**
             * It prints columns at both sides(right and left) to mark the
             * border of the  map.
             */
            if((j < 3) || (j > 12))
            {
                disp_write(point, D_ON);
            }

                /**
                 * If it isn't in a border line, it prints the matrix.
                 */
            else if(*matrix == 0)
            {
                disp_write(point, D_OFF);
                matrix++;
            }

            else
            {
                disp_write(point, D_ON);
                matrix++;
            }
        }
    }
}

void
initMenu(void)
{
    int i;
    for(i = 0; i <= 5; i++)
    {
        /**
         * It prints the letter by letter, turning on and off them on 
         * the top and bottom part of the display.
         * While the counter increases, more letters appear. 
         */
        printL(&tt, 0, 0);
        printL(&tt, 0, 8);

        if(i > 0)
        {
            printL(&ee, 3, 2);
            printL(&ee, 3, 10);
        }
        if(i > 1)
        {
            printL(&tt, 5, 0);
            printL(&tt, 5, 8);
        }
        if(i > 2)
        {
            printL(&rr, 8, 2);
            printL(&rr, 8, 10);
        }
        if(i > 3)
        {
            printL(&ii, 11, 0);
            printL(&ii, 11, 8);
        }
        if(i > 4)
        {
            printL(&xx, 13, 2);
            printL(&xx, 13, 10);
        }
        disp_update();
        usleep(200000);
        disp_clear();
        usleep(200000);
    }
    ///At the  end it just show the world TETRIS at the top.
    printL(&tt, 0, 0);
    printL(&ee, 3, 2);
    printL(&tt, 5, 0);
    printL(&rr, 8, 2);
    printL(&ii, 11, 0);
    printL(&xx, 13, 2);
    //disp_update();

}


/// @privatesection
// === Local function definitions ===

void
printL(letters_t * letter, int x, int y)
{
    int i, j, xp = x;
    dcoord_t point;

    /**
     * It goes threw cols&rows of the letter matrix printing each element
     * in the position of the display given by the user until it is
     * on a border of the display.
     */
    for(i = 0; ((i + y) < MAX)&&(i < LROWS); i++)
    {
        point.y = (i + y);

        for(j = 0; ((j + x) < MAX)&&(j < LCOLS); j++)
        {
            point.x = (j + x);

            if(xp < 0)
            {
                xp++;
            }

            else if((*letter)[i][j] == 0)
            {
                disp_write(point, D_OFF);
            }

            else
            {
                disp_write(point, D_ON);
            }
        }
        xp = x;
    }
}

int
sameLetter(letters_t * letter1, letters_t * letter2)
{
    int i, j;
    ///If it doesn't finds out any different element it will be the same.
    int result = SAME;

    ///It goes threw every element of the matrices of letters comparing
    ///every element.If it finds something different, it returns with NOT_SAME.
    for(i = 0; (i < LROWS)&&(result == SAME); i++)
    {
        for(j = 0; j < LCOLS; j++)
        {
            if(((*letter1)[i][j]) != ((*letter2)[i][j]))
            {
                result = NOT_SAME;
            }
        }
    }
    return result;
}

void
lineoff(int line [BOARD_WIDTH], int y)
{
    int j;
    dcoord_t point;

    point.y = y;

    ///It turns off every led of the line with a delay of 0.1 seconds.
    for(j = 0; j < BOARD_WIDTH; j++)
    {
        point.x = (j + 3);
        disp_write(point, D_OFF);
        usleep(100000);
        disp_update();
    }
    return;
}

void
theEnd(void)
{
    int i, j, k = 0,n=0;
    dcoord_t point;

    while(k <= 4)
    {
        for(i = 0; i < MAX; i++)
        {
            point.y = i;

            for(j = 0; j < MAX; j++)
            {
                point.x = j;
                
                if(((i + j) % 2)&&n)
                {
                    disp_write(point, D_OFF);
                }
                else if(n)
                {
                    disp_write(point, D_ON);
                }
                else if((i + j) % 2)
                {
                    disp_write(point, D_ON);
                }
                else 
                {
                    disp_write(point, D_OFF);
                }
            }
        }
        k++;
        
        if(n==0)
        {
            n++;
        }
        else
        {
            n=0;
        }
        
        disp_update();
        usleep(1000000);
    }

}