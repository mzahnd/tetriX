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
 * @brief   ;
 * 
 * @details ; 
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
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>    // For bool, true and false

#include "display.h"
#include "joystick.h"
#include "libs/disdrv.h"
#include "libs/joydrv.h"
#include "audiolib/libaudio.h"
#include "audiolib/SDL/Include/SDL.h"
#include "../../backend/stats/stats_mgmt.h"
#include "../../backend/board/timer/boardTimer.h"

// This file
#include "rpcontrol.h"
#include "../../backend/board/board.h"

/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

void
play_tetris(board_t * gameboard);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

int
rpi(void)
{
    extern letters_t tt, ee, rr, ii, ss, pp, ll, aa, yy;
    extern letters_t mm, oo, dd, cc, nn, ff, xx, hh;

    ///Create words for the tetris menu.
    words_t play = {&pp, &ll, &aa, &yy, NULL};
    words_t mode = {&mm, &oo, &dd, &ee, NULL};
    words_t score = {&ss, &cc, &oo, &rr, &ee, NULL};
    words_t snd = {&ss, &nn, &dd, NULL};
    words_t on = {&oo, &nn, NULL};
    words_t off = {&oo, &ff, &ff, NULL};
    words_t exit = {&ee, &xx, &ii, &tt, NULL};
    words_t easy = {&ee, &aa, &ss, &yy, NULL};
    words_t hard = {&hh, &aa, &rr, &dd, NULL};

    ///Creates the gameboard
    board_t gameboard;

    int loop = 1;

    /**
     * An array of pointer to words with a counter to show different menu
     * options in the display. The ones that are in the same "page" are
     * separeted from the others with a NULL pointer.
     */
    int k = PLAY;
    words_t * (wordsArray[]) = {NULL, &play, &mode, &score, &snd, &exit, NULL,
                                &on, &off, NULL,
                                &easy, &hard, NULL};

    ///Initializes the display, audio, joystick and gameboard.
    disp_init();
    joy_init();
    init_sound();
    board_init(&gameboard);

    ///Clears the display.
    disp_clear();

    ///Starts the tetris music.
    set_file_to_play("./res/audio/tetris.wav");
    play_sound();

    ///Starts the introduction animation.
    initMenu();
    disp_update();

    ///Until the joyswitch is pressed, it doesn't show the menu.
    /*while(surf() != PRESSED)
    {
        joy_update();
    }
    sleep(1);
     */
    ///It shows the menu starting with PLAY.
    printW(play, 0, 9);
    disp_update();

    while(loop)
    {
        joy_update();

        ///If the user goes right or left, it increase or decrease
        ///the counter that goes trew the array of words.
        if(surf() == RGHT)
        {
            k++;
        }
        else if(surf() == LFT)
        {
            k--;
        }
            ///If the joyswitch is pressed it analyse which one is the 
            ///option chosen.
        else if(surf() == PRESSED)
        {
            switch(k)
            {
                    ///If it is play, it opens the play menu.
                case PLAY:
                    disp_clear();
                    //This is to avoid segmentation fault.
                    while(surf() == PRESSED)
                    {
                        joy_update();
                    }
                    play_tetris(&gameboard);
                    loop = 0;
                    break;

                    ///If it is sound, it goes to On/Off.
                case SND:
                    k = ON;
                    break;
                    ///If it is on, it plays the music.
                case ON:
                    play_sound();
                    k = PLAY;
                    break;
                    ///If it is off, it pauses the music.
                case OFF:
                    pause_sound();
                    k = PLAY;
                    break;
                    ///If it is exit, it finishes the loop.
                case EXIT:
                    loop = 0;
                    break;
                    ///If it is mode, it goes to Easy/Hard.
                case MODE:
                    k = EASY;
                    break;
                    ///If it is easy or hard, it puts the game on that mode.
                case EASY: case HARD:
                    k = PLAY;
                    break;
            }
        }

        /**
         * After the counter change, it analyzes where is it
         * on the array. If it is pointing a NULL, it goes back(or forward)
         * so as to comeback to the last word. 
         */
        if((wordsArray[k] == NULL)&&(surf() == RGHT))
        {
            k--;
        }
        else if((wordsArray[k] == NULL)&&(surf() == LFT))
        {
            k++;
        }
        ///It clears the menu but not the TeTrIs sign.
        disp_n_clear(MAX, MAX / 2, 0, 8);
        ///It prints the word chosen.
        printW(*(wordsArray[k]), 0, 9);
        disp_update();

        /**
         * Unless the joystick goes to the center it doesn't continue.
         * This is done so as to make the user go to right or left every
         * time he wants to change the menu option.
         */
        while(surf() != CENTER)
        {
            joy_update();
        }
    }

    ///It stops the music. User chose EXIT to finish the program. 
    stop_sound();
    ///It inicialyzes and plays a goodbye song.
    set_file_to_play("./res/audio/chau.wav");
    play_sound();

    ///It makes an animation saying goodbye(same as hello twice)
    disp_clear();
    initMenu();
    disp_clear();
    initMenu();
    disp_update();

    ///It doesn't finish until the song stops.
    while(player_status() == PLAYING)
    {
    };
    ///It turns off the display.
    disp_clear();

    return NO_ERROR;
}
/// @privatesection
// === Local function definitions ===

void
play_tetris(board_t * gameboard)
{
    //This is not ready, it has many things to change 
    //

    int k, n, flag;
    int status = CENTER;
    int lines[MAXH];
    stats_t gamestats;

    if(initTimer(&gamestats))
    {
        return;
    }


    while(!(gameboard->ask.endGame()))
    {
        printG(gameboard->ask.board());
        disp_update();

        startTimer();

        while(askTimer() != true)
        {
            joy_update();
            status = surf();

            switch(status)
            {
                case LFT:
                    gameboard->piece.shift(LEFT);
                    printG(gameboard->ask.board());
                    disp_update();
                    break;

                case RGHT:
                    gameboard->piece.shift(RIGHT);
                    printG(gameboard->ask.board());
                    disp_update();
                    break;

                case PRESSED:
                    if(flag == 0)
                    {
                        gameboard->piece.rotate(RIGHT);
                        printG(gameboard->ask.board());
                        disp_update();
                        flag++;
                    }
                    break;

                case DOWN:
                    gameboard->piece.softDrop();
                    printG(gameboard->ask.board());
                    disp_update();
                    break;

                case CENTER:
                    flag = 0;
                    break;

            }

        }


        //It doesnt work
        if((n = (gameboard->ask.filledRows(lines))) != 0)
        {
            for(k = 0; k <= n; k++)
            {
                gameboard->clear.line(lines, lines[k]);
            }
            printG(gameboard->ask.board());
            disp_update();

        }
        gameboard->update();
        printG(gameboard->ask.board());
        disp_update();

    }


    return;
}
