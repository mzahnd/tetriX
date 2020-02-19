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
 * @file    stats_mgmt.c
 * 
 * @brief   In game stats are managed with the structure found in this file.
 * 
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:35
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // For true and false

// This file
#include "stats_mgmt.h"

/// @privatesection
// === Constants and Macro definitions ===

/// Single line cleared
#define SINGLE              1
/// Score for clearing a single line
#define SINGLE_P          100

/// Two lines cleared
#define DOUBLE              2
/// Score for clearing two lines
#define DOUBLE_P          300

/// Three lines cleared
#define TRIPLE              3
/// Score for clearing a three lines
#define TRIPLE_P          500

/// Four lines cleared
#define TETRIS              4
/// Score for clearing a four lines
#define TETRIS_P          800
// === Enumerations, structures and typedefs ===

/**
 * @brief Private structure for stats_t
 */
typedef struct STATS_PRIVATE
{
    /// Public stats_t
    stats_t * public;

    /// Last piece copy from public
    int lastPiece;

    /// The last clearing was a difficult one (i.e. back2back)
    int lastDifficult;

    /// Soft drop performed
    int soft;

    /// There is a new top score!
    int newTop;

    // Top score file r/w operations
    rwScores_t scoreFile;

    // Top score file loaded properly
    bool tsLoaded;
} stats_private_t;

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Verify if the previous move was a difficult one and add extra points
static int
back2Back (int points);

// Destroy current stats, erasing all the structure's information
static void
destroy (void);

// Update the stats with a new piece
static void
newPiece (int * cp, int * np);

// A soft drop has been performed, so add 1 to the score
static void
softDrop (void);

// Advance to the next level if conditions are met
static void
uLevel (void);

// Update the number of lines cleared
static void
uLines (int rows);

// Update scoring when a/some line(s) is/are cleared
static void
updateStats (int rows);

