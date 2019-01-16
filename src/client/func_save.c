#include "func.h"

/* Confirm to delete all locations on server and upload all location on local to server
 * Return:
 *      IOPT_MAINMENU if not confirm
 *      IOPT_SAVE if confirm
 */
Option confirmSaveLocation(){
    char buf[OPT_MAX_LEN];

    printf("This operation will delete all locations on server!\n");
    printf("Do you want to continue? (y/n): ");
    
    do {
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        if(strcmp(buf, "y") == 0) return IOPT_SAVE;
        if(strcmp(buf, "n") == 0) return IOPT_MAINMENU;
        printf("Wrong input, please try again: ");
    } while (1);
    
    return IOPT_MAINMENU;
}