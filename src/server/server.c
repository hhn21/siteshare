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

List *accountList;
LocationBook *locationBook;
List *clientSessionList;

pthread_mutex_t accountListMutex;
pthread_mutex_t clientSessionMutex;
pthread_mutex_t locationBookMutex;

/* try to authenticate a client credentials
 * params:
 *		credentials: "username\npassword" form string
 * return:
 *		0 if authentication fail
 *		1 if authentication success
 */
int authenticate(char* credentials) {
	char username[ACC_NAME_MAX_LEN];
	char password[ACC_NAME_MAX_LEN];
	Account *a;

	sscanf(credentials, "%s\n%s", username, password);
	a = findAccountByName(accountList, username);
	if(a != NULL) {
		if(strcmp(a->password, password) == 0) return 1;
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
 *		0 if signup fail
 *		1 if signup success
 */
int logout(Session *session){
	if(session->status == UNAUTHENTICATED) return 0;
	else if(session->status == LOGGED_IN) {
		session->status = UNAUTHENTICATED;
	}
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
	Request req;
	Response res;
	int rs;

	while(1) {
		if(fetchRequest(connSock, &req) < 0) break;
		switch(req.opcode) {
			case LOGIN:
				rs = authenticate(req.data);
				if(rs) { 
					session->status = LOGGED_IN;
					res.status = SUCCESS; 	res.length = 0; 	res.data = ""; 
				} else  { 
					res.status = ERROR; 	res.length = 0; 	res.data = ""; 
				}
				break;
			case SIGNUP:
				rs = signup(req.data);
				if(rs) { 
					res.status = SUCCESS; 	res.length = 0; 	res.data = ""; 
				} else  { 
					res.status = ERROR; 	res.length = 0; 	res.data = ""; 
				}
			case LOGOUT:
				break;
			default:
				break;
		}
		response(connSock, res);
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
	Account a;
	// import accounts from file
	accountList = newList();
	if(importAccountFromFile(accountList, "./data/account.txt") < 0) return -1;

	// import locations from file
	locationBook = newLocationBook();
	listTraverse(node, accountList) {
		a = *(Account*)(node->data);
		// if(importLocationOfUser(locationBook, a.username) < 0) return -1;
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

	//Step 3: Handle connections
	while(1){
		if ((connSock = accept(listenSock, (struct sockaddr*)&client, &addrSize)) == -1)
			perror("\nError: ");

		printf("You got a connection from %s\n", inet_ntoa(client.sin_addr) ); /* prints client's IP */
		
		/* For each client, spawns a thread, and the thread handles the new client */
		pthread_create(&tid, NULL, &handler, &connSock);	
	}

	destroyList(accountList);
	destroyLocationBook(locationBook);

	return 1;
}