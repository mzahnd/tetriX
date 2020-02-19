#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux
CND_CONF=Release_RaspberryPi
CND_DISTDIR=dist
CND_BUILDDIR=build
CND_DLIB_EXT=so
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tetrix
OUTPUT_BASENAME=tetrix
PACKAGE_TOP_DIR=tetrix/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p null
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/audio"
copyFileToTmpDir "res/audio/tetris.wav" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/audio/tetris.wav" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/audio/fx"
copyFileToTmpDir "res/audio/fx/line.wav" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/audio/fx/line.wav" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/audio/fx"
copyFileToTmpDir "res/audio/fx/chau.wav" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/audio/fx/chau.wav" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/audio/fx"
copyFileToTmpDir "res/audio/fx/blop.wav" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/audio/fx/blop.wav" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/audio/fx"
copyFileToTmpDir "res/audio/fx/gameover.wav" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/audio/fx/gameover.wav" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/bin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bin/${OUTPUT_BASENAME}" 0755


# Generate tar file
cd "${TOP}"
rm -f tetrix.tar
cd ${NBTMPDIR}
tar -vcf ../../../../tetrix.tar *
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
