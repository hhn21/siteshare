#ifndef __LOCATION_H__
#define __LOCATION_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sllist.h"

#define ACC_NAME_MAX_LEN 45

typedef struct {
    char owner[ACC_NAME_MAX_LEN];			// name of owner of the location
    char sharedBy[ACC_NAME_MAX_LEN];		// name of account who shared location to owner
    time_t createdAt;		// created time
    // float lat;			// location coordinate
    // float lng;			// location coordinate
    char name[128];			// loction name
    char note[255];			// note
    int isRead;             // for system to know if user know or not
} Location;

typedef struct {
	char key[ACC_NAME_MAX_LEN];
	List *data;
} BookRow;

/**********************************************************/
/*  A Hash table to store location list 				  */
/*  Table data 											  */
/*	key Location->Location->Location->...	(BookRow)	  */
/*	key Location->Location->Location->...	(BookRow)	  */
/*	key Location->Location->Location->...	(BookRow)	  */
/**********************************************************/
typedef struct {
	List *ownerList;		// BookRow list index by owner
	List *sharedList;		// BookRow list index by shared user
} LocationBook;

/* create new LocationBook
 * Return:
 * - new LocationBook with empty ownerList and sharedList
 */
LocationBook* newLocationBook();

/* get Locations by owner name
 * Params:
 *	 book LocationBook
 *	 owner string owner name
 * Return:
 *   Location List indexed by owner name
 *   NULL if not found
 */
List* getLocationsByOwner(LocationBook* book, char* owner);

/* get Locations by sharer name
 * Params:
 *	 book LocationBook
 *	 sharedBy string sharer name
 * Return:
 *   Location List indexed by sharer name
 *   NULL if not found
 */
List* getLocationsBySharer(LocationBook* book, char* sharedBy);

/* add Location to LocationBook
 * Params:
 *	 book LocationBook
 *	 location Location
 * Return:
 * - Location List indexed by sharer name
 * - NULL if not found
 */
void addLocationtoBook(LocationBook* book, Location *location);

/*
 * import location data from file to a location book
 * params:
 *		book LocationBook
 *		filename string
 * return:
 *		number of locations were read
 *		-1 if has any error while reading file
 */
int importLocationFromFile(LocationBook* book, char *filename);

/*
 * save locations from LocationBook to file (1 location per line)
 * params:
 *		book LocationBook
 *		filename string file name
 */
void saveLocationBookToFile(LocationBook* book, char *filename);

/*
 * free LocationBook from memory
 * params:
 *		book LocationBook
 */
void destroyLocationBook(LocationBook* book);

#endif