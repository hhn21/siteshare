#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#include "account.h"
#include "location.h"
#include "sllist.h"
#include "protocol.h"

int main(int argc, char** argv) {
	// import accounts from file
	List *accountList = newList();
	if(importAccountFromFile(accountList, "./data/account.txt") < 0) return -1;

	// import location from file
	LocationBook *locationBook = newLocationBook();
	if(importLocationFromFile(locationBook, "./data/location.txt") < 0) return -1;

	

	destroyList(accountList);
	destroyLocationBook(locationBook);

	return 1;
}