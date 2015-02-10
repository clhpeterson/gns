# This makefile can be used to build all or some of the sample
# programs.  To build all of them, use the command
# 'make all'.  To build one, say addtwo, use the command
# 'make addtwo'.

# Portions of this makefile require the use of GNU make.
# see http://www.gnu.org/software/make for more information.

VERSION=10.0
WSTPLINKDIR = "/Volumes/Mathematica 1/Mathematica.app/Contents/SystemFiles/Links/WSTP/DeveloperKit/"
SYS = MacOSX-x86-64
CADDSDIR = ${WSTPLINKDIR}/${SYS}/CompilerAdditions
EXTRA_CFLAGS=-stdlib=libstdc++

INCDIR = ${CADDSDIR}
LIBDIR = ${CADDSDIR}

WSPREP = ${CADDSDIR}/wsprep
RM = rm

CC3 = gcc
CXX3 = g++

CC = /usr/bin/clang
CXX = /usr/bin/clang++

CC2 = gcc

CFLAGS = -g3 -std=c99 -pedantic -Wall

SOURCES1 =  gns.c coefficient.c

SOURCES2 = queue.c

TARGET = gns

OBJECTS1 = $(SOURCES1:.c=.o)

OBJECTS2 = $(SOURCES2:.c=.o)

OBJECTS = $(OBJECTS1) $(OBJECTS2)

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	${CXX} ${EXTRA_CFLAGS} $^ -I${INCDIR} -L${LIBDIR}  -lWSTPi4 -lstdc++ -framework Foundation -o $@

	
