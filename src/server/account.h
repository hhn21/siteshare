#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sllist.h"

#define ACC_NAME_MAX_LEN 45

#define ACC_PAGE_SIZE 10

typedef struct {
    char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    int isActive;
    int triedTime;
} Account;

/*
 * Validate username must has only alphabet characters and digits
 * params:
 *		username
 * Returns:
 *		1 if username is valid
 *		0 if username is invalid
 */
int validateUsername(char* username);


/*
 * Validate password cannot have space character
 * params:
 *		password
 * Returns:
 *		1 if password is valid
 *		0 if password is invalid
 */
int validatePassword(char* password);

/*
 * Display 3 info of an account: name, password, ACTIVE or BLOCKED
 * params:
 *		a Account to display info
 */
void displayAccount(Account a);

/*
 * import account data from file to a linked list
 * params:
 *		list: single linked list contains accounts
 *		filename: file name string
 * return:
 *		number of accounts were read
 *		-1 if has any error while reading file
 */
int importAccountFromFile(List* list, char *filename);

/*
 * save accounts from linked list to file (1 account per line, format: username password status)
 * params:
 *		list: single linked list contains accounts
 *		filename: file name string
 */
void saveAccountToFile(List* accounts, char *filename);

// Find account by username
// Return:
// -  Account which has given username
// -  NULL if not found
Account* findAccountByName(List* list, char* username);

/*
 * get users indexed by giving page except 1 user with username store in except
 * Params:
 *   accountList account list to get
 *   page int page number
 *   result array to save the result
 *	 except username to except
 * Return:
 *   Number of users have been gotten
 */
int getUserByPageExcept(List* accountList, int page, Account *result, char *except);

#endif