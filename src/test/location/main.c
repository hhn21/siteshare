#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "location.h"

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

int inputLocationInfo(Location *location){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(add location form)\n");
    printf("Input Category. ");
    printCategoryList();
    printf("Location Category: ");
    fgets(location->category, L_CAT_MAX_LEN, stdin);
    location->category[strlen(location->category) - 1] = '\0';
    printf("Location Name: ");
    fgets(location->name, L_NAME_MAX_LEN, stdin);
    location->name[strlen(location->name) - 1] = '\0';
    printf("Location Note: ");
    fgets(location->note, L_NOTE_NAME_MAX_LEN, stdin);
    location->note[strlen(location->note) - 1] = '\0';

    return 1;
}

int main(){
	LocationBook *locationBook = newLocationBook();
	char username[] = "anhkhoa45";
	int locationNum = 0;
	Location *l;
	BookRow *row;
	ListNode *node1, *node2;

	// import data from file
	locationNum = importLocationOfUser(locationBook, username);
	printf("Imported %d locations\n", locationNum);

	// print book data
	listTraverse(node1, locationBook->ownerList) {
		row = (BookRow*)node1->data;
		printf("Owner: %s\n", row->key);
		listTraverse(node2, row->data){
			l = (Location*)node2->data;
			printf("'%s' %ld '%s' '%s' '%s' %d\n", l->sharedBy, l->createdAt, l->category, l->name, l->note, l->seen);
		}
	}

	// add location to book
	Location *location = malloc(sizeof(Location));
	inputLocationInfo(location);
    strcpy(location->owner, username);
    strcpy(location->sharedBy, "\0");
	location->createdAt = time(NULL);
    location->seen = 1;

    addLocationtoBook(locationBook, location); // add location to in-memory book
    addNewLocationOfUser(location, username); // save location to db

    listTraverse(node1, locationBook->ownerList) {
		row = (BookRow*)node1->data;
		printf("Owner: %s\n", row->key);
		listTraverse(node2, row->data){
			l = (Location*)node2->data;
			printf("'%s' %ld '%s' '%s' '%s' %d\n", l->sharedBy, l->createdAt, l->category, l->name, l->note, l->seen);
		}
	}

    return 0;
}