#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "util.h"
#include "account.h"
#include "location.h"
#include "sllist.h"
#include "protocol.h"

List *accountList;			//list of account
LocationBook *locationBook;	//TODO: ??
List *clientSessionList;	//TODO: ??

pthread_mutex_t accountListMutex;	
pthread_mutex_t clientSessionMutex;	
pthread_mutex_t locationBookMutex;	
pthread_mutex_t locationDBMutex;	

/* try to authenticate a client credentials
 * params:
 *		credentials: "username\npassword" form string
 * return:
 *	0 if account does not exist
 *	1 if authentication success
 *	-1 if incorrect password
 */
int authenticate(Session *session, char* credentials) {
	char username[ACC_NAME_MAX_LEN];
	char password[ACC_NAME_MAX_LEN];
	Account *a;

	sscanf(credentials, "%s\n%s", username, password);
	a = findAccountByName(accountList, username);
	if(a != NULL) {
		if(strcmp(a->password, password) == 0) {
			session->status = LOGGED_IN;
			session->user = *a;
			return 1;
		}
		return -1;
	}
	return 0;
}

/* try to sign a client credentials up
 * params:
 *		credentials: "username\npassword" form string
 * return:
 *		0 if signup fail
 *		1 if signup success
 */
int signup(char* credentials) {
	char username[ACC_NAME_MAX_LEN];
	char password[ACC_NAME_MAX_LEN];
	Account *a;
	int rs = 0;

	sscanf(credentials, "%s\n%s", username, password);
	pthread_mutex_lock(&accountListMutex);
	a = findAccountByName(accountList, username);
	if(a == NULL) {
		a = malloc(sizeof(Account));
		strcpy(a->username, username);
		strcpy(a->password, password);
		a->isActive = 1;
		insertAtTail(accountList, a);
		saveAccountToFile(accountList, "./data/account.txt");
		rs = 1;
	}
	pthread_mutex_unlock(&accountListMutex);
	return rs;
}

/* try to log a client out
 * params:
 *		session: session of client 
 * return:
 *		0 if logout fail
 *		1 if logout success
 */
int logout(Session *session){
	if(session->status == UNAUTHENTICATED) return 0;
	else if(session->status == LOGGED_IN) {
		session->status = UNAUTHENTICATED;
	}
	return 1;
}

int shareLocationDataProcess(Request req, char *receiver, Location *location){
	int dataLen = req.length;
	memcpy(location, req.data, sizeof(Location));
	memcpy(receiver, req.data + sizeof(Location), dataLen - sizeof(Location));
	return 1;
}

int shareLocation(Session *session, Request req){
	char receiver[ACC_NAME_MAX_LEN];
	Location *location = malloc(sizeof(Location));
	shareLocationDataProcess(req, receiver, location);

	if(strcmp(location->owner, session->user.username) != 0) {// validate sender
		free(location);
		return -1;
	}
	if(findAccountByName(accountList, receiver) == NULL) { // validate receiver
		free(location);
		return -2;
	}
	if(strcmp(receiver,session->user.username)) { // validate receiver
		free(location);
		return -3;
	}
	strcpy(location->sharedBy, location->owner);
	strcpy(location->owner, receiver);
	location->createdAt = time(NULL);
	location->seen = 0;
	pthread_mutex_lock(&locationBookMutex);
	addLocationtoBook(locationBook, location);
	pthread_mutex_unlock(&locationBookMutex);

	pthread_mutex_lock(&locationDBMutex);
	addNewLocationOfUser(location, receiver);
	pthread_mutex_unlock(&locationDBMutex);

	return 1;
}

int deleteLocations(Session *session) {
	pthread_mutex_lock(&locationBookMutex);
	if(deleteLocationOfUser(locationBook, session->user.username) == 0) return 0;
	pthread_mutex_unlock(&locationBookMutex);

	pthread_mutex_lock(&locationDBMutex);
	createUserDBFile(session->user.username);
	pthread_mutex_unlock(&locationDBMutex);

	return 1;
}

