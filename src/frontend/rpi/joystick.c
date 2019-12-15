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
 * @file    joystick.c
 * 
 * @brief   ;
 * 
 * @details ; 
 *
 * @author  Gino Minnucci                               <gminnucci@itba.edu.ar>
 * @author  Martín E. Zahnd                                <mzahnd@itba.edu.ar>
 * 
 * @date    10/12/2019, 17:31
 * 
 * @copyright GNU General Public License v3
 *****************************************************************************/

// === Libraries and header files ===
#include <stdio.h>
#include <stdlib.h>

// This file
#include "libs/joydrv.h"
#include "joystick.h"


/// @privatesection
// === Constants and Macro definitions ===

// === Enumerations, structures and typedefs ===

// === Global variables ===

// === Function prototypes for private functions with file level scope ===

// === ROM Constant variables with file level scope ===

// === Static variables and constant variables with file level scope ===

// === Global function definitions ===
/// @publicsection


int
surf(void)
{
    int result;
    
    ///Initializes a variable and a structure with the value of the
    ///joystick coordinates and if the switch was pressed or not.
    jcoord_t coord;
    jswitch_t mySwitch;
    //joy_update();
    coord=joy_get_coord();
    mySwitch=joy_get_switch();
    
    /**
     * Depending on the joystick and joyswitch position it returns with
     * a result showing the state.
     */
    
    ///If the switch is pressed.
    if(mySwitch==J_PRESS)
    {
        result=PRESSED;
    }
    ///If the X coordinate  too big(to the right)
    else if(coord.x>=DIRECTION_LIMIT)
    {
        result=RGHT;
    }
    ///If the X coordinate too small(to the left)
    else if(coord.x<=-DIRECTION_LIMIT)
    {
        result=LFT;
    }
    ///If the Y coordinate too big(to the top)
    else if(coord.y>=DIRECTION_LIMIT)
    {
        result=UP;
    }
    ///If the Y coordinate is too small(to the bottom)
    else if(coord.y<=-DIRECTION_LIMIT)
    {
        result=DOWN;
    }
    ///If it is not in any directions.
    else if((coord.x<=CENTER_LIMIT)&&(coord.x>=-CENTER_LIMIT))
    {
        result=CENTER;
    }
    ///It shouldn't happen because joystick will be always in one
    ///direction or another but jut for precaution there is NOTHING state.
    else
    {
        result=NOTHING;
    }
    return result;
}
/// @privatesection
// === Local function definitions ===
