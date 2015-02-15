# This makefile can be used to build all or some of the sample
# programs.  To build all of them, use the command
# 'make all'.  To build one, say addtwo, use the command
# 'make addtwo'.

# Portions of this makefile require the use of GNU make.
# see http://www.gnu.org/software/make for more information.

VERSION=9.0
MLINKDIR = "/Volumes/Mathematica/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/MacOSX-x86-64"
CADDSDIR = ${MLINKDIR}/CompilerAdditions
EXTRA_CFLAGS=-stdlib=libstdc++

INCDIR = ${CADDSDIR}
LIBDIR = ${CADDSDIR}

WSPREP = ${CADDSDIR}/mprep
RM = rm

CC = /usr/bin/clang
CXX = /usr/bin/clang++


BINARIES = gns

all : $(BINARIES)


gns : gns.o
	${CXX} ${EXTRA_CFLAGS} -I${INCDIR} gns.o -L${LIBDIR} -lMLi3 -lstdc++ -framework Foundation -o $@

gns.o : gns.c
	${CC} -c ${EXTRA_FLAGS} -I${INCDIR} $<
	
