#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "location.h"

#define OPT_MAX_LEN 50

typedef enum {
    IOPT_WELCOME, //welcome screen, see printLoginMenu()
    IOPT_MAINMENU, // home screen, see print Main menu
    IOPT_ADD,
    IOPT_SHARE,
    IOPT_SAVE,
    IOPT_RESTORE,
    IOPT_FETCH,
    IOPT_LOGIN,
    IOPT_SIGNUP,
    IOPT_LOGOUT,
    IOPT_EXIT
} Option;

/* used in loginMenu()
 * Print out the welcome screen
 * Params:
 *
 * Return:
 *
 */
void printWelcomeScreen(){
    int r = rand() % 8;
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
        fgets(buf, OPT_MAX_LEN-1, stdin);
        opt = atoi(buf);
        if (buf[0] == '\n') {
            printf("~ You input nothing, if u wish to exit, please choose 3\n");
        }
        else if (opt < 1 || opt > 3) {
            printf("~ Wrong input, please choose a number, from 1 to 3\n");
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


/* 
 * Params:
 *  char username[]
 *
 * Return: Option
 *  IOPT_WELCOME (if not logged in yet)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputLocation(Location *l){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(add location form)\n");
    printf("Location category\n");
    printf("(Ex: restaurant, museum, school, movie theater, ...)\n")
    printf(": ");
    fgets(l->category, L_CAT_MAX_LEN, stdin);
    l->category[strlen(l->category) - 1] = '\0';
    printf("Location name: ");
    fgets(l->name, L_NAME_MAX_LEN, stdin);
    l->name[strlen(l->name) - 1] = '\0';
}

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
Option mainMenu(char username[]){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printMainMenu(username);
        scanf("%s", buf);
        opt = atoi(buf);
    } while(opt<1 || opt >6);
    switch(opt){
        case 1: return IOPT_ADD;
        case 2: return IOPT_SHARE;
        case 3: return IOPT_SAVE;
        case 4: return IOPT_RESTORE;
        case 5: return IOPT_LOGOUT;
        case 6: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}