// Keep actual and top score equal if the actual is bigger
static void
uScore (void);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===
static stats_private_t gameStats;

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize a STATS structure
 * 
 * @param stats STATS structure to initialize
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
int
initStats (struct STATS * stats)
{
    // Counter
    int i;


    if ( stats == NULL )
    {
        fputs("Invalid STATS structures.", stderr);
        return EXIT_FAILURE;
    }

    gameStats.public = stats;

    // Hold as uninitialized until done
    gameStats.public -> _init = false;

    gameStats.public -> level = 0;
    gameStats.public -> lines.cleared = 0;
    gameStats.public -> piece.next = TETROMINO_NONE;
    gameStats.public -> piece.current = TETROMINO_NONE;
    gameStats.public -> _tsLoaded = false;
    gameStats.public -> topScores = NULL;

    for ( i = 0; i < TETROMINOS; i++ )
    {
        gameStats.public -> piece.number[i] = 0;
    }

    // Initialize rw operations and read top score
    if ( init_rwOps(&gameStats.scoreFile) )
    {
        fputs("R/W Ops could not be initialized. Stats will run anyway.",
              stderr);

        gameStats.public -> score.top = 0;
        gameStats.tsLoaded = false;
    }

    else if ( gameStats.scoreFile.get.readTopScore(&gameStats.scoreFile) )
    {
        fputs("Top scores could not be readed. Stats will run anyway.",
              stderr);

        gameStats.public -> score.top = 0;
        gameStats.tsLoaded = false;
    }

    else
    {
        gameStats.public -> topScores = &gameStats.scoreFile;
        gameStats.public -> score.top = gameStats.scoreFile.get._scores[0];
        gameStats.tsLoaded = gameStats.public -> _tsLoaded = true;
    }

    gameStats.public -> score.actual = 0;

    gameStats.public -> softDrop = &softDrop;
    gameStats.public -> update = &updateStats;
    gameStats.public -> newPiece = &newPiece;
    gameStats.public -> destroy = &destroy;

    gameStats.lastPiece = TETROMINO_NONE;

    gameStats.soft = 0;

    gameStats.newTop = false;

    gameStats.lastDifficult = false;

    // Set as initialized
    gameStats.public -> _init = true;

    return EXIT_SUCCESS;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Verify if the previous move was a difficult one and add extra points
 * 
 * @param points How many points should be used in the calculation if the b2b 
 * is performed
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 * 
 * @note This function should be called <b>only</b> when a difficult action is
 * performed and checking a previous one is desired.
 */
static int
back2Back (int points)
{
    int ans = EXIT_FAILURE;

    if ( gameStats.lastDifficult == true )
    {
        gameStats.public -> score.actual += \
                (points * gameStats.public -> level * 3 / 2);

        ans = EXIT_SUCCESS;
    }

    // This function is always called when a difficult action is performed
    gameStats.lastDifficult = true;

    return ans;
}

/**
 * @brief Destroy current stats, erasing all the structure's information
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
destroy (void)
{
    int i;

    // Clear private vars
    gameStats.lastDifficult = false;
    gameStats.lastPiece = TETROMINO_NONE;
    gameStats.newTop = false;
    gameStats.soft = 0;
    gameStats.tsLoaded = gameStats.public -> _tsLoaded = false;

    // Clear public vars
    gameStats.public -> level = 0;
    gameStats.public -> lines.cleared = 0;
    gameStats.public -> piece.current = TETROMINO_NONE;
    gameStats.public -> piece.next = TETROMINO_NONE;
    gameStats.public -> score.actual = 0;
    gameStats.public -> score.top = 0;

    for ( i = 0; i < TETROMINOS; i++ )
    {
        gameStats.public -> piece.number[i] = 0;
    }

    // Clear public pointers
    gameStats.public -> softDrop = NULL;
    gameStats.public -> newPiece = NULL;
    gameStats.public -> update = NULL;
    gameStats.public -> destroy = NULL;
    gameStats.public = NULL;
}

/**
 * @brief Update the stats with a new piece
 * 
 * @param cp Current Piece (using pieces enum in board.h)
 * @param np Next Piece (using pieces enum in board.h)
 * 
 * @return Nothing
 */
static void
newPiece (int * cp, int * np)
{
    gameStats.lastPiece = gameStats.public -> piece.current;
    gameStats.public -> piece.current = *cp;
    gameStats.public -> piece.next = *np;

    // Update how many pieces where dropped
    (gameStats.public -> piece.number[gameStats.public -> piece.current])++;
}

/**
 * @brief A soft drop has been performed, so add 1 to the score
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
softDrop (void)
{
    (gameStats.public -> score.actual)++;

    // Update score
    uScore();
}

/**
 * @brief Advance to the next level if conditions are met
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
uLevel (void)
{
    // Advance to the next level when 10*n rows have been cleared
    // And 10*n equals the current level (i.e. level to advance from level 10
    // to 11, 110 must have been cleared)
    if ( ((gameStats.public -> level + 1) * 10 <= \
        gameStats.public -> lines.cleared) &&
         (((gameStats.public -> lines.cleared) % 10 >= 0) &&
          ((gameStats.public -> lines.cleared) % 10 <= BLOCKS)) )
    {
        (gameStats.public -> level)++;
    }
}

/**
 * @brief Update the number of lines cleared
 * 
 * @param rows Number of lines cleared
 * 
 * @return Nothing
 */
static void
uLines (int rows)
{
    gameStats.public -> lines.cleared += rows;
}

/**
 * @brief Update scoring when a/some line(s) is/are cleared
 * 
 * @param rows Number of rows to be cleared
 * 
 * @return Nothing
 */
static void
updateStats (int rows)
{
    // When no rows are cleared, exit
    if ( rows == 0 )
    {
        return;
    }

    // Calculate score according to the ammount of lines to be cleared
    switch ( rows )
    {
        case SINGLE:
            (gameStats.public -> score.actual) += \
                    (SINGLE_P * gameStats.public -> level);
            gameStats.lastDifficult = false;
            break;

        case DOUBLE:
            (gameStats.public -> score.actual) += \
                    (DOUBLE_P * gameStats.public -> level);
            gameStats.lastDifficult = false;
            break;

        case TRIPLE:
            (gameStats.public -> score.actual) += \
                    (TRIPLE_P * gameStats.public -> level);
            gameStats.lastDifficult = false;
            break;

        case TETRIS:

            // A TETRIS clearing is a difficult task, so back2Back should be 
            // checked
            if ( back2Back(TETRIS_P) )
            {
                (gameStats.public -> score.actual) += \
                        (TETRIS_P * gameStats.public -> level);
            }

            break;

        default:
            gameStats.lastDifficult = false;
            break;
    }

    // Update number of lines
    uLines(rows);

    // Update level
    uLevel();

    // Update score
    uScore();

    // Restore soft drops counter
    gameStats.soft = 0;
}

/**
 * @brief Keep actual and top score equal if the actual is bigger
 * 
 * @param None
 * 
 * @return Nothing
 */
static void
uScore (void)
{
    if ( gameStats.public -> score.actual >= gameStats.public -> score.top )
    {
        gameStats.public -> score.top = gameStats.public -> score.actual;
        gameStats.newTop = true;
    }
}
