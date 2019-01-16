#include "interface.h"
#include "protocol.h"


/****************************** Welcome screen ******************************/
/* used in welcomeMenu()
 * Print out the welcome screen
 */
void printWelcomeScreen(){
    int r = rand()%8 + 1;
    printf(SCREEN_SPLITTER);
    switch(r){
        case 1: printf("Welcome to Site Share"); break;
        case 2: printf("Good day, explorer!"); break;
        case 3: printf("Are you an explorer?"); break;
        case 4: printf("Wants to share your location with friends?"); break;
        case 5: printf("Explore, share, with Site Share"); break;
        case 6: printf("Found a place? Share away!"); break;
        case 7: printf("Traveler's notebook"); break;
        case 8: printf("Keep your location in hand, with Site Share"); break;
    }
    printf(" (Welcome Screen)\n");
    printf("1. Log in\n");
    printf("2. Sign up\n");
    printf("3. Exit\n");
    printf("----------\n");
    printf("Please choose 1-3\n");
    printf("Your choice: ");
}

/* case: IOPT_WELCOME
 * a welcome screen, user choose login, sign up or exit
 *  Return: Option
 *      IOPT_LOGIN
 *      IOPT_SIGNUP
 *      IOPT_EXIT
 */
Option welcomeMenu(){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printWelcomeScreen();
        fgets(buf, OPT_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';

        opt = atoi(buf);
        if (buf[0] == '\0') {
            printf("\n~ You input nothing, if u wish to exit, please choose 3\n");
        }
        else if (opt < 1 || opt > 3) {
            printf("\n~ Invalid input, please choose a number, from 1 to 3\n");
        }
    } while(opt < 1 || opt > 3);
    switch(opt) {
        case 1: return IOPT_LOGIN;
        case 2: return IOPT_SIGNUP;
        case 3: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}



/****************************** authentication ******************************/
/****************************** Login *****/
/* case: IOPT_LOGIN
 * take username and password from user, check Auth()
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if Invalid username or password or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(Login screen)\n");
    while(1) {
        printf("User name: ");
        fgets(username, ACC_NAME_MAX_LEN-1, stdin);
        username[strlen(username) - 1] = '\0';
        if (username[0] == '\0') {
            printf("You input nothing, which means back\n");
            return IOPT_WELCOME;
        }
        if (validateUsername(username)) break;
        else {
            printf("Username must has only alphabet characters and digits\n");
        }
    }

    while(1) {
        printf("Password : ");
        fgets(password, ACC_NAME_MAX_LEN-1, stdin);
        password[strlen(password) - 1] = '\0';
        if (password[0] == '\0') {
            printf("You input nothing, which means back\n");
            return IOPT_WELCOME;
        }
        if (validatePassword(password)) break;
        else {
            printf("Password cannot have space character\n");
        }
    }
    return IOPT_LOGIN;
}

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

/****************************** sign up *****/
/* case: IOPT_SIGNUP
 * take username and password from user
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if Invalid username already exist or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(Sign up screen)\n");
    while(1) {
        printf("User name: ");
        fgets(username, ACC_NAME_MAX_LEN-1, stdin);
        username[strlen(username) - 1] = '\0';
        if (username[0] == '\0') {
            printf("You input nothing, which means back\n");
            return IOPT_WELCOME;
        }
        if (validateUsername(username)) break;
        else {
            printf("Username must has only alphabet characters and digits\n");
        }
    }

    while(1) {
        printf("Password : ");
        fgets(password, ACC_NAME_MAX_LEN-1, stdin);
        password[strlen(password) - 1] = '\0';
        if (password[0] == '\0') {
            printf("You input nothing, which means back\n");
            return IOPT_WELCOME;
        }
        if (validatePassword(password)) break;
        else {
            printf("Password cannot have space character\n");
        }
    }
    return IOPT_SIGNUP;
}





/****************************** Main menu ******************************/
/* used in mainMenu(), Print out the main menu
 *  Params:
 *      username
 */
void printMainMenu(char username[]){
    printf(SCREEN_SPLITTER);
    printf("Main menu (logged in as %s)\n", username);
    printf("1. Add new location\n");
    printf("2. Show local location\n");
    printf("3. Show server location\n");
    printf("4. Share location\n");
    printf("5. Save to server\n");
    printf("6. Restore from server\n");
    printf("7. Refresh\n");
    printf("8. Log out\n");
    printf("9. Exit\n");
    printf("----------\n");
    printf("Please choose from 1 to 9\n");
    printf("Your choice: ");
}

/* Print our the main menu
 *  Params:
 *      username
 *  Return: Option
 *      IOPT_ADD_LOCAL;
 *      IOPT_SHOW_LOCAL;
 *      IOPT_SHOW_SERVER;
 *      IOPT_SHARE;
 *      IOPT_SAVE;
 *      IOPT_RESTORE;
 *      IOPT_LOGOUT;
 *      IOPT_EXIT
 */
Option mainMenu(char username[]){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printMainMenu(username);
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        opt = atoi(buf);
        if (opt < 1 || opt > 9)
        {
            printf("\n~ Invalid input. Please input a number, from 1 to 8\n");
        }
    } while (opt < 1 || opt > 9);

    switch(opt) {
        case 1: return IOPT_ADD_LOCAL;
        case 2: return IOPT_SHOW_LOCAL;
        case 3: return IOPT_SHOW_SERVER;
        case 4: return IOPT_SHARE;
        case 5: return IOPT_SAVE;
        case 6: return IOPT_RESTORE;
        case 7: return IOPT_FETCH;
        case 8: return IOPT_LOGOUT;
        case 9: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}
    

/****************************** add location ******************************/
/*
 * used in inputLocationInfo
 */
void printCategoryList() {
    const int CATEGORY_COUNT = 7;
    const char *categories[] = {
        "Building",
        "Restaurant",
        "Shop",
        "Street",
        "Lake",
        "River",
        "Park"
    };
    printf("(Ex: ");
    for(int i = 0; i < CATEGORY_COUNT; i++) {
        printf("%s, ", categories[i]);
    }
    printf("...)\n");
}

/* case: IOPT_ADD_LOCAL
 * input location info
 *  Params:
 *      Location* location
 *
 *  Return: Option
 *      IOPT_ADD_LOCAL (if succeed, continue)
 *      IOPT_MAINMENU (if user input nothing)
 */
Option inputLocationInfo(Location *location){
    printf(SCREEN_SPLITTER);
    printf(ADD_LOCATION_INS);
    printf("Input Category ");
    printCategoryList();
    printf("Location Category: ");
    fgets(location->category, L_CAT_MAX_LEN, stdin);
    location->category[strlen(location->category) - 1] = '\0';
    if (location->category[0] == '\0') {
        printf(INPUT_NOTHING_TO_BACK);
        return IOPT_MAINMENU;
    }
    printf("Location Name: ");
    fgets(location->name, L_NAME_MAX_LEN, stdin);
    location->name[strlen(location->name) - 1] = '\0';
    if (location->name[0] == '\0') {
        printf(INPUT_NOTHING_TO_BACK);
        return IOPT_MAINMENU;
    }
    printf("Location Note: ");
    fgets(location->note, L_NOTE_NAME_MAX_LEN, stdin);
    location->note[strlen(location->note) - 1] = '\0';
    if (location->note[0] == '\0') {
        printf(INPUT_NOTHING_TO_BACK);
        return IOPT_MAINMENU;
    }

    return IOPT_ADD_LOCAL;
}



/****************************** share to another user ******************************/
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
        printf(INPUT_NOTHING_TO_BACK);
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
 *  Params:
 *      book LocationBook
 *      username current user to select location
 *      location location pointer to save selected location
 *  Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option selectLocationToShare(LocationBook *book, char *username, Location **location){
    return showLocalLocation(book, username, location);
}

/* Print location table labels*/
void printLocationLabel() {
    printf("\n%-5s %-30s %-30s %-30s %-30s %-30s\n", "#", "Created at", "Category", "Name", "Note", "Shared by");
}

/* Print location table info
 *  Params:
 *      l Location to print
 *      index index of location
 */
void printLocationInfo(Location l, int index){
    char timeString[26];
    struct tm* tm_info;

    tm_info = localtime(&(l.createdAt));
    strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", tm_info);

    printf("%-5d %-30s %-30s %-30s %-30s %-30s\n", index, timeString, l.category, l.name, l.note, l.sharedBy);
}


/* input page navigation commands
 * Return:
 *      -2 if inputed \p
 *      -1 if inputed \n
 *      0 if inputed nothing
 */
int pageNavigateNoNumber() {
    char buf[OPT_MAX_LEN];

    while(1) {
        printf("\n\nInput command (Enter to quit): ");
        fgets(buf, OPT_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';

        if(strcmp(buf, "\\p") == 0) return -2;
        if(strcmp(buf, "\\n") == 0) return -1;
        if(buf[0] == '\0') return 0;
    }
    return 1;
}

/* input page navigation commands
 * Params:
 *      min min value of number input
 *      max max value of number input
 * Return:
 *      -2 if inputed \p
 *      -1 if inputed \n
 *      0 if inputed nothing
 *      > 0 value if inputed a number within [min,max] 
 */
int pageNavigate(int min, int max) {
    int opt = 0;
    char buf[OPT_MAX_LEN];

    while(1) {
        printf("\nChoose item (%d-%d), enter to quit: ", min, max);
        fgets(buf, OPT_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';

        if(strcmp(buf, "\\p") == 0) return -2;
        if(strcmp(buf, "\\n") == 0) return -1;
        if(buf[0] == '\0') return 0;
        opt = atoi(buf);
        if(opt < min || opt > max) {
            printf("\n~ Invalid input, please try again\n");
            continue;
        }
        break;
    }
    return opt;
}

/* input page navigation commands
 * Params:
 *      book LocationBook
 *      username
 *      location selected location - set to NULL if not selected
 * Return:
 *      -2 if inputed \p
 *      -1 if inputed \n
 *      0 if inputed nothing
 *      > 0 value if inputed a number within [min,max] 
 */
Option showLocalLocation(LocationBook *book, char *username, Location **location){
    List *locations;
    ListNode *node2;
    ListNode tmp;
    Location *l;
    Location *l_a[10];
    int j = 0;
    int pageCmd = 0;
    int currPage = 1;
    int printPageInfo;

    locations = getLocationsByOwner(book, username);
    if(locations == NULL) {
        printf(USER_NO_LOCATION);
        return IOPT_MAINMENU;
    }
    printf(SCREEN_SPLITTER);
    printf(VIEW_LOCAL);
    printLocationLabel();
    listTraverse(node2, locations){
        j = j % 10 + 1;
        l = (Location*)node2->data;
        l_a[j - 1] = l;
        printLocationInfo(*l, j);
        printPageInfo = 1;
        if(j % 10 == 0 || node2->next == NULL) { // if reach the end of page or the end of list 
            do {
                if(printPageInfo == 1) {
                    printf("\nPage %d \t", currPage);
                    if(currPage > 1) printf(PREV_PAGE_HOW);
                    if(node2->next != NULL) printf(NEXT_PAGE_HOW);
                }
                pageCmd = pageNavigate(1, j);

                // if inputed "\p"
                if(pageCmd == -2) {
                    if(currPage == 1) {
                        printf(NO_PREV_PAGE);
                        printPageInfo = 0;
                        continue;
                    }
                    // go back to previous page
                    currPage -= 1;
                    tmp.next = locations->root;
                    node2 = &tmp;
                    for(int i = 0; i < (currPage - 1) * 10; i++) node2 = node2->next;
                    j = 0;
                    printf(SCREEN_SPLITTER);
                    printf(VIEW_LOCAL);
                    printLocationLabel();
                    break;
                }

                // if inputed "\n"
                if(pageCmd == -1) {
                    if(node2->next == NULL) {
                        printf(NO_NEXT_PAGE);
                        printPageInfo = 0;
                        continue;
                    }
                    currPage += 1;
                    printf(SCREEN_SPLITTER);
                    printf(VIEW_LOCAL);
                    printLocationLabel();
                    break;
                }

                // if inputed "\0" (blank)
                if(pageCmd == 0) {
                    printf(INPUT_NOTHING_TO_BACK);
                    *location = NULL;
                    return IOPT_MAINMENU;
                }

                // else
                *location = l_a[pageCmd - 1];
                return IOPT_SHARE;
            } while (1);
        }
    }
    return IOPT_SHARE;
}



/****************************** save to server ******************************/
/* Confirm to delete all locations on server and upload all location on local to server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_SAVE if confirm
 */
Option confirmSaveLocation(){
    char buf[OPT_MAX_LEN];

    printf(ASK_SURE_DELETE_SERVER);
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_SAVE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("\n~ Invalid input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}



/****************************** Restore from server ******************************/
/* Confirm to delete all locations on local and restore all location from server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_RESTORE if confirm
 */
Option confirmRestoreLocation(){
    char buf[OPT_MAX_LEN];

    printf(ASK_SURE_DELETE_LOCAL);
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_RESTORE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("\n~ Invalid input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}

void printUpdateMenu(){
    printf("\n(Location Screen)\n");
    printf("1. Update\n");
    printf("2. Delete\n");
    printf("3. Exit\n");
    printf("----------\n");
    printf("Please choose 1, 2 or 3\n");
    printf("Your choice: ");
}

Option locationUpdateMenu() {
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printUpdateMenu();
        fgets(buf, OPT_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';

        opt = atoi(buf);
        if (buf[0] == '\0') {
            printf("\n~ You input nothing, if u wish to exit, please choose 3\n");
        }
        else if (opt < 1 || opt > 3) {
            printf("\n~ Invalid input, please choose a number, from 1 to 3\n");
        }
    } while(opt < 1 || opt > 3);
    switch(opt) {
        case 1: return IOPT_UPDATE_LOCAL;
        case 2: return IOPT_DELETE_LOCAL;
        case 3: return IOPT_MAINMENU;
    }
    return IOPT_MAINMENU;
}


/* case: IOPT_UPDATE_LOCAL
 * input location info
 *  Params:
 *      Location* location
 *
 *  Return: Option
 *      IOPT_UPDATE_LOCAL
 */
Option inputUpdateLocationInfo(Location old, Location *new){
    printf("Input Category ");
    printCategoryList();
    printf("Location Category (%s): ", old.category);
    fgets(new->category, L_CAT_MAX_LEN, stdin);
    new->category[strlen(new->category) - 1] = '\0';
    if (new->category[0] == '\0') {
        strcpy(new->category, old.category);
    }
    printf("Location Name (%s): ", old.name);
    fgets(new->name, L_NAME_MAX_LEN, stdin);
    new->name[strlen(new->name) - 1] = '\0';
    if (new->name[0] == '\0') {
        strcpy(new->name, old.name);
    }
    printf("Location Note (%s): ", old.note);
    fgets(new->note, L_NOTE_NAME_MAX_LEN, stdin);
    new->note[strlen(new->note) - 1] = '\0';
    if (new->note[0] == '\0') {
        strcpy(new->note, old.note);
    }

    return IOPT_UPDATE_LOCAL;
}