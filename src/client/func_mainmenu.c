#include "func.h"

/* used in mainMenu(), Print out the main menu
 *  Params:
 *      username
 */
void printMainMenu(char username[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Main menu (logged in as %s)\n", username);
    printf("1. Add new location\n");
    printf("2. Share location\n");
    printf("3. Save to server\n");
    printf("4. Restore from server\n");
    printf("5. Log out\n");
    printf("6. Exit\n");
    printf("----------\n");
    printf("Please choose from 1 to 6\n");
    printf("Your choice: ");
}

/* Print our the main menu
 *  Params:
 *      username
 *  Return: Option
 *      IOPT_ADD;
 *      IOPT_SHARE;
 *      IOPT_SAVE;
 *      IOPT_RESTORE;
 *      IOPT_LOGOUT;
 *      IOPT_EXIT
 */
Option mainMenu(char username[]){
    char buf[OPT_MAX_LEN];
    int opt = 0;
    do {
        printMainMenu(username);
        fgets(buf, L_NOTE_NAME_MAX_LEN, stdin);
        buf[strlen(buf) - 1] = '\0';
        opt = atoi(buf);
        if (opt < 1 || opt > 6)
        {
            printf("\n~ Please inut a number from 1 to 6\n");
        }
    } while (opt < 1 || opt > 6);

    switch(opt) {
        case 1: return IOPT_ADD;
        case 2: return IOPT_SHARE;
        case 3: return IOPT_SAVE;
        case 4: return IOPT_RESTORE;
        case 5: return IOPT_LOGOUT;
        case 6: return IOPT_EXIT;
    }
    return IOPT_EXIT;
}