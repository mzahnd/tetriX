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
 * @file    rpcontrol.c
 * 
 * @brief   Raspberry module. It works with audio, display and joystick 
 *          control to show on raspberry pi the game(backend).
 * 
 * @details It is the main file of raspberry control.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    10/12/2019, 18:43
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h> 
#include <string.h> // For memcpy

///Frontend defines, enums and functions.
#include "display.h"
#include "joystick.h"
#include "audioControl.h"

///RPI Libraries.
#include "libs/disdrv.h"
#include "libs/joydrv.h"
#include "audiolib/libaudio.h"

///Backend defines, enums and functions.
#include "../../backend/board/board.h"
#include "../../backend/stats/stats_mgmt.h"
#include "../../backend/board/timer/boardTimer.h"

// This file
#include "rpcontrol.h"

/// @privatesection
// === Constants and Macro definitions ===

///@def PAUSE_LIMIT
///@brief Time that indicates the user wants to go to pause menu.
#define PAUSE_LIMIT 20

#define FX_EXIT     "../res/audio/fx/chau.waw"
#define FX_LINE     "../res/audio/fx/line.wav"

// === Enumerations, structures and typedefs ===

enum gamestatus
{
    GM_EXIT = 0,
    GM_MENU,
    GM_PLAYING,
    GM_PAUSE
};

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

/**
 * @brief Creates a playing menu.
 * 
 * It shows the user the gameboard on RPI's display. It let the user to go
 * from the game to the menu to change music, difficulty and check score.
 * 
 * @param A pointer to the gameboard.
 * @param A pointer to the game stats.
 * @param The difficulty of the game.
 * 
 * @return The game status(pause, menu, exit).
 */
int
play_tetris (board_t * gameboard, stats_t* gameStats, int dif);

/**
 * @brief Prints the word "TeTrIx" at the top of the display.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
printTetrix (void);

/**
 * @brief Prints the score.
 * 
 * It prints the score on the display.
 * 
 * @param An array of numbers(of each digit of the score)
 * @param Score.
 * 
 * @return Nothing
 */
void
printScore (words_t scorestring, int scorenumber);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===

/// @publicsection

/**
 * @brief Run game on Raspberry PI.
 * 
 * The game runs on a display and it is controlled by a Rpi.
 * 
 * @param None.
 * 
 * @return NO_ERROR==0
 * @return ERROR=-1
 */
