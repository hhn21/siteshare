#include "func.h"

/*
 * used in inputLocationInfo
 */
void printCategoryList() {
    const int CATEGORY_COUNT = 7;
    const char *categories[] = {
        "Building",
        "Restaurant",
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

/* case: IOPT_ADD
 * input location info
 *  Params:
 *      char username[]
 *
 *  Return: Option
 *      IOPT_WELCOME (if not logged in yet)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLocationInfo(Location *location){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(Add location form. Note: Leave a blank input if you want to quit)\n");
    printf("Input Category. ");
    printCategoryList();
    printf("Location Category: ");
    fgets(location->category, L_CAT_MAX_LEN, stdin);
    location->category[strlen(location->category) - 1] = '\0';
    if (location->category[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    printf("Location Name: ");
    fgets(location->name, L_NAME_MAX_LEN, stdin);
    location->name[strlen(location->name) - 1] = '\0';
    if (location->name[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }
    printf("Location Note: ");
    fgets(location->note, L_NOTE_NAME_MAX_LEN, stdin);
    location->note[strlen(location->note) - 1] = '\0';
    if (location->note[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_MAINMENU;
    }

    return IOPT_ADD;
}