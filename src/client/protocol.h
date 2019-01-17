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

#define ADDRESS_PORT_NEED "\n~ You must run program with address and port number!"
#define PORT_INVALID "\n~ Invalid port number" 
#define SERVER_CANNOT_CONNECT "\nError! Can not connect to sever! Client exit imediately!\n" 
#define SCREEN_SPLITTER "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
#define MENU_SPLITTER "----------\n"
#define INPUT_NOTHING_TO_BACK "\n~ You input nothing, which means back\n"
#define USER_NO_LOCATION "\n~ You don't have any location\n"
#define ASK_SURE_DELETE_SERVER "\n~ This operation will delete all locations on server!\n~ Do you want to continue? (y/n): "
#define ASK_SURE_DELETE_LOCAL "\n~ This operation will delete all local locations!\n~ Do you want to continue? (y/n): "
#define ASK_SURE_RESTORE_ONE_LOCATION "\n~ Restore 1 location from server!\n~ Do you want to continue? (y/n): "
#define VIEW_SERVER "Viewing Server locations:\n"
#define VIEW_LOCAL "Viewing local locations:\n"
#define UPDATING_LOCATION "Updating locations:\n"
#define DELETING_LOCATION "Deleting location:\n"
#define VIEW_FETCHED "Viewing new Sites from fellow Site sharer:\n"

#define LOGIN_NOT "\n~ You are not logged in yet!\n" 
#define ADD_LOCATION_SUCCESS "\n~ New location added! Welldone Site sharer <3\n" 
#define NO_RESULT "\nNo results\n" 
#define PREV_PAGE_HOW "Type '\\p' to prev page "
#define NEXT_PAGE_HOW "Type '\\n' to next page"
#define NO_PREV_PAGE "\n~ No previous page\n"
#define NO_NEXT_PAGE "\n~ Reached the end of result\n"
#define SERVER_LOCATION_DELETE_SUCCESS "\n~ Server locations deleted\n"
#define SAVE_LOCATION_SUCCESS "\n~ Locations saved to server. We got your back ;)\n"
#define RESTORE_LOCATION_SUCCESS "\n~ Restored locations successfully. Worry not, Site sharer!\n"
#define ADD_LOCATION_INS "(Add location form. Note: Leave a blank input if you wish to quit)\n"

#define FAREWELL "\nFarewell Site sharer\n\n"

typedef int RequestLengthType;
typedef int ResponseLengthType;

typedef enum {
	SIGNUP,
	LOGIN,
	LOGOUT,
	SHARE_LOCATION,	//share to another user
	SAVE_LOCATION,	//save location to server
	GET_OWNED,		
	GET_SHARED,
	FETCH_UNSEEN,			//fetch new location when user log in
	DELETE_LOCATIONS,
	GET_USERS
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
	Account user;			// identify the user in the session
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