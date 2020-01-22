#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=RaspberryPi
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/backend/board/board.o \
	${OBJECTDIR}/src/backend/board/pieces/piece_actions.o \
	${OBJECTDIR}/src/backend/board/random_generator.o \
	${OBJECTDIR}/src/backend/rw/rw_ops.o \
	${OBJECTDIR}/src/backend/stats/stats_mgmt.o \
	${OBJECTDIR}/src/frontend/allegro/keys.o \
	${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o \
	${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o \
	${OBJECTDIR}/src/frontend/rpi/display.o \
	${OBJECTDIR}/src/frontend/rpi/joystick.o \
	${OBJECTDIR}/src/frontend/rpi/rpcontrol.o \
	${OBJECTDIR}/src/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f3 \
	${TESTDIR}/TestFiles/f4 \
	${TESTDIR}/TestFiles/f2 \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/backend/test_board.o \
	${TESTDIR}/tests/backend/test_piece_actions.o \
	${TESTDIR}/tests/backend/test_random_gen.o \
	${TESTDIR}/tests/test_boardTimer.o

# C Compiler Flags
CFLAGS=-D RPI

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tetrics

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tetrics: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tetrics ${OBJECTFILES} ${LDLIBSOPTIONS} src/frontend/rpi/libs/disdrv.o src/frontend/rpi/libs/joydrv.o src/frontend/rpi/libs/termlib.o src/frontend/rpi/audiolib/libaudio.so src/frontend/rpi/audiolib/SDL/libSDL1.so src/frontend/rpi/audiolib/SDL/libSDL-1.2.so.0 src/frontend/rpi/audiolib/SDL/libSDL-1.2.so.0.11.4

${OBJECTDIR}/src/backend/board/board.o: src/backend/board/board.c
	${MKDIR} -p ${OBJECTDIR}/src/backend/board
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/board.o src/backend/board/board.c

${OBJECTDIR}/src/backend/board/pieces/piece_actions.o: src/backend/board/pieces/piece_actions.c
	${MKDIR} -p ${OBJECTDIR}/src/backend/board/pieces
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/pieces/piece_actions.o src/backend/board/pieces/piece_actions.c

${OBJECTDIR}/src/backend/board/random_generator.o: src/backend/board/random_generator.c
	${MKDIR} -p ${OBJECTDIR}/src/backend/board
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/random_generator.o src/backend/board/random_generator.c

${OBJECTDIR}/src/backend/rw/rw_ops.o: src/backend/rw/rw_ops.c
	${MKDIR} -p ${OBJECTDIR}/src/backend/rw
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/rw/rw_ops.o src/backend/rw/rw_ops.c

${OBJECTDIR}/src/backend/stats/stats_mgmt.o: src/backend/stats/stats_mgmt.c
	${MKDIR} -p ${OBJECTDIR}/src/backend/stats
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/stats/stats_mgmt.o src/backend/stats/stats_mgmt.c

${OBJECTDIR}/src/frontend/allegro/keys.o: src/frontend/allegro/keys.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/keys.o src/frontend/allegro/keys.c

${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o: src/frontend/allegro/screen/gamePause.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro/screen
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o src/frontend/allegro/screen/gamePause.c

${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o: src/frontend/allegro/screen/gameStats.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro/screen
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o src/frontend/allegro/screen/gameStats.c

${OBJECTDIR}/src/frontend/rpi/display.o: src/frontend/rpi/display.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/display.o src/frontend/rpi/display.c

${OBJECTDIR}/src/frontend/rpi/joystick.o: src/frontend/rpi/joystick.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/joystick.o src/frontend/rpi/joystick.c

${OBJECTDIR}/src/frontend/rpi/rpcontrol.o: src/frontend/rpi/rpcontrol.c
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/rpcontrol.o src/frontend/rpi/rpcontrol.c

${OBJECTDIR}/src/main.o: src/main.c
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f3: ${TESTDIR}/tests/backend/test_board.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f3 $^ ${LDLIBSOPTIONS}   -lcunit 

${TESTDIR}/TestFiles/f4: ${TESTDIR}/tests/test_boardTimer.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f4 $^ ${LDLIBSOPTIONS}   -lcunit 

${TESTDIR}/TestFiles/f2: ${TESTDIR}/tests/backend/test_piece_actions.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f2 $^ ${LDLIBSOPTIONS}   -lcunit 

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/backend/test_random_gen.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   -lcunit 


${TESTDIR}/tests/backend/test_board.o: tests/backend/test_board.c 
	${MKDIR} -p ${TESTDIR}/tests/backend
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/backend/test_board.o tests/backend/test_board.c


${TESTDIR}/tests/test_boardTimer.o: tests/test_boardTimer.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/test_boardTimer.o tests/test_boardTimer.c


${TESTDIR}/tests/backend/test_piece_actions.o: tests/backend/test_piece_actions.c 
	${MKDIR} -p ${TESTDIR}/tests/backend
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/backend/test_piece_actions.o tests/backend/test_piece_actions.c


${TESTDIR}/tests/backend/test_random_gen.o: tests/backend/test_random_gen.c 
	${MKDIR} -p ${TESTDIR}/tests/backend
	${RM} "$@.d"
	$(COMPILE.c) -g -Wall -std=c11 -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/backend/test_random_gen.o tests/backend/test_random_gen.c


${OBJECTDIR}/src/backend/board/board_nomain.o: ${OBJECTDIR}/src/backend/board/board.o src/backend/board/board.c 
	${MKDIR} -p ${OBJECTDIR}/src/backend/board
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/backend/board/board.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/board_nomain.o src/backend/board/board.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/backend/board/board.o ${OBJECTDIR}/src/backend/board/board_nomain.o;\
	fi

${OBJECTDIR}/src/backend/board/pieces/piece_actions_nomain.o: ${OBJECTDIR}/src/backend/board/pieces/piece_actions.o src/backend/board/pieces/piece_actions.c 
	${MKDIR} -p ${OBJECTDIR}/src/backend/board/pieces
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/backend/board/pieces/piece_actions.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/pieces/piece_actions_nomain.o src/backend/board/pieces/piece_actions.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/backend/board/pieces/piece_actions.o ${OBJECTDIR}/src/backend/board/pieces/piece_actions_nomain.o;\
	fi

${OBJECTDIR}/src/backend/board/random_generator_nomain.o: ${OBJECTDIR}/src/backend/board/random_generator.o src/backend/board/random_generator.c 
	${MKDIR} -p ${OBJECTDIR}/src/backend/board
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/backend/board/random_generator.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/board/random_generator_nomain.o src/backend/board/random_generator.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/backend/board/random_generator.o ${OBJECTDIR}/src/backend/board/random_generator_nomain.o;\
	fi

${OBJECTDIR}/src/backend/rw/rw_ops_nomain.o: ${OBJECTDIR}/src/backend/rw/rw_ops.o src/backend/rw/rw_ops.c 
	${MKDIR} -p ${OBJECTDIR}/src/backend/rw
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/backend/rw/rw_ops.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/rw/rw_ops_nomain.o src/backend/rw/rw_ops.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/backend/rw/rw_ops.o ${OBJECTDIR}/src/backend/rw/rw_ops_nomain.o;\
	fi

${OBJECTDIR}/src/backend/stats/stats_mgmt_nomain.o: ${OBJECTDIR}/src/backend/stats/stats_mgmt.o src/backend/stats/stats_mgmt.c 
	${MKDIR} -p ${OBJECTDIR}/src/backend/stats
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/backend/stats/stats_mgmt.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/backend/stats/stats_mgmt_nomain.o src/backend/stats/stats_mgmt.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/backend/stats/stats_mgmt.o ${OBJECTDIR}/src/backend/stats/stats_mgmt_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/allegro/keys_nomain.o: ${OBJECTDIR}/src/frontend/allegro/keys.o src/frontend/allegro/keys.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/allegro/keys.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/keys_nomain.o src/frontend/allegro/keys.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/allegro/keys.o ${OBJECTDIR}/src/frontend/allegro/keys_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/allegro/screen/gamePause_nomain.o: ${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o src/frontend/allegro/screen/gamePause.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro/screen
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/screen/gamePause_nomain.o src/frontend/allegro/screen/gamePause.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/allegro/screen/gamePause.o ${OBJECTDIR}/src/frontend/allegro/screen/gamePause_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/allegro/screen/gameStats_nomain.o: ${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o src/frontend/allegro/screen/gameStats.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/allegro/screen
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/allegro/screen/gameStats_nomain.o src/frontend/allegro/screen/gameStats.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/allegro/screen/gameStats.o ${OBJECTDIR}/src/frontend/allegro/screen/gameStats_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/rpi/display_nomain.o: ${OBJECTDIR}/src/frontend/rpi/display.o src/frontend/rpi/display.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/rpi/display.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/display_nomain.o src/frontend/rpi/display.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/rpi/display.o ${OBJECTDIR}/src/frontend/rpi/display_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/rpi/joystick_nomain.o: ${OBJECTDIR}/src/frontend/rpi/joystick.o src/frontend/rpi/joystick.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/rpi/joystick.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/joystick_nomain.o src/frontend/rpi/joystick.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/rpi/joystick.o ${OBJECTDIR}/src/frontend/rpi/joystick_nomain.o;\
	fi

${OBJECTDIR}/src/frontend/rpi/rpcontrol_nomain.o: ${OBJECTDIR}/src/frontend/rpi/rpcontrol.o src/frontend/rpi/rpcontrol.c 
	${MKDIR} -p ${OBJECTDIR}/src/frontend/rpi
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/frontend/rpi/rpcontrol.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frontend/rpi/rpcontrol_nomain.o src/frontend/rpi/rpcontrol.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/frontend/rpi/rpcontrol.o ${OBJECTDIR}/src/frontend/rpi/rpcontrol_nomain.o;\
	fi

${OBJECTDIR}/src/main_nomain.o: ${OBJECTDIR}/src/main.o src/main.c 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.c) -g -Wall -std=c11 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main_nomain.o src/main.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f3 || true; \
	    ${TESTDIR}/TestFiles/f4 || true; \
	    ${TESTDIR}/TestFiles/f2 || true; \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
