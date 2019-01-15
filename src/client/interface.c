#include "interface.h"

/* used in loginMenu()
 * Print out the welcome screen
 * Params:
 *
 * Return:
 *
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
 * Params:
 *
 * Return: Option
 *  IOPT_LOGIN
 *  IOPT_SIGNUP
 *  IOPT_EXIT
 *
 */
Option loginMenu(){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printWelcomeScreen();
        // memset(buf,'\0',strlen(buf)+1);
        // scanf("%[^\n]%*c", buf);
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
 * Params:
 *  char username[]
 *  char password[]
 * Return: Option
 *  IOPT_WELCOME (if wrong username or password or input nothing)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(login screen)\n");
    printf("User name: ");
    // memset(username,'\0',strlen(username)+1);
    // scanf("%[^\n]%*c", username);
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    // memset(password,'\0',strlen(password)+1);
    // scanf("%[^\n]%*c", password);
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
 * Params:
 *  char username[]
 *  char password[]
 * Return: Option
 *  IOPT_WELCOME (if wrong username already exist or input nothing)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(sign up screen)\n");
    printf("User name: ");
    // scanf("%[^\n]%*c", username);
    // username[strlen(username)] = '\0';
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    // scanf("%[^\n]%*c", password);
    // password[strlen(password)] = '\0';
    fgets(password, ACC_NAME_MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
    if (password[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    return IOPT_SIGNUP;
}

void printCategoryList() {
    const int CATEGORY_COUNT = 7;
    const char *categories[] = {
        "Building",
        "Restaurent",
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
 * Params:
 *  char username[]
 *
 * Return: Option
 *  IOPT_WELCOME (if not logged in yet)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputLocationInfo(Location *location){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(add location form)\n");
    printf("Input Category. ");
    printCategoryList();
    printf("Location Category: ");
    // scanf("%[^\n]%*c", location->category);
    // location->category[strlen(location->category)] = '\0';
    fgets(location->category, L_CAT_MAX_LEN, stdin);
    location->category[strlen(location->category) - 1] = '\0';
    printf("Location Name: ");
    // scanf("%[^\n]%*c", location->name);
    // location->name[strlen(location->name)] = '\0';
    fgets(location->name, L_NAME_MAX_LEN, stdin);
    location->name[strlen(location->name) - 1] = '\0';
    printf("Location Note: ");
    // scanf("%[^\n]%*c", location->note);
    // location->note[strlen(location->note)] = '\0';
    fgets(location->note, L_NOTE_NAME_MAX_LEN, stdin);
    location->note[strlen(location->note) - 1] = '\0';

    return IOPT_WELCOME;
}

/* Print our the login menu
 * Params:
 *
 * Return:
 *
 */
void printMainMenu(char username[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Main menu (logged in as %s)\n", username);
    printf("1. Add new location\n");
    printf("2. Share location\n");
    printf("3. Save to server\n");
    printf("4. Restore from server\n");
    printf("5. Log out\n");
    printf("6. Exit\n");
    printf("----------\n");
    printf("Please choose from 1 to 6\n");
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
        // scanf("%[^\n]%*c", buf);
        // buf[strlen(buf)] = '\0';
        opt = atoi(buf);
        if (opt < 1 || opt > 6)
        {
            printf("\n~ Please inut a number from 1 to 6\n");
        }
    } while (opt < 1 || opt > 6);

    switch(opt) {
        case 1: return IOPT_ADD;
        case 2: return IOPT_SHARE;
        case 3: return IOPT_SAVE;
        case 4: return IOPT_RESTORE;
        case 5: return IOPT_LOGOUT;
        case 6: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}

/* case: IOPT_SIGNUP
 * take username and password from user
 * Params:
 *  char username[]
 *  char password[]
 * Return: Option
 *  IOPT_WELCOME (if wrong username already exist or input nothing)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputSharingReceiver(char *receiver){
    printf("Receiver (username): ");
    // scanf("%[^\n]%*c", receiver);
    // username[strlen(username)] = '\0';
    fgets(receiver, ACC_NAME_MAX_LEN, stdin);
    receiver[strlen(receiver) - 1] = '\0';
    if (receiver[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    return IOPT_SHARE;
}

Option selectLocationToShare(LocationBook *book, char *username, Location **location) {
    BookRow *row;
    ListNode *node1, *node2;
    Location *l;
    Location *l_a[10];
    int j = 0;
    char timeString[26];
    struct tm* tm_info;
    char buf[OPT_MAX_LEN];
    int opt = 0;

    listTraverse(node1, book->ownerList) {
        row = (BookRow*)node1->data;
        if(strcmp(username, row->key) != 0) continue;
        printf("%-5s %-20s %-30s %-30s %-50s\n", "#", "Created at", "Category", "Name", "Note");
        listTraverse(node2, row->data){
            l = (Location*)node2->data;
            l_a[j] = l;
            j++;
            tm_info = localtime(&(l->createdAt));
            strftime(timeString, 26, "%Y-%m-%d %H:%M:%S", tm_info);
            printf("%-5d %-20s %-30s %-30s %-50s\n", j, timeString, l->category, l->name, l->note);
            if(j % 10 == 0 || node2->next == NULL) { // if reach the end of page or the end of list 
                do {
                    printf("Choose location(1-10), '\\n' to next page, enter to quit: ");
                    // scanf("%[^\n]%*c", buf);
                    // buf[strlen(buf)] = '\0';
                    fgets(buf, OPT_MAX_LEN, stdin);
                    buf[strlen(buf) - 1] = '\0';
                    if(strcmp(buf, "\\n") == 0) break;
                    if(buf[0] == '\0') {
                        printf("You input nothing, which means back\n");
                        *location = NULL;
                        return IOPT_MAINMENU;
                    }
                    opt = atoi(buf);
                    if(opt < 1 || opt > 10) {
                        printf("Invalid input, please try again\n");
                        continue;
                    }
                    *location = l_a[opt];
                    return IOPT_SHARE;
                } while (1);
            }
        }
    }
    return IOPT_SHARE;
}