int
rpi (void)
{
    extern letters_t tt, ee, rr, ii, ss, pp, ll, aa, yy;
    extern letters_t mm, oo, dd, cc, nn, ff, xx, hh, uu;

    ///Create words for the tetris menu.
    words_t play = {&pp, &ll, &aa, &yy, NULL};
    words_t mode = {&mm, &oo, &dd, &ee, NULL};
    words_t score = {&ss, &cc, &oo, &rr, &ee, NULL};
    words_t sound = {&ss, &oo, &uu, &nn, &dd, NULL};
    words_t on = {&oo, &nn, NULL};
    words_t off = {&oo, &ff, &ff, NULL};
    words_t exit = {&ee, &xx, &ii, &tt, NULL};
    words_t easy = {&ee, &aa, &ss, &yy, NULL};
    words_t medium = {&mm, &ee, &dd, &ii, &uu, &mm, NULL};
    words_t hard = {&hh, &aa, &rr, &dd, NULL};

    ///Array of pointer to numbers.
    words_t * scorestring = {NULL};

    ///Creates the gameboard
    board_t gameboard;

    ///Creates a pointer to score table
    stats_t * gameStats = NULL;
    stats_t scoretable;

    ///Game status.
    int gameplay = GM_MENU;
    ///Variable that takes joystick state.
    int joyc;
    ///Variable that gives difficulty mode
    int dif = EASY;

    /**
     * An array of pointer to words with a counter to show different menu
     * options in the display. The ones that are in the same "page" are
     * separated from the others with a NULL pointer.
     */
    int k = PLAY;
    words_t * (wordsArray[]) = {NULL, &play, &mode, &score, &sound, &exit, NULL,
        &on, &off, NULL,
        &easy, &medium, &hard, NULL};

    ///Initializes the display, audio, joystick and gameboard.
    disp_init();
    joy_init();
    /* 
        init_sound();
     */
    ///Clears the display.
    disp_clear();
    /*
        ///Turns on the music 
        playMusic();     
     */
    ///Starts the introduction animation.
    initMenu();
    disp_update();

    ///Until the joyswitch is pressed, it doesn't show the menu.
    while ( surf() != PRESSED )
    {
        joy_update();
    }

    ///It shows the menu starting with PLAY.
    printW(play, 0, 9);
    disp_update();

    ///Until the joyswitch is unpressed, it doesn't get into the menu.
    while ( surf() == PRESSED )
    {
        joy_update();
    }

    ///The menu, it includes pause menu.
    while ( gameplay == GM_MENU || gameplay == GM_PAUSE )
    {
        ///It analyzes the joystick position.
        joy_update();
        joyc = surf();

        ///If the user goes right or left, it increase or decrease
        ///the counter that goes threw the array of words.
        if ( joyc == RIGHT )
        {
            k++;
        }

        else if ( joyc == LEFT )
        {
            k--;
        }

            ///If the joyswitch is pressed it analyzes which one is the 
            ///option chosen.
        else if ( joyc == PRESSED )
        {
            switch ( k )
            {
                    ///If it is play, it opens the play menu.
                case PLAY:
                    disp_clear();

                    ///It doesn't start until joyswitch is unpressed
                    while ( surf() == PRESSED )
                    {
                        joy_update();
                    }

                    ///If it is the first time playing, it starts the 
                    ///stats, timer and gameboard.
                    if ( gameplay == GM_MENU )
                    {
                        board_init(&gameboard);
                        gameStats = (stats_t *) gameboard.ask.stats();
                        initTimer(gameStats);
                    }
                    ///It comes with the instruction of pausing the game 
                    ///or ending it.
                    //stopMusic();
                    gameplay = play_tetris(&gameboard, gameStats, dif);
                    disp_clear();
                    break;
                    ///If it is sound, it goes to On/Off.
                case SOUND:
                    k = ON;
                    break;
                    ///If it is on, it plays the music.
                case ON:
                    //playMusic();
                    k = PLAY;
                    break;
                    ///If it is off, it pauses the music.
                case OFF:
                    //stopMusic();
                    k = PLAY;
                    break;
                    ///If it is exit, it finishes the loop.
                case EXIT:
                    gameplay = GM_EXIT;
                    //stopMusic();
                    break;
                    ///If it is mode, it goes to Easy/Hard.
                case MODE:
                    k = EASY;
                    break;
                    ///If it is easy or hard, it puts the game on that mode.
                case EASY:
                    dif = EASY;
                    k = PLAY;
                    break;
                case MEDIUM:
                    dif = MEDIUM;
                    k = PLAY;
                    break;
                case HARD:
                    dif = HARD;
                    k = PLAY;
                    break;
                case SCORE:
                    if ( gameplay == GM_MENU &&
                         initStats(&scoretable) == EXIT_SUCCESS &&
                         scoretable._tsLoaded == true )
                    {
                        printScore(*scorestring, scoretable.score.top);
                    }
                    else if ( gameplay == GM_PAUSE &&
                              gameStats != NULL &&
                              gameStats -> _init == true )
                    {
                        board_t tmp;
                        memcpy(&tmp, &gameboard, sizeof (tmp));

                        printScore(*scorestring, gameStats->score.actual);

                        memcpy(&gameboard, &tmp, sizeof (tmp));
                    }
                    else
                    {
                        printScore(*scorestring, 99999999);
                    }
                    break;
            }
        }

        /**
         * After the counter change, it analyzes where is it
         * on the array. If it is pointing a NULL, it goes back(or forward)
         * so as to comeback to the last word. 
         */
        if ( (wordsArray[k] == NULL)&&(surf() == RIGHT) )
        {
            k--;
        }
        else if ( (wordsArray[k] == NULL)&&(surf() == LEFT) )
        {
            k++;
        }

        ///It clears the menu but not the TeTrIs sign.
        disp_n_clear(MAX, MAX / 2, 0, 8);

        ///It prints the word chosen, if it is too long, it prints words that 
        ///move from right to left.
        if ( k == SOUND )
        {
            printWmove(*(wordsArray[k]), 0, 9);
        }
        else if ( k == MEDIUM )
        {
            printWmove(*(wordsArray[k]), 0, 9);
        }
        else
        {
            printW(*(wordsArray[k]), 0, 9);
        }

        ///It prints tetrix at the top.
        printTetrix();
        disp_update();

        /**
         * Unless the joystick goes to the center it doesn't continue.
         * This is done so as to make the user go to right or left every
         * time he wants to change the menu option.
         */
        while ( surf() != CENTER && k != SOUND && k != MEDIUM )
        {
            joy_update();
        }
    }

    /*
        ///It initializes and plays a goodbye song.
        //playFX(FX_EXIT);    

        ///It doesn't finish until the song stops.
    
        while(player_status() == PLAYING)
        {
               theEnd();     
        }
     */

    ///It makes an animation saying goodbye(same as hello twice)
    //THIS IS HERE BECAUSE WE ARE HAVING TROUBLE WITH THE MUSIC.
    theEnd();
    theEnd();

    ///It turns off the display.
    disp_clear();

    return NO_ERROR;
}
/// @privatesection
// === Local function definitions ===

