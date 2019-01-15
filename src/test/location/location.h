#ifndef __LOCATION_H__
#define __LOCATION_H__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "sllist.h"

#define PAGE_SIZE 10

#define ACC_NAME_MAX_LEN 45
#define L_CAT_MAX_LEN 100
#define L_NAME_MAX_LEN 128
#define L_NOTE_NAME_MAX_LEN 255

typedef struct {
    char owner[ACC_NAME_MAX_LEN];           // name of owner of the location
    char sharedBy[ACC_NAME_MAX_LEN];        // name of account who shared location to owner
    time_t createdAt;                       // created time
    char category[L_CAT_MAX_LEN];           // location category
    char name[L_NAME_MAX_LEN];              // loction name
    char note[L_NOTE_NAME_MAX_LEN];         // note
    int seen;
} Location;

typedef struct {
    char key[ACC_NAME_MAX_LEN];
    List *data;
} BookRow;

/**********************************************************/
/*  A Hash table to store location list                   */
/*  Table data                                            */
/*  key Location->Location->Location->...   (BookRow)     */
/*  key Location->Location->Location->...   (BookRow)     */
/*  key Location->Location->Location->...   (BookRow)     */
/**********************************************************/
typedef struct {
    List *ownerList;        // BookRow list index by owner
    List *sharedList;       // BookRow list index by shared user
} LocationBook;

/* create new LocationBook
 * Return:
 * - new LocationBook with empty ownerList and sharedList
 */
LocationBook* newLocationBook();

/* get Locations by owner name
 * Params:
 *   book LocationBook
 *   owner string owner name
 * Return:
 *   Location List indexed by owner name
 *   NULL if not found
 */
List* getLocationsByOwner(LocationBook* book, char* owner);

/* get Locations by sharer name
 * Params:
 *   book LocationBook
 *   sharedBy string sharer name
 * Return:
 *   Location List indexed by sharer name
 *   NULL if not found
 */
List* getLocationsBySharer(LocationBook* book, char* sharedBy);

/* add Location to LocationBook
 * Params:
 *   book LocationBook
 *   location Location
 * Return:
 * - Location List indexed by sharer name
 * - NULL if not found
 */
void addLocationtoBook(LocationBook* book, Location *location);

/*
 * import location data from file to a location book
 * params:
 *      book LocationBook
 *      username string
 * return:
 *      number of locations were read
 *      -1 if has any error while reading file
 */
int importLocationOfUser(LocationBook* book, char *username);

/*
 * add 1 location to the end of db file
 * params:
 *      location Location
 *      username string
 *      
 */
int addNewLocationOfUser(Location *location, char *username);

/*
 * save locations from LocationBook to file (1 location per line)
 * params:
 *      book LocationBook
 *      username string
 */
int saveLocationOfUser(LocationBook* book, char *username);

/*
 * free LocationBook from memory
 * params:
 *      book LocationBook
 */
void destroyLocationBook(LocationBook* book);

/*
 * create location db file of an user (delete old file if existed)
 * params:
 *      username
 */
void createUserDBFile(char* username);

/*
 * get locations of an user indexed by giving page
 * Params:
 *   book LocationBook
 *   username string username
 *   page int
 *   result array to save the result
 * Return:
 *   Number of locations have been gotten
 */
int getLocationsOfUserByPage(LocationBook *book, char* username, int page, Location *result);

/*
 * delete all location of an user from book
 * params:
 *      book LocationBook
 *      username string
 * return:
 *      1 if deleted successfully
 *      0 if not found username in book
 */
int deleteLocationOfUser(LocationBook *book, char* username);

#endif