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

#include "interface.c"

int makeLoginDataBuff(char* username, char* password, char** buff) {
	int buffSize;
	buffSize = strlen(username) + strlen(password) + 2;
	*buff = malloc(buffSize);
	sprintf(*buff, "%s\n%s", username, password);
	return buffSize;
}

int makeSignupDataBuff(char* username, char* password, char** buff) {
	int buffSize;
	buffSize = strlen(username) + strlen(password) + 2;
	*buff = malloc(buffSize);
	sprintf(*buff, "%s\n%s", username, password);
	return buffSize;
}

int main(int argc, char** argv) {
	/********************************************* Process arguments *********************************************************/
	if(argc != 3) error("You must run program with address and port number!");
	int port = stoui(argv[2]);
	if(port == -1) error("Invalid port number");

	/********************************************* Connect to server *********************************************************/
	int socketfd;
	struct sockaddr_in server_addr; /* server's address information */

	//Step 1: Construct socketfd
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	
	//Step 2: Specify server address
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//Step 3: Request to connect server
	if(connect(socketfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("\nError!Can not connect to sever! Client exit imediately! ");
		return 0;
	}

	//Step 4: Communicate with server
	LocationBook *locationBook = newLocationBook();
	char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    char *buff = NULL;
    int buffSize;
    // Location l;
    FILE *fptr;
    char filePath[100];

    Request req;
    Response res;

    Option opt;
    opt = loginMenu();
	do {
        switch(opt) {
            case IOPT_WELCOME: opt = loginMenu(); break;
            case IOPT_MAINMENU: opt = mainMenu(username); break;
            case IOPT_LOGIN: 
            	opt = inputLoginCredentials(username,password);
            	if(opt == IOPT_WELCOME) break;
            	buffSize = makeLoginDataBuff(username, password, &buff);
            	req.opcode = LOGIN;
            	req.length = buffSize;
            	req.data = buff;
            	request(socketfd, req, &res);
            	if (res.status == SUCCESS) {
					printf("Login succeeded\n");
					opt = IOPT_MAINMENU;
					if(importLocationOfUser(locationBook, username) < 0) {
                        printf("Creating new data store ...\n");
                        sprintf(filePath, "./data/%s.txt", username);
                        fptr = fopen(filePath, "w");
                        fclose(fptr);
                    }
            	} else {
            		printf("Your credentials does not match our record\n");
            		opt = IOPT_WELCOME;
            	}
            	free(buff);
            	buff = NULL;
            	break;
            case IOPT_SIGNUP: 
            	opt = inputSignupCredentials(username, password);
            	if(opt == IOPT_WELCOME) break;
            	buffSize = makeSignupDataBuff(username, password, &buff);
            	req.opcode = SIGNUP;
            	req.length = buffSize;
            	req.data = buff;
            	request(socketfd, req, &res);
            	if (res.status == SUCCESS) {
					printf("Signup succeeded! Now you can login to system\n");
					opt = IOPT_WELCOME;
            	} else {
            		printf("Username is existed\n");
            		opt = IOPT_SIGNUP;
            	}
            	break;
            case IOPT_LOGOUT: 
            	printf("log out\n"); 
            	break;
            case IOPT_EXIT:
            	break;
            case IOPT_ADD: 
            	printf("add\n"); 
            	break;
            case IOPT_SHARE: 
            	printf("share\n"); 
            	break;
            case IOPT_SAVE: 
            	printf("save\n"); 
            	break;
            case IOPT_RESTORE: 
            	printf("restore\n"); 
            	break;
            case IOPT_FETCH: 
            	printf("fetch\n"); 
            	break;
        }
        if (opt == IOPT_EXIT) {
            printf("\nFarewell Site sharer\n\n");
        }
    } while(opt != IOPT_EXIT);

	close(socketfd);
	return 0;
}