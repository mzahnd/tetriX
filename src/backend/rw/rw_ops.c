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
 * @file    rw_ops.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:31
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strncpy

// For stat and mkdir
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>

// This file
#include "rw_ops.h"

/// @privatesection
// === Constants and Macro definitions ===

// Top Score directory path
#define TSDIR           "gamefiles/"
// Top Score file path
#define TSFILE          "gamefiles/topscore"

// Text formatting inside the file
#define FILE_TXT_FORMAT "%s %u "

// Dummy name (string)
#define DUMMYNAME       "---"

// Dummy name (character)
#define DUMMYCHAR       '-'

// Maximum dummy top score
#define DUMMYTOPSCORE   (100000 * NTOPSCORE)

// End of string (yes, writting EOS it's too much work)
#define EOS             '\0'

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// Copy two name strings
static void
copyName (char * destination, const char * source);

// Creates a dummy "Top Scores" file
static int
createDummy (FILE **** pFile);

// Read the top score information from a file
static int
readTopScore (rwScores_t * self);

// Wraps verifyDir() and verifyFile() in a single function.
static int
verifyDF (FILE ** pFile);

// Verify if TSDIR directory exists. If not, create it.
static int
verifyDir (void);

// Verify if TSFILE exists. If not, create it with createDummy()
static int
verifyFile (FILE *** pFile);

// Writes a new score in the top
static int
writeNewScore (rwScores_t * self);

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection

/**
 * @brief Initialize read and write operations and functions in rwScores_t
 * 
 * @warning This function must be called before trying to use any function
 * pointer from an RWSCORES structure (rwScores_t typedef).
 * 
 * @param self Structure to initialize
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
int
init_rwOps (rwScores_t * self)
{
    // Counter
    int i;

    if ( self == NULL )
    {
        return EXIT_FAILURE;
    }

    // Variables

    // .get
    for ( i = 0; i < NTOPSCORE; i++ )
    {
        // Scores
        self -> get._scores[i] = 0;
        // Names
        memset(self -> get._names[i], EOS, NAMESIZE);
    }

    // .add
    // Score
    self -> add.score = 0;
    // Name
    memset(self -> add.name, EOS, NAMESIZE);

    // Function pointers
    self -> add.writeIntoScore = &writeNewScore;
    self -> get.readTopScore = &readTopScore;

    return EXIT_SUCCESS;
}

/// @privatesection
// === Local function definitions ===

/**
 * @brief Copy two name strings.
 * 
 * @param destination Destination of copied name
 * @param source Name to copy
 * 
 * @return Nothing
 */
static void
copyName (char * destination, const char * source)
{
    // Copy string
    strncpy(destination, source, NAMESIZE - 1);
    // Null character manually added
    /* 
     * Not needed since strncpy is used and all strings are initialized with
     * an EOS at the end
     */
    //*(destination + NAMESIZE) = EOS;  
}

