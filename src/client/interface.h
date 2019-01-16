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
    IOPT_WELCOME, //welcome screen, see printWelcomeScreen()
    IOPT_MAINMENU, // home screen, see print Main menu
    IOPT_ADD,
    IOPT_SHOW_LOCAL,
    IOPT_SHOW_SERVER,
    IOPT_SHARE,
    IOPT_SAVE,
    IOPT_RESTORE,
    IOPT_FETCH,
    IOPT_LOGIN,
    IOPT_SIGNUP,
    IOPT_LOGOUT,
    IOPT_EXIT
} Option;

/* used in welcomeMenu()
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
Option welcomeMenu();

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

/* Print out the login menu
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

/* Input receiver
 * Params:
 *      receiver 
 * Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option inputSharingReceiver(char *receiver);

/* Print out locations of an user an let user select 1
 * Params:
 *      book LocationBook
 *      username current user to select location
 *      location location pointer to save selected location
 * Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option selectLocationToShare(LocationBook *book, char *username, Location **location);

/* Confirm to delete all locations on server and upload all location on local to server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_SAVE if confirm
 */
Option confirmSaveLocation();

/* Confirm to delete all locations on local and restore all location from server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_RESTORE if confirm
 */
Option confirmRestoreLocation();

Option showLocalLocation(LocationBook *locationBook, char *username, Location **location);

#endif