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

/****************************** Welcome screen ******************************/
/* used in welcomeMenu()
 * Print out the welcome screen
 */
void printWelcomeScreen();

/* case: IOPT_WELCOME
 * a welcome screen, user choose login, sign up or exit
 *  Return: Option
 *      IOPT_LOGIN
 *      IOPT_SIGNUP
 *      IOPT_EXIT
 */
Option welcomeMenu();



/****************************** authentication ******************************/
/****************************** Login *****/
/* case: IOPT_LOGIN
 * take username and password from user, check Auth()
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username or password or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password);
/*
 * add username\npassword to buff and return buffsize
 *  params:
 *      char* username
 *      char* password
 *      char** buff
 *  return: int
 *      buffsize
 */
int makeAuthDataBuff(char* username, char* password, char** buff);

/****************************** sign up *****/
/* case: IOPT_SIGNUP
 * take username and password from user
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username already exist or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]);



/****************************** Main menu ******************************/
/* used in mainMenu(), Print out the main menu
 *  Params:
 *      username
 */
void printMainMenu(char username[]);

/* Print our the main menu
 *  Params:
 *      username
 *  Return: Option
 *      IOPT_ADD;
 *      IOPT_SHOW_LOCAL;
 *      IOPT_SHOW_SERVER;
 *      IOPT_SHARE;
 *      IOPT_SAVE;
 *      IOPT_RESTORE;
 *      IOPT_LOGOUT;
 *      IOPT_EXIT
 */
Option mainMenu(char username[]);

/****************************** add location ******************************/
/*
 * used in inputLocationInfo
 */
void printCategoryList();

/* case: IOPT_ADD
 * input location info
 *  Params:
 *      Location* location
 *
 *  Return: Option
 *      IOPT_ADD (if succeed, continue)
 *      IOPT_MAINMENU (if user input nothing)
 */
Option inputLocationInfo(Location *location);



/****************************** share to another user ******************************/
/* Input receiver
 * Params:
 *      receiver 
 * Return:
 *      IOPT_MAINMENU if user input nothing
 *      IOPT_SHARE otherwise
 */
Option inputSharingReceiver(char *receiver);

/*
 * add location and receiver to buff and return buffsize
 *  params:
 *      char* receiver
 *      Location* location
 *      char** buff
 *  return: int
 *      buffsize
 */
int makeShareDataBuff(char* receiver, Location *location, char** buff);

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



/****************************** Show location ******************************/
void printLocationLabel() ;

void printLocationInfo(Location l, int index);

Option showLocalLocation(LocationBook *book, char *username, Location **location);


/****************************** save to server ******************************/
/* Confirm to delete all locations on server and upload all location on local to server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_SAVE if confirm
 */
Option confirmSaveLocation();


/****************************** Restore from server ******************************/
/* Confirm to delete all locations on local and restore all location from server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_RESTORE if confirm
 */
Option confirmRestoreLocation();

/* Print location table labels*/
void printLocationLabel();

/* Print location table info
 * Params:
 *      l Location to print
 *      index index of location
 */
void printLocationInfo(Location l, int index);

/* input page navigation commands
 * Return:
 *      -2 if inputed \p
 *      -1 if inputed \n
 *      0 if inputed nothing
 *      > 0 value if inputed a number within [min,max] 
 */
int pageNavigate(int min, int max);

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
Option showLocalLocation(LocationBook *locationBook, char *username, Location **location);

#endif