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
 * @brief   Time between different board updates when playing.
 * 
 * @details After initializing the timer, askTimeLimit() can be called to 
 * know how much time should be ellapsed between two board updates. This can
 * be handled with your own code or using askTimer(), which automatically calls
 * askTimeLimit() and, using the system clock, calculates if that time has
 * been ellapsed.
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    06/12/2019, 18:13
 * 
 * @copyright GNU General Public License v3
 */

#ifndef BOARDTIMER_H
#    define BOARDTIMER_H 1

// === Libraries and header files ===
// For STATS struct
#include "../../stats/stats_mgmt.h"

// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === ROM Constant variables ===

// === Global function definitions ===

// Tells if the time limit has been reached
bool
askTimer (void);

// Returns the time in ms that a piece has to be static before updating
int
askTimeLimit (void);

// Initialize the timer functions.
int
initTimer (const stats_t * sts);

// (Re)Set timer
void
startTimer (void);
#endif /* BOARDTIMER_H */
