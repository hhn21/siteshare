#include "location.h"

BookRow* _newBookRow(){
	BookRow *row = malloc(sizeof(BookRow));
	row->data = newList();
	return row;
}

/* create new LocationBook
 * Return: 
 * - new LocationBook with empty ownerList and sharedList
 */
LocationBook* newLocationBook(){
	LocationBook *book = malloc(sizeof(LocationBook));
	book->ownerList = newList();
	book->sharedList = newList();
	return book;
}


/* get a BookRow by owner name
 * Params:
 *	 book LocationBook
 *	 owner string owner name
 * Return: 
 *   BookRow indexed by owner name
 *   NULL if not found
 */
BookRow* _getBookRowByOwner(LocationBook* book, char* owner) {
	BookRow *row;
	ListNode *node;
	listTraverse(node, book->ownerList) {
		row = (BookRow*)node->data;
		if(strcmp(owner, row->key) == 0){
			return row;
		}
	}
	return NULL;
}

/* get a BookRow by sharer name
 * Params:
 *	 book LocationBook
 *	 sharedBy string sharer name
 * Return: 
 *   BookRow indexed by sharer name
 *   NULL if not found
 */
BookRow* _getBookRowBySharer(LocationBook* book, char* sharedBy) {
	BookRow *row;
	ListNode *node;
	listTraverse(node, book->sharedList) {
		row = (BookRow*)node->data;
		if(strcmp(sharedBy, row->key) == 0){
			return row;
		}
	}
	return NULL;
}

/* get Locations by owner name
 * Params:
 *	 book LocationBook
 *	 owner string owner name
 * Return: 
 *   Location List indexed by owner name
 *   NULL if not found
 */
List* getLocationsByOwner(LocationBook* book, char* owner) {
	BookRow *row = _getBookRowByOwner(book, owner);
	if(row == NULL) return NULL;
	return row->data;
}

/* get Locations by sharer name
 * Params:
 *	 book LocationBook
 *	 sharedBy string sharer name
 * Return: 
 *   Location List indexed by sharer name
 *   NULL if not found
 */
List* getLocationsBySharer(LocationBook* book, char* sharedBy) {
	BookRow *row = _getBookRowBySharer(book, sharedBy);
	if(row == NULL) return NULL;
	return row->data;
}

/* add Location to LocationBook
 * Params:
 *	 book LocationBook
 *	 location Location
 * Return: 
 * - Location List indexed by sharer name
 * - NULL if not found
 */
void addLocationtoBook(LocationBook* book, Location *location){
	BookRow *row;

	// insert to owner list
	row = _getBookRowByOwner(book, location->owner);
	if(row == NULL) {
		row = _newBookRow();
		strcpy(row->key, location->owner);
		row->data = newList();
		insertAtTail(book->ownerList, (void*)row);
	}
	insertAtHead(row->data, (void*)location);

	// insert to shared list
	row = _getBookRowBySharer(book, location->sharedBy);
	if(row == NULL) {
		row = _newBookRow();
		strcpy(row->key, location->sharedBy);
		row->data = newList();
		insertAtTail(book->sharedList, (void*)row);
	}
	insertAtHead(row->data, (void*)location);
}

/* 
 * import location data from file to a location book
 * params: 
 *		book LocationBook
 *		username string
 * return: 
 *		number of locations were read
 *		-1 if has any error while reading file
 */
int importLocationOfUser(LocationBook* book, char *username){
	char filename[100];
	sprintf(filename, "%s/%s.txt", "data", username);
	FILE *fpin = fopen(filename, "rb");
	if(fpin == NULL){
	    printf("Error! Unable to open %s!\n", filename);
	    return -1;
	}

    int count = 0;

    Location *location;
    while(!feof(fpin)){
	    location = malloc(sizeof(Location));
	    if(fread(location, sizeof(Location), 1, fpin) != 1) {
	    	free(location);
	    	return count;
	    }
	    strcpy(location->owner, username);
  		addLocationtoBook(book, location);
  		count++;
    }
	fclose(fpin);

	return count;
}

/* 
 * save locations from LocationBook to file (1 location per line)
 * params: 
 *		book LocationBook
 *		username string
 */
int addNewLocationOfUser(Location *l, char *username) {
	char filename[100];
	sprintf(filename, "%s/%s.txt", "data", username);
	FILE *fpout = fopen(filename, "ab");
	if(fpout == NULL){
	    printf("Error! Unable to open %s!", filename);
	}
	else {
		if(fwrite(l, sizeof(Location), 1, fpout) != 1) return 0;
	}
	fclose(fpout);
	return 1;
}

