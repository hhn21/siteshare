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

#include "account.h"

#define BACKLOG 2

#define TRUE             1
#define FALSE            0
#define BUFF_SIZE 		 7000

//response message from server to client
#define LOGIN_FAIL_USERNAME_NOT_EXIST "~ Login failed! Username does not exist!"
#define LOGIN_SUCCESS "~ Login succeeded"
#define LOGIN_FAIL_PASSWORD_INCORRECT "~ Login failed! Incorrect password!"

#define SIGNUP_SUCCESS "~ Signup succeeded! Now you can login to system"
#define SIGNUP_FAIL_USERNAME_EXIST "~ Signup failed! Username already exists"

#define LOGOUT_SUCCESS "~ Logout succeeded"
#define LOGOUT_FAIL "~ Logout failed"

#define SHARE_SUCCESS "~ Share succeeded! The world owes you a new discovery"
#define SHARE_FAIL "~ Share failed"

#define SAVE_SUCCESS "~ Saved locations on server"
#define SAVE_FAIL "~ Some error occured! Save failed"

#define RESTORE_SUCCESS "~ Restored locations successfully"
#define RESTORE_FAIL "~ Restore failed"
// #define LOGOUT_FAIL "~ Logout failed"
// #define LOGOUT_FAIL "~ Logout failed"


typedef int RequestLengthType;
typedef int ResponseLengthType;

typedef enum {
	SIGNUP,
	LOGIN,
	LOGOUT,
	SHARE_LOCATION,
	SAVE_LOCATION,
	GET_OWNED,
	GET_SHARED,
	FETCH,			//fetch new location when user log in
	DELETE_LOCATIONS
} RequestOpcode;

typedef enum {
	SUCCESS,
	ERROR
} ResponseStatus;

typedef enum {
	UNAUTHENTICATED,
	LOGGED_IN
} SessionStatus;

typedef struct {
	int fd;    				// client socket descriptor
	Account user;			// current user in the session
	SessionStatus status;	// Session status
} Session;

typedef struct {
	RequestOpcode opcode;
	RequestLengthType length;
	void *data;
} Request;

typedef struct {
	ResponseStatus status;
	ResponseLengthType length;
	void *data;
} Response;

/* Make new request with NULL data */
Request newRequest();

/* Make new response with NULL data */
Response newResponse();

/* Bind socket to specific port and listen to connections
 * params:
 *      socket: socket file descriptor
 *      port: port number
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int listenOnPort(int socket, int port);

/* Send a request to server through socket and wait until receive response from server
 * params:
 *      socket: socket file descriptor
 *      request: request to send
 *		response: response pointer to save server response
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

/* Receive a request from client through socket
 * params:
 *      socket: socket file descriptor
 *      request: request pointer to save client request
 * returns:
 *      1 if request succesfully
 *      <= 0 value if has any error
 */
int fetchRequest(int socket, Request *request);

#endif