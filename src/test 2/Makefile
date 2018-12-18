CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm

all: interface

interface: interface.o account.o location.o sllist.o
	${CC} interface.o account.o location.o sllist.o -o interface

interface.o: interface.c
	${CC} ${CFLAGS} interface.c

account.o: account.c
	${CC} ${CFLAGS} account.c

location.o: location.c
	${CC} ${CFLAGS} location.c

sllist.o: sllist.c
	${CC} ${CFLAGS} sllist.c

clean:
	rm -f *.o *~
	rm interface