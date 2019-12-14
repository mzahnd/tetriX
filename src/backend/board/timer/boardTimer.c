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
 * @file    boardTimer.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    06/12/2019, 18:13
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // For bool, true and false
#include <time.h>       // For clock_t, clock and CLOCKS_PER_SEC

// This file
#include "boardTimer.h"

/// @privatesection
// === Constants and Macro definitions ===
/**
 * @def NTIMER_T
 * @brief tmr_t variable name
 */
#define NTIMER_T    (timer)

/**
 * @def CLKDIFF(t)
 * @brief Get the difference between the actual clock and @p t
 */
#define CLKDIFF(t)  (clock() - (t))
/**
 * @def CLK2MS(c)
 * @brief Convert the difference between two clocks (@p c) to milliseconds
 */
#define CLK2MS(c)    ( ( ( (double)(c) ) / CLOCKS_PER_SEC) * 1000.0 )

/**
 * @def TLIMIT
 * @brief Get the time limit for the current level
 */
#define TLIMIT      (NTIMER_T.t1)

/**
 * @def RESET_T0
 * @brief Set the initial time variable to the current time.
 */
#define RESET_T0    (NTIMER_T.t0 = clock())

// === Enumerations, structures and typedefs ===

/**
 * @brief Structure with the current game level, initial timer and time limit
 */
typedef struct TIMER
{
    // Stats structure used to get the current level
    stats_t * stats;

    // Initial timer
    clock_t t0;

    // Time limit in the current level
    double t1;

    // Current level
    int level;
} tmr_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===
// Update the time limit according to the current level
static void
updateLimit (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
// tmr_t global variable. Used by all functions
static tmr_t NTIMER_T;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize the timer functions.
 * 
 * @param sts Pointer to the current STATS structure. Used to get the current
 * game level.
 * 
 * @return Success: 0
 * @return Fail: Non-zero
 * 
 * @note This function <b>must</b> be called once before askTimer or startTimer
 */
int
initTimer (struct STATS * sts)
{
    // Check that STATS is valid
    if ( sts == NULL )
    {
        fputs("Bad STATS structure.", stderr);
        return 1;
    }

    NTIMER_T.stats = sts;

    // Get current level
    NTIMER_T.level = NTIMER_T.stats -> level;

    // Initial time limit
    TLIMIT = 1000.0;

    // Start timer in case askTimer is called before another call to startTimer
    RESET_T0;

    return 0;
}

/**
 * @brief Tells if the time limit has been reached
 * 
 * @return true Time limit has been reached or passed
 * @return false Time limit has not been reached
 */
bool
askTimer (void)
{
    bool ans = false;

    // Get time difference between the initial and actual time, in ms
    double dt = CLK2MS(CLKDIFF(NTIMER_T.t0));

    // Time limit
    if ( dt >= TLIMIT )
    {
        // Update time limit if level has changed
        if ( NTIMER_T.level != NTIMER_T.stats -> level )
        {
            updateLimit();
        }

        ans = true;

        // Reset timer
        RESET_T0;
    }

    return ans;
}

/**
 * @brief Reset timer
 * 
 * This function should be called when the delay counter should start.
 * 
 * @param None
 * 
 * @return Nothing
 */
void
startTimer (void)
{
    RESET_T0;
}
/// @privatesection
// === Local function definitions ===

/**
 * @brief Update the time limit according to the current level
 */
static void
updateLimit (void)
{
    // Update level
    NTIMER_T.level = (NTIMER_T.stats -> level);


    if ( NTIMER_T.stats -> level < 6 )
    {
        TLIMIT -= 30.0;
    }

    else if ( NTIMER_T.stats -> level < 11 )
    {
        TLIMIT -= 50.0;
    }

    else if ( NTIMER_T.stats -> level < 16 )
    {
        TLIMIT -= 70;
    }

    else if ( NTIMER_T.stats -> level == 16 )
    {
        TLIMIT -= 30.0;
    }
    else if ( NTIMER_T.stats -> level == 20 )
    {
        TLIMIT -= 50.0;
    }
}
