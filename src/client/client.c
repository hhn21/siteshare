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


/*
 * add username\npassword to buff and return buffsize
 *  params:
 *      char* username
 *      char* password
 *      char** buff
 *  return: int
 *      buffsize
 */
int makeAuthDataBuff(char* username, char* password, char** buff) {
    int buffSize;
    buffSize = strlen(username) + strlen(password) + 2;
    *buff = malloc(buffSize);
    sprintf(*buff, "%s\n%s", username, password);
    return buffSize;
}

/*
 * add location and receiver to buff and return buffsize
 *  params:
 *      char* receiver
 *      Location* location
 *      char** buff
 *  return: int
 *      buffsize
 */
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
	if(argc != 3) error("\n~ You must run program with address and port number!");
	int port = stoui(argv[2]);
	if(port == -1) error("\n~ Invalid port number");

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
	LocationBook *locationBook = NULL;
    SessionStatus sessionStatus = UNAUTHENTICATED;
	char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    char receiver[ACC_NAME_MAX_LEN];
    char *buff = NULL;
    int buffSize;
    Location *location, *locations;
    int currPage, rs, locationNum;
    Location locationArr[10];

    Request req;
    Response res;

    Option opt;
    opt = welcomeMenu();
	do {
        switch(opt) {
/******************************** Welcome screen ***************************/
            case IOPT_WELCOME: opt = welcomeMenu(); break;

/******************************** Main menu ********************************/
            case IOPT_MAINMENU: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }
                opt = mainMenu(username);
                break;

/******************************** 1. Login ********************************/
            case IOPT_LOGIN: 
            	opt = inputLoginCredentials(username,password);
            	if(opt == IOPT_WELCOME) break;

            	//send user name and password to server
            	buffSize = makeAuthDataBuff(username, password, &buff);
            	req.opcode = LOGIN;
            	req.length = buffSize;
            	req.data = buff;
            	request(socketfd, req, &res);
            	
            	//receive response from server
            	if (res.status == SUCCESS) {
					printf("\n~ Login succeeded\n");
                    sessionStatus = LOGGED_IN;
					opt = IOPT_FETCH;
                    locationBook = newLocationBook();
					if(importLocationOfUser(locationBook, username) < 0) {
                        createUserDBFile(username);
                    }
            	} else {
                    printf("%s\n", (char*)res.data);
            		opt = IOPT_LOGIN;
            	}
            	free(buff);
            	buff = NULL;
                free(res.data);
            	break;

/******************************** 2. Sign up ********************************/
            case IOPT_SIGNUP: 
            	opt = inputSignupCredentials(username, password);
            	if(opt == IOPT_WELCOME) break;

            	//send user name and password to server
            	buffSize = makeAuthDataBuff(username, password, &buff);
            	req.opcode = SIGNUP;
            	req.length = buffSize;
            	req.data = buff;
            	request(socketfd, req, &res);

            	//receive response from server
            	if (res.status == SUCCESS) {
					printf("\n~ Signup succeeded! Now you can login to system\n");
					opt = IOPT_WELCOME;
            	} else {
                    printf("\n~ Signup failed! Username already exists\n");
            		opt = IOPT_SIGNUP;
            	}
                free(buff);
                buff = NULL;
                free(res.data);
            	break;

/******************************** 5. Log out ********************************/
            case IOPT_LOGOUT:
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }

            	//send request LOGOUT to server
                buff = "";
                req.opcode = LOGOUT;
                req.length = buffSize;
                req.data = buff;
                request(socketfd, req, &res);

            	//receive response from server
                if (res.status == SUCCESS) {
                    printf("\n~ Logout succeeded\n");
                    opt = IOPT_WELCOME;
                    destroyLocationBook(locationBook);
                    locationBook = NULL;
                    sessionStatus = UNAUTHENTICATED;
                } else {
                    printf("~ Logout failed\n");
                    opt = IOPT_MAINMENU;
                }
                buff = NULL;
                free(res.data);
            	break;

/******************************** 1. Add location ********************************/
            case IOPT_ADD: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME; 
                    break;
                }

                // create location
                location = malloc(sizeof(Location));
                opt = inputLocationInfo(location);
                if(opt == IOPT_MAINMENU) {
                    free(location);
                    location = NULL;
                    break;
                }

                // add some basic info
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

