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
 * @file    rw_ops.h
 * 
 * @brief   Read and Write operations performed by the game.
 * 
 * @details Reads and writes the Top Score table inside gamefiles/topscore.
 * Both, read and write operations verify if the directory and file already
 * exists and creates both whenever necessary.

 * It is printed a message in stderr when this happens, saying that the file
 * could not be found and that it will be created.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:31
 * 
 * @copyright GNU General Public License v3
 */

#ifndef RW_OPS_H
#    define RW_OPS_H 1

// === Libraries and header files ===

// === Constants and Macro definitions ===

/// Number of top scores in the list
#    define NTOPSCORE       10

/// Size of a name string (including '\0')
#    define NAMESIZE        4

// === Enumerations, structures and typedefs ===

/// See RWSCORES documentation
typedef struct RWSCORES rwScores_t;

/**
 * @brief Read and write scores in and from the top score list.
 * 
 * Gives information about the top scores, as well as a NAMESIZE-1 long string
 * with the name of the author and allows adding a new score to the list.
 * 
 * @note Trying to add a new score that is lower than the lowest score already 
 * in the list will do nothing. This is made for always calling the function
 * after a game has ended.
 * 
 * @warning Function init_rwOps() must be called before accessing any field 
 * in this structure.
 * 
 */
struct RWSCORES
{
    /// Read scores and names from the top list
    struct
    {
        /// List of top scores. Being 0 the higher score.
        unsigned int _scores [NTOPSCORE];
        /// List of names. Being 0 the author of the higher score.
        char _names [NTOPSCORE][NAMESIZE];

        /**
         * @brief Read the top score information from a file.
         * 
         * Stores the information in get._scores[] and get._names[].
         * 
         * @param self Structure from which this function is called
         * 
         * @return Success: EXIT_SUCCESS
         * @return Fail: EXIT_FAILURE
         */
        int (* readTopScore)(rwScores_t * self);
    } get;

    /// Add a new score to the top list
    struct
    {
        /// New score to be added
        unsigned int score;
        /// Author's name of the new score
        char name[NAMESIZE];

        /**
         * @brief Writes a new score in the top.
         * 
         * When the given score is lower than the lowest top, no actions are 
         * performed.
         * 
         * @note After calling this function, readTopScore is not need to be
         * called as _scores and _names in get are already updated.
         * 
         * @warning add.score and add.name should be filled with valid data
         * before calling this function.
         * 
         * @param self Structure from which this function is called
         * 
         * @return Success: EXIT_SUCCESS
         * @return Fail: EXIT_FAILURE
         */
        int (*writeIntoScore)(rwScores_t * self);

    } add;
};

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Initialize read and write operations and functions in rwScores_t
int
init_rwOps (rwScores_t * self);

#endif /* RW_OPS_H */
