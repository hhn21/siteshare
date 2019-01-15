CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: main

main: main.o location.o sllist.o
	${CC} main.o location.o sllist.o -o main -pthread

main.o: main.c
	${CC} ${CFLAGS} main.c

location.o: location.c
	${CC} ${CFLAGS} location.c

sllist.o: sllist.c
	${CC} ${CFLAGS} sllist.c

clean:
	rm -f *.o *~
	rm main