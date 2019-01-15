#include "func.h"


/* Confirm to delete all locations on local and restore all location from server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_RESTORE if confirm
 */
Option confirmRestoreLocation(){
    char buf[OPT_MAX_LEN];

    printf("This operation will delete all local locations!\n");
    printf("Do you want to continue? (y/n): ");
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_RESTORE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("Wrong input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}