int saveLocation(Session *session, Request req) {
	int locationNum = req.length / sizeof(Location);
	Location *locations = (Location*)req.data;
	Location *l;

	pthread_mutex_lock(&locationBookMutex);
	pthread_mutex_lock(&locationDBMutex);
	for(int i = 0; i < locationNum; i++) {
		l = malloc(sizeof(Location));
		memcpy(l, &locations[i], sizeof(Location));
		addLocationtoBook(locationBook, l);
		addNewLocationOfUser(l, session->user.username);
	}
	pthread_mutex_unlock(&locationBookMutex);
	pthread_mutex_unlock(&locationDBMutex);

	return 1;
}

void* handler(void *arg){
	int connSock;
	connSock = *((int *) arg);
	pthread_detach(pthread_self());

	// add client session to list
	Session *session = malloc(sizeof(Session));
	session->fd = connSock;
	session->status = UNAUTHENTICATED;
	pthread_mutex_lock(&clientSessionMutex);
	insertAtTail(clientSessionList, session);
	pthread_mutex_unlock(&clientSessionMutex);

	// communicate with client
	Request req;	//request from user, includes: opcode, length, data
	Response res;	//response message to client, includes: status, length, data
	int rs;			//result, determind what to send back to user

	Location locationArr[PAGE_SIZE];

	while(1) {
		if(fetchRequest(connSock, &req) < 0) break;
		switch(req.opcode) {
			case LOGIN:
				rs = authenticate(session, req.data);
				if (rs == -1){
					res.status = ERROR;
					res.length = strlen(LOGIN_FAIL_PASSWORD_INCORRECT)+1;
					res.data = LOGIN_FAIL_PASSWORD_INCORRECT;
				}
				else if (rs == 0){
					res.status = ERROR; 
					res.length = strlen(LOGIN_FAIL_USERNAME_NOT_EXIST)+1;
					res.data = LOGIN_FAIL_USERNAME_NOT_EXIST;
				}
				else if (rs == 1){
					res.status = SUCCESS;
					res.length = strlen(LOGIN_SUCCESS)+1;
					res.data = LOGIN_SUCCESS;
				}
				break;
			case SIGNUP:
				rs = signup(req.data);
				if(rs) { 
					res.status = SUCCESS;
					res.length = strlen(SIGNUP_SUCCESS)+1;
					res.data = SIGNUP_SUCCESS; 
				} else  { 
					res.status = ERROR;
					res.length = strlen(SIGNUP_FAIL_USERNAME_EXIST)+1;
					res.data = SIGNUP_FAIL_USERNAME_EXIST; 
				}
				break;
			case LOGOUT:
				rs = logout(session);
				if(rs) { 
					res.status = SUCCESS;
					res.length = strlen(LOGOUT_SUCCESS)+1;
					res.data = LOGOUT_SUCCESS; 
				} else  { 
					res.status = ERROR;
					res.length = strlen(LOGOUT_FAIL)+1;
					res.data = LOGOUT_FAIL; 
				}
				break;
			case SHARE_LOCATION:
				rs = shareLocation(session, req);
				if(rs == 1) { 
					res.status = SUCCESS;
					res.length = strlen(SHARE_SUCCESS)+1;
					res.data = SHARE_SUCCESS; 
				} else if (rs == -1){
					res.status = ERROR;
					res.length = strlen(SHARE_FAIL_AUTH_USER)+1;
					res.data = SHARE_FAIL_AUTH_USER; 
				} else if (rs == -2){
					res.status = ERROR;
					res.length = strlen(SHARE_FAIL_USERNAME_NOT_EXIST)+1;
					res.data = SHARE_FAIL_USERNAME_NOT_EXIST; 
				} else if (rs == -3){
					res.status = ERROR;
					res.length = strlen(SHARE_FAIL_CANNOT_SHARE_SELF)+1;
					res.data = SHARE_FAIL_CANNOT_SHARE_SELF; 
				}
				break;
			case FETCH_UNSEEN:
				if(session->status != LOGGED_IN) {
					rs = -1;
				}
				else {
					pthread_mutex_lock(&locationBookMutex);
					rs = getUnseenLocationsOfUserByPage(locationBook, session->user.username, *(int*)req.data, locationArr);
					pthread_mutex_unlock(&locationBookMutex);

					pthread_mutex_lock(&locationDBMutex);
					saveLocationOfUser(locationBook, session->user.username);
					pthread_mutex_unlock(&locationDBMutex);
				}

				if(rs >= 0){
					res.status = SUCCESS;
					res.length = rs * sizeof(Location);
					res.data = locationArr;
				} else {
					res.status = ERROR;
					res.length = 0;
					res.data = ""; 
				}
				break;
			case DELETE_LOCATIONS:
				rs = deleteLocations(session);
				if(rs) { 
					res.status = SUCCESS; 	res.length = 0; 	res.data = ""; 
				} else  { 
					res.status = ERROR; 	res.length = 0; 	res.data = ""; 
				}
				break;
			case SAVE_LOCATION:
				rs = saveLocation(session, req);
				if(rs) { 
					res.status = SUCCESS;
					res.length = strlen(SAVE_SUCCESS) +1;
					res.data = SAVE_SUCCESS; 
				} else  { 
					res.status = ERROR;
					res.length = strlen(SAVE_FAIL) +1;
					res.data = SAVE_FAIL; 
				}
				break;
			case GET_OWNED:
				if(session->status != LOGGED_IN) 
					rs = -1;
				else {
					pthread_mutex_lock(&locationBookMutex);
					rs = getLocationsOfUserByPage(locationBook, session->user.username, *(int*)req.data, locationArr);
					pthread_mutex_unlock(&locationBookMutex);
				}

				if(rs >= 0){
					res.status = SUCCESS;
					res.length = rs * sizeof(Location);
					res.data = locationArr;
				} else {
					res.status = ERROR;
					res.length = strlen(RESTORE_FAIL) +1;
					res.data = RESTORE_FAIL; 
				}
				break;
			default:
				break;
		}
		if(response(connSock, res) < 0) break;
	}

	// delete client session
	ListNode *tmp = clientSessionList->root;
	while(tmp != NULL){
		if(tmp->data == session) {
			pthread_mutex_lock(&clientSessionMutex);
			deleteNode(clientSessionList, tmp); break;
			pthread_mutex_unlock(&clientSessionMutex);
		}
		tmp = tmp->next;
	}
	free(session);
	// close the socket
	close(connSock);
	return NULL;
}

