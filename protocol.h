#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define BACKLOG 2

#define TRUE             1
#define FALSE            0
#define BUFF_SIZE 		 255

typedef enum {
	USERNAME,
	PASSWORD,
	LOGOUT,
	SHARE_LOCATION,
	SAVE_LOCATION,
	GET_OWNED,
	GET_SHARED
} RequestOpcode;

typedef enum {
	SUCCESS,
	ERROR
} ResponseStatus;

//need review, minh gui username va password 1 the luon.
typedef enum {
	WAITING_USN,
	WAITING_PASS,
	LOGGED_IN
} SessionStatus;

typedef struct {
	int fd;    				// client socket descriptor
	SessionStatus status;	// Session status
} Session;

typedef struct {
	RequestOpcode opcode;
	int length;
	void *data;
} Request;

typedef struct {
	ResponseStatus status;
	int length;
	void *data;
} Response;

int listenOnPort(int socket, int port);

/* Send a request to server through socket
 * params:
 *      socket: socket file descriptor
 *      request: request to send
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int request(int socket, Request request, Response *response);

/* Send a response to client through socket
 * params:
 *      socket: socket file descriptor
 *      response: response to send
 * returns:
 *      bytes sent
 *      <= 0 value if has any error
 */
int response(int socket, Response response);

#endif