int
play_tetris (board_t * gameboard, stats_t * gameStats, int dif)
{
    ///It takes joystick positition.
    int jmovement;
    ///It indicates if the user wants to go to the pause menu or not.
    int pauseflag = 0;
    ///It works as a timer.
    int timeflag = 0;
    ///It indicates the difficulty mode that the user wants.
    int mode;
    ///Some variables used as counters, number of lines completed and loops.
    int k, n, gameplay = GM_PLAYING;
    ///An array used for checking completed lines.
    int lines[BOARD_HEIGHT];
    ///Game timing.
    int delay_time = askTimeLimit();

    ///It creates a gameboard.
    grid_t * board = gameboard->ask.board();

    //It shows the gameboard.
    printG(board);
    disp_update();

    ///Depending on the difficulty the user chose it makes differents delays.
    switch ( dif )
    {
        case EASY:
            mode = 20;
            break;
        case MEDIUM:
            mode = 10;
            break;
        case HARD:
            mode = 0;
            break;
    }

    //It is a loop until the game ends.
    while ( !(gameboard->ask.endGame()) && gameplay == GM_PLAYING )
    {
        //It asks for the user instruction(joystick movement)
        joy_update();
        jmovement = surf();

        ///If the user pressed the switch too much time, it goes to pause menu.
        ///ending the playing loop.
        if ( pauseflag > PAUSE_LIMIT )
        {
            gameplay = GM_PAUSE;
        }
            ///If the user didn't move the piece, it does an update and resets the
            ///"time counter".
        else if ( timeflag > mode )
        {
            timeflag = 0;
            gameboard->update();
        }
            ///If the user goes to the right, it makes a right switch.     
        else if ( jmovement == RIGHT )
        {
            gameboard->piece.shift(RIGHT);
        }
            ///If the user goes to the left, it makes a left switch.
        else if ( jmovement == LEFT )
        {
            gameboard->piece.shift(LEFT);
        }
            ///If the user goes up for the first time, it makes a left rotation
            ///and adds one to the pause flag(this make the piece stop rotating too
            ///much)
        else if ( jmovement == UP && pauseflag == 0 )
        {
            gameboard->piece.rotate(LEFT);
            pauseflag++;
        }
            ///If the user pressed the switch bottom for the first time, it makes a
            ///right rotation and adds one to the pause flag to avoid the piece to 
            ///rotate too much.
        else if ( jmovement == PRESSED && pauseflag == 0 )
        {
            gameboard->piece.rotate(RIGHT);
            pauseflag++;
        }
            ///If the user goes down, it makes a soft drop.
        else if ( jmovement == DOWN )
        {
            gameboard->piece.softDrop();
        }
            ///If the user didn't move, it adds one to the time counter and it
            ///resets pause flag(so as the user can rotate the piece again).
        else if ( jmovement == CENTER )
        {
            pauseflag = 0;
            timeflag++;
        }
            ///If the pressed bottom is still pressed, it adds one to the pauseflag.
        else if ( jmovement == PRESSED && pauseflag != 0 )
        {
            pauseflag++;
        }

        ///It analises if there is any filled row.
        n = gameboard->ask.filledRows(lines);
        if ( n != 0 )
        {
            for ( k = 0; k < n; k++ )
            {
                ///If there is one, it eliminates the line. 
                lineoff(lines, lines[k]);
                gameboard->clear.line(lines, k);
                ///It changes delay time in case too many rows were cleared.
                delay_time = askTimeLimit();
                //playFX(FX_LINE);
            }
        }

        ///It prints the gameboard after this changes.
        printG(board);
        disp_update();

        ///It makes a little delay for a better playability.
        usleep(delay_time * 10);
    }

    ///If the user lost.
    if ( gameboard->ask.endGame() )
    {
        ///It creates a scorestring that wil be shown at the top of the display.
        words_t scorestring = {NULL};
        stats_t *gameStats = (stats_t *) gameboard->ask.stats();

        ///It prints the actual score.
        printScore(scorestring, gameStats->score.actual);
        gameboard->destroy();
        ///It goes to the menu as if it has to start again.
        gameplay = GM_MENU;
    }

    return gameplay;
}

void
printTetrix (void)
{
    extern letters_t tt, ee, rr, ii, xx;

    printL(&tt, 0, 0);
    printL(&ee, 3, 2);
    printL(&tt, 5, 0);
    printL(&rr, 8, 2);
    printL(&ii, 11, 0);
    printL(&xx, 13, 2);
}

void
printScore (words_t scorestring, int scorenumber)
{
    ///It make an array with each number on a 5x5 matrix format.
    extern letters_t one, two, three, four, six, seven, eight, nine, oo, ss;
    words_t numbersArray = {&oo, &one, &two, &three, &four, &ss,
        &six, &seven, &eight, &nine, NULL};

    ///It takes score on another variable to avoid loosing it.
    int number = scorenumber;
    ///Counter to go threw the array and to count the number of digit.
    int k;

    ///It divides the number in 10 so as to get the amount of digits.
    for ( k = 0; number != 0; k++ )
    {
        number /= 10;
    }

    ///It puts an end on the array knowing the number of digits.
    scorestring[k] = NULL;

    ///It creates a "score string" with every digit from the score.
    while ( k > 0 )
    {
        ///It starts from right to left.
        k--;
        ///It puts every number on the "string" with the letters_t format.
        scorestring[k] = (numbersArray[number = (scorenumber % 10)]);
        ///It eliminates the last digit to go for the next one.
        scorenumber /= 10;
    }
    ///It prints the "score string"
    printWmove(scorestring, 0, 9);
}
