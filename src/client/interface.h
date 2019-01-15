#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "location.h"
#include "account.h"

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
void printWelcomeScreen();

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
Option loginMenu();

/* case: IOPT_LOGIN
 * take username and password from user, check Auth()
 * Params:
 *  char username[]
 *  char password[]
 * Return: Option
 *  IOPT_WELCOME (if wrong username or password or input nothing)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password);

/* case: IOPT_SIGNUP
 * take username and password from user
 * Params:
 *  char username[]
 *  char password[]
 * Return: Option
 *  IOPT_WELCOME (if wrong username already exist or input nothing)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]);

void printCategoryList();

/* case: IOPT_ADD
 * input location info
 * Params:
 *  char username[]
 *
 * Return: Option
 *  IOPT_WELCOME (if not logged in yet)
 *  IOPT_MAINMENU (if succeed)
 */
Option inputLocationInfo(Location *location);

/* Print our the login menu
 * Params:
 *
 * Return:
 *
 */
void printMainMenu(char username[]);

/* Print our the login menu
 * Params:
 *
 * Return:
 *
 */
Option mainMenu(char username[]);

Option inputSharingReceiver(char *receiver);

Option selectLocationToShare(LocationBook *book, char *username, Location **location);

#endif