#include "interface.h"

/* used in welcomeMenu()
 * Print out the welcome screen
 */
void printWelcomeScreen(){
    int r = rand()%8 + 1;
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
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
    printf("Please choose 1, 2 or 3\n");
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
            printf("\n~ Wrong input, please choose a number, from 1 to 3\n");
        }
    } while(opt < 1 || opt > 3);
    switch(opt) {
        case 1: return IOPT_LOGIN;
        case 2: return IOPT_SIGNUP;
        case 3: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}

/* case: IOPT_LOGIN
 * take username and password from user, check Auth()
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username or password or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(login screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
    if (password[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    return IOPT_LOGIN;
}

/* case: IOPT_SIGNUP
 * take username and password from user
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username already exist or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(sign up screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
    if (password[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    return IOPT_SIGNUP;
}

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

/* case: IOPT_ADD
 * input location info
 *  Params:
 *      char username[]
 *
 *  Return: Option
 *      IOPT_WELCOME (if not logged in yet)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLocationInfo(Location *location){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(Add location form. Note: Leave a blank input if you want to quit)\n");
    printf("Input Category. ");
    printCategoryList();
    printf("Location Category: ");
    fgets(location->category, L_CAT_MAX_LEN, stdin);
    location->category[strlen(location->category) - 1] = '\0';
    if (location->category[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    printf("Location Name: ");
    fgets(location->name, L_NAME_MAX_LEN, stdin);
    location->name[strlen(location->name) - 1] = '\0';
    if (location->name[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    printf("Location Note: ");
    fgets(location->note, L_NOTE_NAME_MAX_LEN, stdin);
    location->note[strlen(location->note) - 1] = '\0';
    if (location->note[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }

    return IOPT_ADD;
}

/* Print our the main menu
 *  Params:
 *      username
 */
void printMainMenu(char username[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Main menu (logged in as %s)\n", username);
    printf("1. Add new location\n");
    printf("2. Show local location\n");
    printf("3. Show server location\n");
    printf("4. Share location\n");
    printf("5. Save to server\n");
    printf("6. Restore from server\n");
    printf("7. Log out\n");
    printf("8. Exit\n");
    printf("----------\n");
    printf("Please choose from 1 to 8\n");
    printf("Your choice: ");
}

/* Print our the login menu
 * Params:
 *
 * Return:
 *
 */
Option mainMenu(char username[]){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printMainMenu(username);
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        opt = atoi(buf);
        if (opt < 1 || opt > 8)
        {
            printf("\n~ Please inut a number from 1 to 8\n");
        }
    } while (opt < 1 || opt > 8);

    switch(opt) {
        case 1: return IOPT_ADD;
        case 2: return IOPT_SHOW_LOCAL;
        case 3: return IOPT_SHOW_SERVER;
        case 4: return IOPT_SHARE;
        case 5: return IOPT_SAVE;
        case 6: return IOPT_RESTORE;
        case 7: return IOPT_LOGOUT;
        case 8: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}

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
    return showLocalLocation(book, username, location);
}

/* Confirm to delete all locations on server and upload all location on local to server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_SAVE if confirm
 */
Option confirmSaveLocation(){
    char buf[OPT_MAX_LEN];

    printf("This operation will delete all locations on server!\n");
    printf("Do you want to continue? (y/n): ");
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_SAVE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("Wrong input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}

/* Confirm to delete all locations on local and restore all location from server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_RESTORE if confirm
 */
Option confirmRestoreLocation(){
    char buf[OPT_MAX_LEN];

    printf("This operation will delete all local locations!\n");
    printf("Do you want to continue? (y/n): ");
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_RESTORE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("Wrong input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}

/* Print location table labels*/
void printLocationLabel() {
    printf("\n%-5s %-30s %-30s %-30s %-30s %-30s\n", "#", "Created at", "Category", "Name", "Note", "Shared by");
}

/* Print location table info
 * Params:
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
    int opt;
    char buf[OPT_MAX_LEN];

    while(1) {
        printf("\n\nChoose location (%d-%d), enter to quit: ", min, max);
        fgets(buf, OPT_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';

        if(strcmp(buf, "\\p") == 0) return -2;
        if(strcmp(buf, "\\n") == 0) return -1;
        if(buf[0] == '\0') return 0;
        opt = atoi(buf);
        if(opt < min || opt > max) {
            printf("Invalid input, please try again\n");
            continue;
        }
    }
    return 0;
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
        printf("You don't have any location\n");
        return IOPT_MAINMENU;
    }
    
    printLocationLabel();
    listTraverse(node2, locations){
        l = (Location*)node2->data;
        l_a[j] = l;
        j = j % 10 + 1;
        printLocationInfo(*l, j);
        printPageInfo = 1;
        if(j % 10 == 0 || node2->next == NULL) { // if reach the end of page or the end of list 
            do {
                if(printPageInfo == 1) {
                    printf("\nPage %d \t", currPage);
                    if(currPage > 1) printf("Type '\\p' to prev page ");
                    if(node2->next != NULL) printf("Type '\\n' to next page");
                }
                pageCmd = pageNavigate(1, j);

                // if inputed "\p"
                if(pageCmd == -2) {
                    if(currPage == 1) {
                        printf("Error. No previous page!\n");
                        printPageInfo = 0;
                        continue;
                    }
                    // go back to previous page
                    currPage -= 1;
                    tmp.next = locations->root;
                    node2 = &tmp;
                    for(int i = 0; i < (currPage - 1) * 10; i++) node2 = node2->next;
                    j = 0;
                    printLocationLabel();
                    break;
                }

                // if inputed "\n"
                if(pageCmd == -1) {
                    if(node2->next == NULL) {
                        printf("Error. Reached the end of result!\n");
                        printPageInfo = 0;
                        continue;
                    }
                    currPage += 1;
                    printLocationLabel();
                    break;
                }

                // if inputed "\0" (blank)
                if(pageCmd == 0) {
                    printf("You input nothing, which means back\n");
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