/**
 * @brief Creates a dummy "Top Scores" file.
 * 
 * Uses DUMMYNAME to assign a name and DUMMYTOPSCORE for its score.
 * 
 * @note DUMMYTOPSCORE is divided by 2 on every next dummy score.
 * 
 * @note When succeded, the file position is rewinded to the beggining.
 * 
 * @param pFile Pointer to object where the new file control information will 
 * be stored
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
createDummy (FILE **** pFile)
{
    // Counter and dummy score
    int i, score = DUMMYTOPSCORE;

    // Open file
    ***pFile = fopen(TSFILE, "w+");
    if ( !***pFile )
    {
        fputs("File could not be created.\n", stderr);
        return EXIT_FAILURE;
    }

    // Write dummy info in it
    for ( i = 0; i < NTOPSCORE; i++ )
    {
        fprintf(***pFile, FILE_TXT_FORMAT, DUMMYNAME, score);
        score /= 2;
    }

    // Go back to the beggining of the file
    rewind(***pFile);

    return EXIT_SUCCESS;
}

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
static int
readTopScore (rwScores_t * self)
{
    // Counter
    int i;

    // File to read
    FILE * pFile;

    // Verify if everything is in order
    if ( self == NULL )
    {
        fputs("Invalid rwScores structure.\n", stderr);
        return EXIT_FAILURE;
    }

    // Try to read the file
    pFile = fopen(TSFILE, "r");

    // When not posible, try creating a new one
    if ( !pFile )
    {
        fputs("Opening error. Creating file.\n", stderr);

        if ( verifyDF(&pFile) )
        {
            return EXIT_FAILURE;
        }
    }

    // Read data from file
    for ( i = 0; i < NTOPSCORE; i++ )
    {
        fscanf(pFile, FILE_TXT_FORMAT,
               self -> get._names[i], &(self -> get._scores[i]));

#ifdef DEBUG
        fprintf("Name %d: %s %d \n",
                i, self -> get._names[i], self -> get._scores[i]);
#endif
    }

    // Close the file
    fclose(pFile);

    return EXIT_SUCCESS;
}

/**
 * @brief Wraps verifyDir() and verifyFile() in a single function.
 * 
 * @param pFile Pointer to object where the new file control information will 
 * be stored.
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
verifyDF (FILE ** pFile)
{
    int ans = EXIT_SUCCESS;

    // Verify directory
    if ( verifyDir() == EXIT_FAILURE )
    {
        ans = EXIT_FAILURE;
    }

        // Verify file
    else
    {
        ans = verifyFile(&pFile);
    }

    return ans;
}

/**
 * @brief Verify if TSDIR directory exists. If not, create it.
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
verifyDir (void)
{
    int ans = EXIT_SUCCESS;

    struct stat st = {0};

    // Set new umask to ~777
    mode_t n_umask = ~(S_IRWXU | S_IRWXG | S_IRWXO);
    // Save original umask for later restoring
    mode_t org_umask = umask(n_umask);

    // Exists?
    if ( stat(TSDIR, &st) == -1 )
    {
        // No, then create it
        if ( errno == ENOENT )
        {
            // Create directory with 666 permisions
            if ( mkdir(TSDIR,
                       S_IRWXU | // User
                       S_IRGRP | S_IWGRP | // Group
                       S_IROTH | S_IWOTH) == -1 ) // Others
            {
                perror("Directory could not be created");
                ans = EXIT_FAILURE;
            }

        }

            // Check if errno was set with != ENOENT after calling stat()
        else if ( errno == EEXIST )
        {
            ans = EXIT_SUCCESS;
        }

        else
        {
            perror("Directory could not be created");
            ans = EXIT_FAILURE;
        }
    }

    // Restore original umask
    umask(org_umask);

    // fprintf(stderr, "umask: %u", umask(org_umask));

    return ans;
}

/**
 * @brief Verify if TSFILE exists. If not, create it with createDummy()
 * 
 * @param pFile Pointer to object where the new file control information will 
 * be stored.
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
verifyFile (FILE *** pFile)
{
    int ans = EXIT_SUCCESS;

    struct stat st = {0};

    // Exists?
    if ( stat(TSFILE, &st) == -1 )
    {
        // No, then create it
        if ( errno == ENOENT )
        {
            errno = 0;
            if ( createDummy(&pFile) )
            {
                ans = EXIT_FAILURE;
            }
        }

        if ( errno != 0 )
        {
            perror("File could not be created. ");
            ans = EXIT_FAILURE;
        }
    }

    return ans;
}

/**
 * @brief Writes a new score in the top.
 * 
 * When the given score is lower than the lowest top, no actions are performed.
 * 
 * @note add.score and add.name should be filled with valid data before calling
 * this function.
 * 
 * @param self Structure from which this function is called
 * 
 * @return Success: EXIT_SUCCESS
 * @return Fail: EXIT_FAILURE
 */
static int
writeNewScore (rwScores_t * self)
{
    // Counter
    int i, j;

    // File to read
    FILE * pFile;

    if ( self == NULL )
    {
        fputs("Invalid rwScores structure.\n", stderr);
        return EXIT_FAILURE;
    }

    // Read data from file for later manipulation
    if ( readTopScore(self) )
    {
        return EXIT_FAILURE;
    }

    // Continue only if the new score is greater than the lowest in the top
    if ( self -> add.score > self -> get._scores[NTOPSCORE - 1] )
    {
        // Open again the file, this time in r+ mode
        pFile = fopen(TSFILE, "r+");
        if ( !pFile )
        {
            fputs("File could not be opened.\n", stderr);
            return EXIT_FAILURE;
        }

        // Get the new score position in the list
        i = 0;
        while ( self -> add.score < self -> get._scores[i] )
        {
            i++;
        }

        // Move old scores one position down in the list (the lowest will be 
        // discarded
        j = NTOPSCORE - 1;
        while ( j > i )
        {
            // Score
            self -> get._scores[j] = self -> get._scores[j - 1];
            // Name
            copyName(self -> get._names[j],
                     self -> get._names[j - 1]);

            j--;
        }

        // Add the new score
        // Score
        self -> get._scores[i] = self -> add.score;
        // Name
        // When an empty string is passed, create a new one
        if ( self -> get._names[i][0] == EOS )
        {
            for ( j = 0; j < NTOPSCORE; j++ )
            {
                self -> get._names[i][j] = DUMMYCHAR;
            }
            // Manually add NULL character at the end
            self -> get._names[i][j] = EOS;
        }

        copyName(self -> get._names[i], self -> add.name);

        // Write the file with the new scores
        rewind(pFile);
        for ( i = 0; i < NTOPSCORE; i++ )
        {
            fprintf(pFile, FILE_TXT_FORMAT,
                    self -> get._names[i], self -> get._scores[i]);
        }

        // Close file
        fclose(pFile);
    }

    return EXIT_SUCCESS;
}