int main(int argc, char** argv) {
	/********************************************* Process arguments *********************************************************/
	if(argc != 2) error("You must run program with port number!");
	int port = stoui(argv[1]);
	if(port == -1) error("Invalid port number");

	/********************************************* Import data *********************************************************/
	ListNode *node;
	Account *a;
	// import accounts from file
	accountList = newList();
	if(importAccountFromFile(accountList, "./data/account.txt") < 0) return -1;

	// import locations from file
	locationBook = newLocationBook();
	listTraverse(node, accountList) {
		a = (Account*)(node->data);
		if(importLocationOfUser(locationBook, a->username) < 0) {
			createUserDBFile(a->username);
		}
	}

	// init clientSessionList list
	clientSessionList = newList();
	
	/********************************************* Start server *********************************************************/
	int listenSock, connSock;
	struct sockaddr_in client; /* client's address information */
	socklen_t addrSize;
	pthread_t tid;

	//Step 1: Construct a TCP socket to listen connection request
	if ((listenSock = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}

	//Step 2: Bind address to socket and listen to new connections
	if (listenOnPort(listenSock, port) == -1 ){  /* calls socket() */
		perror("\nError: ");
		return 0;
	}

	printf("Server is listening\n");

	//Step 3: Handle connections
	while(1){
		if ((connSock = accept(listenSock, (struct sockaddr*)&client, &addrSize)) == -1)
			perror("\nError: ");

		printf("Got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		/* For each client, spawns a thread, and the thread handles the new client */
		pthread_create(&tid, NULL, &handler, &connSock);	
	}

	destroyList(accountList);
	destroyLocationBook(locationBook);

	return 1;
}