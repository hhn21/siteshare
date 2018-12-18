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

#include "util.h"
#include "account.h"
#include "location.h"
#include "sllist.h"
#include "protocol.h"

int main(int argc, char** argv) {
	/********************************************* Process arguments *********************************************************/
	if(argc != 3) error("You must run program with address and port number!");
	int port = stoui(argv[2]);
	if(port == -1) error("Invalid port number");

	/********************************************* Connect to server *********************************************************/
	int clientSock;
	struct sockaddr_in server_addr; /* server's address information */

	//Step 1: Construct socket
	clientSock = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//Step 3: Request to connect server
	if(connect(clientSock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

	//Step 4: Communicate with server			
	while(1){
		// Client codes
		
	}
	close(clientSock);
	return 0;
}