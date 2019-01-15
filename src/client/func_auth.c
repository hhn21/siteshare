#include "func.h"

/****************************** Login ******************************/
/* case: IOPT_LOGIN
 * take username and password from user, check Auth()
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username or password or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputLoginCredentials(char* username, char* password){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(login screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
    if (password[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    return IOPT_LOGIN;
}

/*
 * add username\npassword to buff and return buffsize
 *  params:
 *      char* username
 *      char* password
 *      char** buff
 *  return: int
 *      buffsize
 */
int makeAuthDataBuff(char* username, char* password, char** buff) {
    int buffSize;
    buffSize = strlen(username) + strlen(password) + 2;
    *buff = malloc(buffSize);
    sprintf(*buff, "%s\n%s", username, password);
    return buffSize;
}

/****************************** sign up ******************************/
/* case: IOPT_SIGNUP
 * take username and password from user
 *  Params:
 *      char username[]
 *      char password[]
 *  Return: Option
 *      IOPT_WELCOME (if wrong username already exist or input nothing)
 *      IOPT_MAINMENU (if succeed)
 */
Option inputSignupCredentials(char username[], char password[]){
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("(sign up screen)\n");
    printf("User name: ");
    fgets(username, ACC_NAME_MAX_LEN, stdin);
    username[strlen(username) - 1] = '\0';
    if (username[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    printf("Password : ");
    fgets(password, ACC_NAME_MAX_LEN, stdin);
    password[strlen(password) - 1] = '\0';
    if (password[0] == '\0') {
        printf("You input nothing, which means back\n");
        return IOPT_WELCOME;
    }
    return IOPT_SIGNUP;
}
