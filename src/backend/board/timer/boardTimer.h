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
 * @file    boardTimer.h
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
 */

#ifndef BOARDTIMER_H
#    define BOARDTIMER_H

// === Libraries and header files ===
// For STATS struct
#include "../../stats/stats_mgmt.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===
// Initialize the timer functions.
int
initTimer (struct STATS * sts);

// Tells if the time limit has been reached
bool
askTimer (void);

// Reset timer
void
startTimer (void);
#endif /* BOARDTIMER_H */
