INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc
CPP=g++

CDEFS=
CFLAGS= -g -Wall -O3 -fopenmp $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= 
CPPFILES= scale.cpp

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	scale 

clean:
	-rm -f *.o *.d
	-rm -f scale


scale: scale.cpp
	$(CPP) $(CFLAGS) -o $@ scale.cpp -lm

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
