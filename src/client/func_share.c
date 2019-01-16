#include "func.h"

/* Input receiver
 * Params:
 *      receiver 
 * Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option inputSharingReceiver(char *receiver){
    printf("Receiver (username): ");
    fgets(receiver, ACC_NAME_MAX_LEN, stdin);
    receiver[strlen(receiver) - 1] = '\0';
    if (receiver[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    return IOPT_SHARE;
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

/* Print out locations of an user an let user select 1
 * Params:
 *      book LocationBook
 *      username current user to select location
 *      location location pointer to save selected location
 * Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option selectLocationToShare(LocationBook *book, char *username, Location **location) {
    List *locations;
    ListNode *node2;
    ListNode tmp;
    Location *l;
    Location *l_a[10];
    int j = 0;
    char timeString[26];
    struct tm* tm_info;
    char buf[OPT_MAX_LEN];
    int opt = 0;
    int currPage = 1;
    int printPageInfo;

    locations = getLocationsByOwner(book, username);
    if(locations == NULL) {
        printf("You don't have any location to share\n");
        return IOPT_MAINMENU;
    }
    
    printf("\n%-5s %-20s %-30s %-30s %-50s\n", "#", "Created at", "Category", "Name", "Note");
    listTraverse(node2, locations){
        l = (Location*)node2->data;
        l_a[j] = l;
        j = j % 10 + 1;
        tm_info = localtime(&(l->createdAt));
        strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%-5d %-20s %-30s %-30s %-50s \n", j, timeString, l->category, l->name, l->note);
        printPageInfo = 1;
        if(j % 10 == 0 || node2->next == NULL) { // if reach the end of page or the end of list 
            do {
                if(printPageInfo == 1) {
                    printf("\nPage %d \t", currPage);
                    if(currPage > 1) printf("Type '\\p' to prev page, ");
                    if(node2->next != NULL) printf("Type '\\n' to next page");
                }
                printf("\n\nChoose location (1-%d), enter to quit: ", j);
                fgets(buf, OPT_MAX_LEN, stdin);
                buf[strlen(buf) - 1] = '\0';

                // if inputed "\p"
                if(strcmp(buf, "\\p") == 0) {
                    if(currPage == 1) {
                        printf("Error. No previous page!\n");
                        continue;
                    }
                    // go back to previous page
                    currPage -= 1;
                    tmp.next = locations->root;
                    node2 = &tmp;
                    for(int i = 0; i < (currPage - 1) * 10; i++) node2 = node2->next;
                    j = 0;
                    break;
                }

                // if inputed "\n"
                if(strcmp(buf, "\\n") == 0) {
                    if(node2->next == NULL) {
                        printf("Error. Reached the end of result!\n");
                        continue;
                    }
                    currPage += 1;
                    printf("\n\n%-5s %-20s %-30s %-30s %-50s\n", "#", "Created at", "Category", "Name", "Note");
                    break;
                }

                // if inputed "\0" (blank)
                if(buf[0] == '\0') {
                    printf("You input nothing, which means back\n");
                    *location = NULL;
                    return IOPT_MAINMENU;
                }

                // else
                opt = atoi(buf);
                if(opt < 1 || opt > j) {
                    printf("Invalid input, please try again\n");
                    printPageInfo = 0;
                    continue;
                }
                *location = l_a[opt - 1];
                return IOPT_SHARE;
            } while (1);
        }
    }
    return IOPT_SHARE;
}