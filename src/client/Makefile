CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: client

client: client.o account.o location.o sllist.o protocol.o util.o interface.o
	${CC} client.o account.o location.o sllist.o protocol.o util.o interface.o -o client

# client: client.o account.o location.o sllist.o protocol.o util.o func_add.o func_auth.o func_mainmenu.o func_restore.o func_save.o func_share.o func_welcomescreen.o
# 	${CC} client.o account.o location.o sllist.o protocol.o util.o func_add.o func_auth.o func_mainmenu.o func_restore.o func_save.o func_share.o func_welcomescreen.o -o client

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

interface.o: interface.c
	${CC} ${CFLAGS} interface.c

# func_add.o: func_add.c
# 	${CC} ${CFLAGS} func_add.c

# func_auth.o: func_auth.c
# 	${CC} ${CFLAGS} func_auth.c

# func_mainmenu.o: func_mainmenu.c
# 	${CC} ${CFLAGS} func_mainmenu.c

# func_restore.o: func_restore.c
# 	${CC} ${CFLAGS} func_restore.c

# func_save.o: func_save.c
# 	${CC} ${CFLAGS} func_save.c

# func_share.o: func_share.c
# 	${CC} ${CFLAGS} func_share.c

# func_welcome.o: func_welcomescreen.c
# 	${CC} ${CFLAGS} func_welcomescreen.c


clean:
	rm -f *.o *~
	rm client