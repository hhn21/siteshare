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



int main(int argc, char** argv) {
	/********************************************* Process arguments *********************************************************/
	if(argc != 3) error(ADDRESS_PORT_NEED);
	int port = stoui(argv[2]);
	if(port == -1) error(PORT_INVALID);

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
		printf(SERVER_CANNOT_CONNECT);
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
                    printf(LOGIN_NOT);
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
                printf("\n%s\n", (char*)res.data);
            	if (res.status == SUCCESS) {
                    sessionStatus = LOGGED_IN;
					opt = IOPT_FETCH;
                    locationBook = newLocationBook();
					if(importLocationOfUser(locationBook, username) < 0) {
                        createUserDBFile(username);
                    }
            	} else {
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
                printf("\n%s\n", (char*)res.data);
            	if (res.status == SUCCESS) {
					opt = IOPT_WELCOME;
            	} else {
            		opt = IOPT_SIGNUP;
            	}

                //free malloc for further use
                free(buff);
                buff = NULL;
                free(res.data);
            	break;

/******************************** 5. Log out ********************************/
            case IOPT_LOGOUT:
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
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
                printf("\n%s\n", (char*)res.data);
                if (res.status == SUCCESS) {
                    opt = IOPT_WELCOME;
                    destroyLocationBook(locationBook);
                    locationBook = NULL;
                    sessionStatus = UNAUTHENTICATED;
                } else {
                    opt = IOPT_WELCOME;
                }

                //free malloc for further use
                buff = NULL;
                free(res.data);
            	break;

/******************************** 1. Add location ********************************/
            case IOPT_ADD: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
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
                printf(ADD_LOCATION_SUCCESS);
                opt = IOPT_MAINMENU;
            	break;

/******************************** 2. Show local location ********************************/
            case IOPT_SHOW_LOCAL:
                location = NULL;
                opt = showLocalLocation(locationBook, username, &location);
                if(location != NULL) {
                    // TODO: selected 1 location
                }
                break;

/******************************** 3. Show server location ********************************/
            case IOPT_SHOW_SERVER:
                currPage = 1;
                do {
                    req.opcode = GET_OWNED;
                    req.length = sizeof(int);
                    req.data = &currPage;
                    request(socketfd, req, &res);
                    if (res.status == SUCCESS) {
                        locationNum = res.length / sizeof(Location);
                        if(locationNum > 0) {
                            printLocationLabel();
                            locations = res.data;
                            for(int i = 0; i < locationNum; i++) {
                                printLocationInfo(locations[i], i + 1);
                            }
                        } else {
                            printf(NO_RESULT);
                        }
                        printf("Page %d ", currPage);
                        if(currPage > 1) printf(PREV_PAGE_HOW);
                        if(locationNum == PAGE_SIZE) printf(NEXT_PAGE_HOW);
                        rs = pageNavigate(1, locationNum);
                        if(rs == -2) { 
                            if(currPage == 1) {
                                printf(NO_PREV_PAGE);
                                continue;
                            }
                            currPage -= 1; continue; 
                        }
                        if(rs == -1) { 
                            if(locationNum != PAGE_SIZE) {
                                printf(NO_NEXT_PAGE);
                                continue;
                            }
                            currPage += 1; continue; 
                        }
                        // TODO: selected 1 location
                        if(rs >= 0) {
                            opt = IOPT_MAINMENU;
                            break; 
                        }
                    } else {
                        opt = IOPT_MAINMENU;
                        break;
                    }
                    free(res.data); 
                } while(1);
                break;

/******************************** 4. Share location ********************************/
            case IOPT_SHARE:
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
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

                // receive response from server
                printf("\n%s\n", (char*)res.data);
                opt = IOPT_MAINMENU;

                //free malloc for further use
                free(buff);
                buff = NULL;
                free(res.data);
            	break;

/******************************** 3. Save to server ********************************/
            case IOPT_SAVE: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
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
                        printf("\n%s\n", (char*)res.data);
                        opt = IOPT_MAINMENU;
                        break;
                    }
                    free(res.data);
                    printf(SERVER_LOCATION_DELETE_SUCCESS);
                    // send locations to server
                    currPage = 1;
                    do {
                        rs = getLocationsOfUserByPage(locationBook, username, currPage++, locationArr);
                        if(rs == 0) break;
                        req.opcode = SAVE_LOCATION;
                        req.length = rs * sizeof(Location);
                        req.data = locationArr;
                        request(socketfd, req, &res);

                        //receive response from server
                        if (res.status != SUCCESS) {
                            printf("\n%s\n", (char*)res.data);
                            opt = IOPT_MAINMENU;
                            break;
                        }
                        free(res.data);
                    } while(rs > 0);
                    printf(SAVE_LOCATION_SUCCESS);
                    opt = IOPT_MAINMENU;
                }
            	break;

/******************************** 4. Restore from server ********************************/
            case IOPT_RESTORE: 
            	//authentication
            	 if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
                    opt = IOPT_WELCOME; 
                    break;
                }
                
                opt = confirmRestoreLocation();
                if(opt == IOPT_RESTORE) {
                    // delete local locations
                    deleteLocationOfUser(locationBook, username);
                    createUserDBFile(username);
                    printf("\n~ Deleted local locations\n");
                    // restore locations from server
                    currPage = 1;
                    do {
                        req.opcode = GET_OWNED;
                        req.length = sizeof(int);
                        req.data = &currPage;
                        request(socketfd, req, &res);
                        if (res.status == SUCCESS) {
                            locationNum = res.length / sizeof(Location);
                            if(locationNum == 0) break;
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
                    printf(RESTORE_LOCATION_SUCCESS);
                }
                opt = IOPT_MAINMENU;
            	break;

/******************************** Fetch new location from server ****************/
            case IOPT_FETCH: 
            	//authentication
                if(sessionStatus != LOGGED_IN) {
                    printf(LOGIN_NOT);
                    opt = IOPT_WELCOME;
                    break;
                }
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
            printf(FAREWELL);
        }
    } while(opt != IOPT_EXIT);

    // free location book
    if(locationBook != NULL) 
        destroyLocationBook(locationBook);
    // close socket connection
	close(socketfd);

	return 0;
}