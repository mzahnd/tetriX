#!/bin/bash

# Compile and generate a tar file with the game
ALLEGROLIB_PATH=/usr/include/allegro5/

# Allegro definition for Makefile
MAKEALLEGRO=Release_Allegro
# Raspberry Pi definition for Makefile
MAKERPI=Release_RaspberryPi

# Makefile to modify
MAKEFILE=nbproject/Makefile-impl.mk

# Package script
PACKAGESCRIPT=nbproject/Package-

# Ask Architecture to compile
ask_arch()
{
    echo "Are you compiling in a Linux machine with Allegro or in a Raspberry Pi?"
    read -p "Insert 'allegro' or 'rpi': " INPUTARCH
    
    if [ $INPUTARCH == "allegro" ]; then
        sed -i "/DEFAULTCONF=/c\DEFAULTCONF=${MAKEALLEGRO}" ${MAKEFILE}
        call_make ${MAKEALLEGRO}
    elif [ $INPUTARCH == "rpi" ]; then
        sed -i "/DEFAULTCONF=/c\DEFAULTCONF=${MAKERPI}" ${MAKEFILE}
        call_make ${MAKERPI}
    else
        ask_arch
    fi
}

# Call make
call_make()
{
    # Create needed dirs
   mkdir build
   mkdir dist
 
   # Make
   set -e
   make -f Makefile

   # Run package script
   chmod +x ${PACKAGESCRIPT}${1}.bash
   ${PACKAGESCRIPT}${1}.bash

   if [ ${1} == ${MAKERPI} ]
   then
        cp dist/Release_RaspberryPi/GNU-Linux/package/*.tar tetrix.tar
   fi


   # Remove unnecessary dirs
   rm -R build
   rm -R dist
   rm -R null
}

# Try to detect where is the script being run...
try_arch()
{
    echo "Trying to detect host machine..."

    # Raspberry Pi
    if [ $(hostnamectl | awk '/Operating System:/ {print $3}') == "Raspbian" ]
    then
        call_make ${MAKERPI}
    else
        # Detect Allegro
        ls -A ${ALLEGROLIB_PATH} &> /dev/null
        if [ $? -eq 0 ]; then
            call_make ${MAKEALLEGRO}
        else
            # 
            ask_arch
        fi
    fi
}

# MAIN
try_arch
