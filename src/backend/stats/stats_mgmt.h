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
 * @file    stats_mgmt.h
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    27/11/2019, 22:35
 * 
 * @copyright GNU General Public License v3
 */

#ifndef STATS_MGMT_H
#    define STATS_MGMT_H

// === Libraries and header files ===

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

typedef struct
{

    struct SCORE_STRC
    {
        unsigned int top;
        unsigned int actual;
    } score;

    unsigned int level;
    unsigned int lines;
    unsigned char next;

    struct PIECES_STRC
    {
        unsigned int I;
        unsigned int J;
        unsigned int L;
        unsigned int O;
        unsigned int S;
        unsigned int T;
        unsigned int Z;
    } pieces;

} stats_t;

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

#endif /* STATS_MGMT_H */
