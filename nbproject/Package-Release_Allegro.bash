#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux
CND_CONF=Release_Allegro
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
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMonoHB.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMonoHB.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorHB8.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorHB8.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMono.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMono.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperator-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperator-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorSC.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorSC.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperator.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperator.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorHBSC.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorHBSC.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMono-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMono-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/.uuid" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/.uuid" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/LICENSE.txt" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/LICENSE.txt" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMonoHB8.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMonoHB8.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperator8-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperator8-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMono8-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMono8-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorSC-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorSC-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorMono8.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorMono8.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperatorHB.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperatorHB.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/pixel-operator"
copyFileToTmpDir "res/fonts/pixel-operator/PixelOperator8.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/pixel-operator/PixelOperator8.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/AUTHORS" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/AUTHORS" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/LiberationSerif-Italic.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/LiberationSerif-Italic.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/LiberationSerif-Regular.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/LiberationSerif-Regular.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/License.txt" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/License.txt" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/COPYING" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/COPYING" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/LiberationSerif-BoldItalic.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/LiberationSerif-BoldItalic.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/fonts/liberation_serif"
copyFileToTmpDir "res/fonts/liberation_serif/LiberationSerif-Bold.ttf" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/fonts/liberation_serif/LiberationSerif-Bold.ttf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/images/main"
copyFileToTmpDir "res/images/main/itba_logo.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/images/main/itba_logo.png" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/images/main"
copyFileToTmpDir "res/images/main/background.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/images/main/background.png" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/images/game"
copyFileToTmpDir "res/images/game/endgame.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/images/game/endgame.png" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/images/game"
copyFileToTmpDir "res/images/game/initial.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/images/game/initial.png" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}/tetrix/res/images/game"
copyFileToTmpDir "res/images/game/background.png" "${NBTMPDIR}/${PACKAGE_TOP_DIR}res/images/game/background.png" 0644

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


# Generate tar file
cd "${TOP}"
rm -f tetrix.tar
cd ${NBTMPDIR}
tar -vcf ../../../../tetrix.tar *
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
