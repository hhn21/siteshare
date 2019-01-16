#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sllist.h"

#define ACC_NAME_MAX_LEN 45

typedef struct {
    char username[ACC_NAME_MAX_LEN];
    char password[ACC_NAME_MAX_LEN];
    int isActive;
    int isLoggedIn;
    int triedTime;
} Account;

/*
 * Display 3 info of an account: name, password, ACTIVE or BLOCKED
 * params:
 *		a Account to display info
 */
void displayAccount(Account a);

/* 
 * Find account by username
 *	Params:
 *		List* list account list
 *		char *username
 *	Return:
 * -  Account which has given username
 * -  NULL if not found
 */
Account* findAccountByName(List* list, char* username);

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



#endif