/******************************** 2. Show local location ********************************/
            case IOPT_SHOW_LOCAL:
                opt = showLocalLocation(locationBook, username, &location);
                break;

/******************************** 2. Show server location ********************************/
            case IOPT_SHOW_SERVER:
                // opt = showLocalLocation(locationBook, username);
                break;

/******************************** 3. Share location ********************************/
            case IOPT_SHARE:
            	//authentication
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
                free(res.data);
            	break;

/******************************** 3. Save to server ********************************/
            case IOPT_SAVE: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf("You are not logged in yet!\n");
                    opt = IOPT_WELCOME; 
                    break;
                }
                opt = confirmSaveLocation();
                if(opt == IOPT_SAVE) {
                    // send delete db request
                    req.opcode = DELETE_LOCATIONS;
                    req.length = 0;
                    req.data = "";
                    request(socketfd, req, &res);
                    if (res.status != SUCCESS) {
                        printf("Some error occured! Save failed\n");
                        opt = IOPT_MAINMENU;
                        break;
                    }
                    printf("Deleted locations on server\n");
                    free(res.data);
                    // send locations to server
                    currPage = 1;
                    do {
                        rs = getLocationsOfUserByPage(locationBook, username, currPage++, locationArr);
                        if(rs == 0) break;
                        req.opcode = SAVE_LOCATION;
                        req.length = rs * sizeof(Location);
                        req.data = locationArr;
                        request(socketfd, req, &res);
                        if (res.status != SUCCESS) {
                            printf("Some error occured! Save failed\n");
                            opt = IOPT_MAINMENU;
                            break;
                        }
                        free(res.data);
                    } while(rs > 0);
                    printf("Saved locations on server\n");
                    opt = IOPT_MAINMENU;
                }
            	break;

/******************************** 4. Restore from server ********************************/
            case IOPT_RESTORE: 
            	//authentication
            	 if(sessionStatus != LOGGED_IN) {
                    printf("You are not logged in yet!\n");
                    opt = IOPT_WELCOME; 
                    break;
                }
                opt = confirmRestoreLocation();
                if(opt == IOPT_RESTORE) {
                    // delete local locations
                    deleteLocationOfUser(locationBook, username);
                    createUserDBFile(username);
                    printf("Deleted local locations\n");
                    // restore locations from server
                    currPage = 1;
                    do {
                        req.opcode = GET_OWNED;
                        req.length = sizeof(int);
                        req.data = &currPage;
                        request(socketfd, req, &res);
                        if (res.status == SUCCESS) {
                            locationNum = res.length / sizeof(Location);
                            locations = res.data;
                            for(int i = 0; i < locationNum; i++) {
                                location = malloc(sizeof(Location));
                                memcpy(location, &locations[i], sizeof(Location));
                                addLocationtoBook(locationBook, location);
                                addNewLocationOfUser(location, username);
                            }
                            free(res.data);
                        } else {
                            opt = IOPT_MAINMENU;
                            free(res.data);
                            break;
                        }
                        currPage++;
                    } while(1);
                    printf("Restored locations successfully\n");
                }
                opt = IOPT_MAINMENU;
            	break;

/******************************** Fetch new location from server ****************/
            case IOPT_FETCH: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf("\n~ You are not logged in yet!\n");
                    opt = IOPT_WELCOME;
                    break;
                }
                printf("\n~ fetching unseen locations ...\n");
                buff = malloc(strlen(username) + 1);
                strcpy(buff, username);
                req.opcode = FETCH;
                req.length = strlen(username) + 1;
                req.data = buff;
                request(socketfd, req, &res);
                if (res.status == SUCCESS) {
                    printf("\n~ fetch succeeded TODO: Show 10 new location per page\n");
                    //TODO: show fetch list in pages
                    //TODO: hoi nguoi dung co muon luu vao may khong
                    opt = IOPT_MAINMENU;
                } else {
                    printf("\n~ No new location\n");
                    opt = IOPT_MAINMENU;
                }
                opt = IOPT_MAINMENU;
                free(buff);
                buff = NULL;
                break;

/******************************** 3. 6. Exit ********************************/
            case IOPT_EXIT:
                break;
        }
        if (opt == IOPT_EXIT) {
            printf("\nFarewell Site sharer\n\n");
        }
    } while(opt != IOPT_EXIT);

    // free location book
    if(locationBook != NULL) 
        destroyLocationBook(locationBook);
    // close socket connection
	close(socketfd);

	return 0;
}