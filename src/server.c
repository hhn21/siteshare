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

void* handler(void *arg){
	int connSock;

	connSock = *((int *) arg);
	pthread_detach(pthread_self());

	while(1) {
		
	}

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
		if(importLocationOfUser(locationBook, a.username) < 0) return -1;
	}
	
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