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
 * @file    piece_actions.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    28/11/2019, 18:31
 * 
 * @copyright GNU General Public License v3
 */

#ifndef PIECE_ACTIONS_H
#    define PIECE_ACTIONS_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

enum shiftingTypes
{
    NONE,
    LEFT,
    RIGHT
};

typedef struct PIECE
{
    void (* rotate) (void);

    void (* shift) (void);

    void (* update_board) (void);

    char type;

    int drop;
    int shifting;

    struct
    {
        int status;
        int position;
    } rotation;

    struct
    {

        struct
        {
            int b1[2];
            int b2[2];
            int b3[2];
            int b4[2];
        } current, old;
    } coord;

} piece_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
int
piece_init (struct PIECE * pstruct,
            const char * bag, const int position,
            char *** board, const int board_h, const int board_w);

#endif /* PIECE_ACTIONS_H */
