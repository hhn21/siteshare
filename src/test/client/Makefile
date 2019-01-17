CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client

client: client.o account.o location.o sllist.o protocol.o util.o
	${CC} client.o account.o location.o sllist.o protocol.o util.o -o client

client.o: client.c
	${CC} ${CFLAGS} client.c

util.o: util.c
	${CC} ${CFLAGS} util.c

account.o: account.c
	${CC} ${CFLAGS} account.c

location.o: location.c
	${CC} ${CFLAGS} location.c

sllist.o: sllist.c
	${CC} ${CFLAGS} sllist.c

protocol.o: protocol.c
	${CC} ${CFLAGS} protocol.c

clean:
	rm -f *.o *~
	rm client
	rm server