#include "account.h"

/*
 * Display info of an account: name, password, ACTIVE or BLOCKED
 * params:
 *		a Account to display info
 */
void displayAccount(Account a){
	char activeLabel[10];
	if(a.isActive == 1){
		strcpy(activeLabel, "ACTIVE");
	} else {
		strcpy(activeLabel, "BLOCK");
	}

    printf("%-45s%-45s%-10s\n", a.username, a.password, activeLabel);
};

/*
 * import account data from file to a linked list
 * params:
 *		list: single linked list contains accounts
 *		filename: file name string
 * return:
 *		number of accounts were read
 *		-1 if has any error while reading file
 */
int importAccountFromFile(List* list, char *filename) {
	FILE *fpin = fopen(filename, "r");
	if(fpin == NULL){
	    printf("Error! Unable to open %s!\n", filename);
	    return -1;
	}

    char username[45];
    char password[45];
    int isActive;
    int count = 0;
    Account *a;
    while(!feof(fpin) && fscanf(fpin,"%s %s %d\n", username, password, &isActive) != EOF){
	    a = malloc(sizeof(Account));
	    strcpy(a->username, username);
	    strcpy(a->password, password);
	    a->isActive = isActive;
  		insertAtTail(list, a);
    }
	fclose(fpin);

	return count;
}

/*
 * save accounts from linked list to file (1 account per line, format: username password status)
 * params:
 *		list: single linked list contains accounts
 *		filename: file name string
 */
void saveAccountToFile(List* accounts, char *filename) {
	FILE *fpout = fopen(filename, "w");
	if(fpout == NULL){
	    printf("Error! Unable to open %s!", filename);
	}
	else {
		ListNode* tmp = accounts->root;
		Account* a;
	    while(tmp != NULL){
	    	a = (Account*)(tmp->data);
		    fprintf(fpout, "%s %s %d\n", a->username, a->password, a->isActive);
		    tmp = tmp->next;
	    }
	}
	fclose(fpout);
}

/* Find account by username
 * Return:
 * -  Account which has given username
 * -  NULL if not found
 */
Account* findAccountByName(List* list, char* username) {
	ListNode *tmp = list->root;
	Account *a;
    while(tmp != NULL) {
        a = (Account*)tmp->data;
        if(strcmp(a->username, username) == 0) return a;
        tmp = tmp->next;
    }

    return NULL;
}