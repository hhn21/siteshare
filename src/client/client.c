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

#include "util.h"
#include "account.h"
#include "location.h"
#include "sllist.h"
#include "protocol.h"
#include "interface.h"


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

int makeShareDataBuff(char* receiver, Location *location, char** buff) {
    int buffSize;
    buffSize = strlen(receiver) + sizeof(Location) + 2;
    *buff = malloc(buffSize);
    memcpy(*buff, location, sizeof(Location));
    memcpy(*buff + sizeof(Location), receiver, strlen(receiver) + 1);
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
		printf("\nError!Can not connect to sever! Client exit imediately!\n\n");
		return 0;
	}

	//Step 4: Communicate with server
	LocationBook *locationBook;
    SessionStatus sessionStatus = UNAUTHENTICATED;
	char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    char receiver[ACC_NAME_MAX_LEN];
    char *buff = NULL;
    int buffSize;
    Location *location;
    FILE *fptr;
    char filePath[100];

    Request req;
    Response res;

    Option opt;
    opt = loginMenu();
	do {
        switch(opt) {
            case IOPT_WELCOME: opt = loginMenu(); break;
            case IOPT_MAINMENU: 
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }
                opt = mainMenu(username);
                break;
            case IOPT_LOGIN: 
            	opt = inputLoginCredentials(username,password);
            	if(opt == IOPT_WELCOME) break;
            	buffSize = makeLoginDataBuff(username, password, &buff);
            	req.opcode = LOGIN;
            	req.length = buffSize;
            	req.data = buff;
            	request(socketfd, req, &res);
            	if (res.status == SUCCESS) {
					printf("\n~ Login succeeded\n");
                    sessionStatus = LOGGED_IN;
					opt = IOPT_FETCH;
                    locationBook = newLocationBook();
					if(importLocationOfUser(locationBook, username) < 0) {
                        printf("\n~ Creating new data store ...\n");
                        sprintf(filePath, "./data/%s.txt", username);
                        fptr = fopen(filePath, "w");
                        fclose(fptr);
                    }
            	} else {
                    printf("%s\n", (char*)res.data);
            		opt = IOPT_LOGIN;
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
					printf("\n~ Signup succeeded! Now you can login to system\n");
					opt = IOPT_WELCOME;
            	} else {
                    printf("\n~ Signup failed! Username already exists\n");
            		opt = IOPT_SIGNUP;
            	}
                free(buff);
                buff = NULL;
            	break;
            case IOPT_LOGOUT:
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }
                buff = NULL;
                req.opcode = LOGOUT;
                req.length = buffSize;
                req.data = buff;
                request(socketfd, req, &res);
                if (res.status == SUCCESS) {
                    printf("\n~ Logout succeeded\n");
                    opt = IOPT_WELCOME;
                    destroyLocationBook(locationBook);
                    sessionStatus = UNAUTHENTICATED;
                } else {
                    printf("\n~ Logout failed\n");
                    opt = IOPT_SIGNUP;
                }
            	break;
            case IOPT_EXIT:
            	break;
            case IOPT_ADD: 
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME; 
                    break;
                }
                // create location
                location = malloc(sizeof(Location));
                inputLocationInfo(location);
                strcpy(location->owner, username);
                strcpy(location->sharedBy, "\0");
                location->createdAt = time(NULL);
                location->seen = 1;
                // add location to in-memory book
                addLocationtoBook(locationBook, location);
                // save location to db
                addNewLocationOfUser(location, username);
                opt = IOPT_MAINMENU;
            	break;
            case IOPT_SHARE: 
                if(sessionStatus != LOGGED_IN) {
                    printf("You are not logged in yet!\n");
                    opt = IOPT_WELCOME; 
                    break;
                }
                // select location to share
                opt = selectLocationToShare(locationBook, username, &location);
                if(opt == IOPT_MAINMENU) break;
                // input receiver
                opt = inputSharingReceiver(receiver);
                if(opt == IOPT_MAINMENU) break;
                // send location to server
                buffSize = makeShareDataBuff(receiver, location, &buff); // Buff: Location|receiver
                req.opcode = SHARE_LOCATION;
                req.length = buffSize;
                req.data = buff;
                request(socketfd, req, &res);
                if (res.status == SUCCESS) {
                    printf("Share succeeded!\n");
                } else {
                    printf("Share failed\n");
                }
                opt = IOPT_MAINMENU;
                free(buff);
                buff = NULL;
            	break;
            case IOPT_SAVE: 
            	printf("save\n"); 
                opt = IOPT_MAINMENU;
            	break;
            case IOPT_RESTORE: 
            	printf("restore\n"); 
                opt = IOPT_MAINMENU;
            	break;
            case IOPT_FETCH: 
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }
                buff = username;
                req.opcode = FETCH;
                req.length = strlen(buff) + 2;
                req.data = buff;
                request(socketfd, req, &res);
                if (res.status == SUCCESS) {
                    printf("\n~ fetch succeeded TODO: Show 10 new location per page\n");
                    //TODO: show fetch list in pages
                    opt = IOPT_MAINMENU;
                } else {
                    printf("\n~ No new location\n");
                    opt = IOPT_MAINMENU;
                }
                break;
                //request tren server
                //hien thi cho nguoi dung 10 thong bao 1
                //hoi nguoi dung muon luu vao may khong
        }
        if (opt == IOPT_EXIT) {
            printf("\nFarewell Site sharer\n\n");
        }
    } while(opt != IOPT_EXIT);

	close(socketfd);
	return 0;
}