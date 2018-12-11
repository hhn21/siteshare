#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "account.h"
// #include "location.h"

//delete this**********************************************************************************/
#define ACC_NAME_MAX_LEN 45
typedef struct {
    char owner[ACC_NAME_MAX_LEN];           // name of owner of the location
    char sharedBy[ACC_NAME_MAX_LEN];        // name of account who shared location to owner
    time_t createdAt;       // created time
    char category[100];     // location category
    char name[128];         // loction name
    char note[255];         // note
    int seen;
} Location;
/***********************************************************************************************/

#define OPT_MAX_LEN 50

typedef enum option{
    WELCOME, //welcome screen, see printLoginMenu()
    MAINMENU, // home screen, see print Main menu
    ADD,
    SHARE,
    SAVE,
    RESTORE,
    FETCH,
    LOGIN,
    SIGNUP,
    LOGOUT,
    EXIT
}option;

/* used in loginMenu()
 * Print out the welcome screen
 * Params:
 *
 * Return:
 *
 */
void printWelcomeScreen(){
    int r = rand()%8;
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

/* case: WELCOME
 * a welcome screen, user choose login, sign up or exit
 * Params:
 *
 * Return: option
 *  LOGIN
 *  SIGNUP
 *  EXIT
 *
 */
option loginMenu(){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do{
        printWelcomeScreen();
        fgets(buf, OPT_MAX_LEN-1, stdin);
        opt = atoi(buf);
        if (buf[0] == '\n')
        {
            printf("~ You input nothing, if u wish to exit, please choose 3\n");
        }
        else if (opt<1 || opt >3)
        {
            printf("~ Wrong input, please choose a number, from 1 to 3\n");
        }
    }while(opt<1 || opt >3);
    switch(opt){
        case 1: return LOGIN;
        case 2: return SIGNUP;
        case 3: return EXIT;
    }
}

/* case: LOGIN
 * take username and password from user, check Auth()
 * Params:
 *  char username[]
 *  char password[]
 * Return: option
 *  WELCOME (if wrong username or password or input nothing)
 *  MAINMENU (if succeed)
 */
option login(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(login screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN-1, stdin);
    username[strlen(username)-1]='\0';
    if (username[0]=='\0')
    {
        printf("You input nothing, which means back\n");
        return WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN-1, stdin);
    password[strlen(password)-1]='\0';
    if (password[0]=='\0')
    {
        printf("You input nothing, which means back\n");
        return WELCOME;
    }
    //use switch case is better, set return value for the account search
    if (0)
    {
        printf("\n~ User name don't exist.\n");
        return WELCOME;
    }
    if (0)
    {
        printf("\n~ Wrong password.\n");
        return WELCOME;
    }
    printf("Login succeeded\n");
    return MAINMENU;
}

/* case: SIGNUP
 * take username and password from user
 * Params:
 *  char username[]
 *  char password[]
 * Return: option
 *  WELCOME (if wrong username already exist or input nothing)
 *  MAINMENU (if succeed)
 */
option signup(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(sign up screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN-1, stdin);
    username[strlen(username)-1]='\0';
    if (username[0]=='\0')
    {
        printf("You input nothing, which means back\n");
        return WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN-1, stdin);
    password[strlen(password)-1]='\0';
    if (password[0]=='\0')
    {
        printf("You input nothing, which means back\n");
        return WELCOME;
    }
    //use switch case is better, set return value for the account search
    if (0)
    {
        printf("\n~ User name already exist.\n");
        return WELCOME;
    }
    printf("Login succeeded\n");
    return MAINMENU;
}


// /* case: ADD
//  * save new location to local file
//  * Params:
//  *  char username[]
//  *
//  * Return: option
//  *  WELCOME (if not logged in yet)
//  *  MAINMENU (if succeed)
//  */
// option addLocation(char username[]){
//     if (!strcmp(username,""))
//     {
//         printf("~ You need to login first\n");
//         return WELCOME;
//     }
//     Location newlocation;
//     strcpy(newlocation.owner, username);
//     strcpy(newlocation.sharedBy, username);

//     // time_t createdAt;       // created time
//     char category[100];     // location category
//     char name[128];         // loction name
//     char note[255];         // note
//     printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//     printf("(add location form)\n");
//     printf("User name: ");
//     fgets(username, ACC_NAME_MAX_LEN-1, stdin);
//     username[strlen(username)-1]='\0';
//     if (username[0]=='\0')
//     {
//         printf("You input nothing, which means back\n");
//         return WELCOME;
//     }
//     printf("Password : ");
//     fgets(password, ACC_NAME_MAX_LEN-1, stdin);
//     password[strlen(password)-1]='\0';
//     if (password[0]=='\0')
//     {
//         printf("You input nothing, which means back\n");
//         return WELCOME;
//     }
//     //use switch case is better, set return value for the account search
//     if (0)
//     {
//         printf("\n~ User name already exist.\n");
//         return WELCOME;
//     }
//     printf("Login succeeded\n");
//     return MAINMENU;
// }

/* Print our the login menu
 * Params:
 *
 * Return:
 *
 */
void printMainMenu(char username[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Main menu (%s)\n", username);
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
option mainMenu(char username[]){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do{
        printMainMenu(username);
        scanf("%s", buf);
        opt = atoi(buf);
    }while(opt<1 || opt >6);
    switch(opt){
        case 1: return ADD;
        case 2: return SHARE;
        case 3: return SAVE;
        case 4: return RESTORE;
        case 5: return LOGOUT;
        case 6: return EXIT;
    }
}

int main()
{
    char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    option opt;
    opt = loginMenu();
    do{
        switch(opt){
            case WELCOME: opt = loginMenu(); break;
            case MAINMENU: opt = mainMenu(username); break;
            case LOGIN: opt = login(username,password); break;
            case SIGNUP: opt = signup(username,password); break;
            case LOGOUT: printf("log out\n"); break;
            case EXIT: break;
            case ADD: printf("add\n"); break;
            case SHARE: printf("share\n"); break;
            case SAVE: printf("save\n"); break;
            case RESTORE: printf("restore\n"); break;
            case FETCH: printf("fetch\n"); break;
        }
        if (opt == EXIT)
        {
            printf("\nFarewell Site sharer\n\n");
        }
    }while(opt != EXIT);
}