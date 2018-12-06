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
	insertAtTail(row->data, (void*)location);

	// insert to shared list
	row = _getBookRowBySharer(book, location->sharedBy);
	if(row == NULL) {
		row = _newBookRow();
		strcpy(row->key, location->sharedBy);
		row->data = newList();
		insertAtTail(book->sharedList, (void*)row);
	}
	insertAtTail(row->data, (void*)location);
}

/* 
 * import location data from file to a location book
 * params: 
 *		book LocationBook
 *		filename string
 * return: 
 *		number of locations were read
 *		-1 if has any error while reading file
 */
int importLocationFromFile(LocationBook* book, char *filename){
	FILE *fpin = fopen(filename, "r");
	if(fpin == NULL){
	    printf("Error! Unable to open %s!\n", filename);
	    return -1;
	}

    char owner[ACC_NAME_MAX_LEN];			// name of owner of the location
    char sharedBy[ACC_NAME_MAX_LEN];		// name of account who shared location to owner
    time_t createdAt;		// created time
    float lat;				// location coordinate
    float lng;				// location coordinate
    char name[128];			// loction name
    char note[255];			// note
    int count = 0;

    Location *location;
    while(!feof(fpin) && fscanf(fpin,"%s %s %ld %f %f %s %s\n", owner, sharedBy, (long*)&createdAt, &lat, &lng, name, note)){
	    location = malloc(sizeof(Location));
	    strcpy(location->owner, owner);
	    strcpy(location->sharedBy, sharedBy);
	    location->createdAt = createdAt;
	    location->lat = lat;
	    location->lng = lng;
	    strcpy(location->name, name);
	    strcpy(location->note, note);
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
 *		filename string file name 
 */
void saveLocationBookToFile(LocationBook* book, char *filename) {
	BookRow *row;
	ListNode *node1, *node2;
	Location *l;

	FILE *fpout = fopen(filename, "w");
	if(fpout == NULL){
	    printf("Error! Unable to open %s!", filename);
	}
	else {
		listTraverse(node1, book->ownerList) {
			row = (BookRow*)node1->data;
			listTraverse(node2, row->data){
				l = (Location*)node2->data;
				fprintf(fpout, "%s %s %ld %f %f %s %s\n", l->owner, l->sharedBy, l->createdAt, l->lat, l->lng, l->name, l->note);
			}
		}
	}
	fclose(fpout);
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