/* 
 * save locations from LocationBook to file (1 location per line)
 * params: 
 *		book LocationBook
 *		username string
 */
int saveLocationOfUser(LocationBook* book, char *username) {
	List *locations;
	ListNode *node;
	Location *l;

	char filename[100];
	sprintf(filename, "%s/%s.txt", "data", username);
	FILE *fpout = fopen(filename, "wb");
	if(fpout == NULL){
		printf("Error! Unable to open %s!", filename);
	    return 0;
	}
	else {
		locations = getLocationsByOwner(book, username);
		reverseList(locations);
		listTraverse(node, locations){
			l = (Location*)node->data;
			if(fwrite(l, sizeof(Location), 1, fpout) != 1) return 0;
		}
		reverseList(locations);
	}
	fclose(fpout);
	return 1;
}

/* 
 * free LocationBook from memory
 * params: 
 *		book LocationBook
 */
void destroyLocationBook(LocationBook* book){
	BookRow *row;
	ListNode *node1, *node2;

	listTraverse(node1, book->ownerList) {
		row = (BookRow*)node1->data;
		listTraverse(node2, row->data){
			free(node2->data);
		}
		destroyList(row->data);
	}
	destroyList(book->ownerList);
	destroyList(book->sharedList);
}

/*
 * create location db file of an user (delete old file if existed)
 * params:
 *      username
 */
void createUserDBFile(char* username){
	char filename[100];
	sprintf(filename, "%s/%s.txt", "data", username);
	FILE *fpout = fopen(filename, "w");
	fclose(fpout);
}

/*
 * get locations of an user indexed by giving page
 * Params:
 *   book LocationBook
 *   username string username
 *   page int page number
 *   result array to save the result
 * Return:
 *   Number of locations have been gotten
 */
int getLocationsOfUserByPage(LocationBook *book, char* username, int page, Location *result){
	List *locations = getLocationsByOwner(book, username);
	ListNode *node = locations->root;
	Location *l;
	int i;

	// go to desired page
	for(i = 0; i < (page-1) * PAGE_SIZE; i++){
		if(node == NULL) return 0;
		node = node->next;
	}

	for(i = 0; i < PAGE_SIZE; i++) {
		if(node == NULL) break;
		l = (Location*)node->data;
		result[i] = *l;
		node = node->next;
	}
	return i;
}

/*
 * delete all location of an user from book
 * params:
 *      book LocationBook
 *      username string
 * return:
 *      1 if deleted successfully
 *      0 if not found username in book
 */
int deleteLocationOfUser(LocationBook *book, char* username){
	BookRow *row;
	ListNode *node1;

	listTraverse(node1, book->ownerList) {
		row = (BookRow*)node1->data;
		if(strcmp(username, row->key) != 0) continue;

		destroyList(row->data);
		row->data = newList();
		break;
	}

	return 1;
}

/* get Locations by owner name
 * Params:
 *	 book LocationBook
 *	 owner string owner name
 * Return: 
 *   Location List indexed by owner name
 *   NULL if not found
 */
void getUnseenLocationsByOwner(LocationBook* book, char* owner, List *unseenLocations) {
	List *locations = getLocationsByOwner(book, owner);
	ListNode *node;
	Location *l;

	listTraverse(node, locations) {
		l = (Location*)node->data;
		if(l->seen == 0) {
			insertAtTail(unseenLocations, node->data);
		}
	}
}

/*
 * get locations of an user indexed by giving page
 * Params:
 *   book LocationBook
 *   username string username
 *   page int page number
 *   result array to save the result
 * Return:
 *   Number of locations have been gotten
 */
int getUnseenLocationsOfUserByPage(LocationBook *book, char* username, int page, Location *result){
	List *unseenLocations = newList();
	getUnseenLocationsByOwner(book, username, unseenLocations);

	ListNode *node = unseenLocations->root;
	Location *l;
	int i;

	// go to desired page
	for(i = 0; i < (page-1) * PAGE_SIZE; i++){
		if(node == NULL) return 0;
		node = node->next;
	}

	for(i = 0; i < PAGE_SIZE; i++) {
		if(node == NULL) break;
		l = (Location*)node->data;
		l->seen = 1;
		result[i] = *l;
		node = node->next;
	}

	free(unseenLocations);
	return i;
}


/*
 * delete a location of an user (first location found in book)
 * Params:
 *   book LocationBook
 *   username string username
 *   location pointer to a location to delete
 * Return:
 *   Number of locations have been gotten
 */
int deleteALocationOfUser(LocationBook *book, char* username, Location *location){
	List *locations = getLocationsByOwner(book, username);
	if(locations == NULL) return 0;
	deleteNodeByData(locations, location);
	return 1;
}