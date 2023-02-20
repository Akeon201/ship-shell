#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define AMPER "&"
#define REDIR ">"

int checkOperators(char**);
int checkForLocalCommands(char**);
int executeCommand(char**);
int performLocalCommand(char**);
void freeArr(char**);
void resetGlobalVariables();

int ampNum;
int redirNum;
int redirLink;
int numCommands;
int localCommands;
int length;
char *commandList[] = {"cd", "path", "exit", NULL};

/**
 * This provides the bash functionality.
 *
 * Author: Kenyon Leblanc
*/
int bashMode(char **voice, int size) {
    resetGlobalVariables();
         //int x = 0;
    //while (voice[x]!=NULL) {
   //     printf("%s\n", voice[x++]);
  // }
  //  printf("%d\n", x);
  //  printf("point 1\n");
    int checkLC = checkForLocalCommands(voice);
    int checkOP = checkOperators(voice);
    length = size;

 //   printf("point 2\n");


    if (checkLC == -1 || checkOP == -1) { return 1; }
    //printf("point 3\n");

    if (checkLC > 0 && !checkOP) {
        //printf("point 4\n");
        if (performLocalCommand(voice)) {
            return 1;
        }
        return 0;
    }

    if (checkOP > 0) {

    }

    if (executeCommand(voice)) {
        return 1;
    }

    return 0;
}

/**
 * Checks and keeps tally of operations.
 *
 * @return 1 if yes, -1 if error, 0 if none
*/
int checkOperators(char **arr) {

    //Gets number of ampersands and greater-than signs.
    int i = 0;
    while (arr[i] != NULL) {
        if (!strcmp(arr[i], AMPER)) { ampNum++; }
        if (!strcmp(arr[i], REDIR)) {
            redirNum++;
            redirLink = i + 1;
        }
        i++;
    }
    numCommands = ampNum + 1;

    //If more than one redirection throw an error.
    if (redirNum > 1) { return -1; }
    if (ampNum > 0 && redirNum > 0) { return -1; }

    if (ampNum > 0 || redirNum > 0) { return 1; }

    return 0;
}

/**
 * Checks for number of built in commands
 *
 * @return -1 if error, 0 if none, 1 if yes
*/
int checkForLocalCommands(char **arr) {
    int i = 0;
    int j = 0;

    while(arr[i] != NULL) {

        while(commandList[j] != NULL) {
            if (!strcmp(arr[i], commandList[j])) {
                localCommands++;
            }
            j++;
        }

        j = 0;
        i++;
    }

    if (localCommands > 1) { return -1; }

    if (localCommands == 1) { return 1; }

    return 0;
}

/**
 * Performs built in commands.
 *
 * @return 0 if successful, 1 if not
*/
int performLocalCommand(char **arr) {
    int result;
//printf("point 5\n");
    if (!strcmp(arr[0], commandList[0]) && length != 2) { return 1; }
//printf("point 6\n");
    if (!strcmp(arr[0], commandList[0]) && length == 2) {
       // printf("point 7\n");
        result = chdir(arr[1]);
        //printf("point 8\n");
        if (result) {
            return 1;
        }
    }
    //printf("point 9\n");

    if (!strcmp(arr[0], commandList[1])) {
        result = setPaths(arr);
        if (result) {
            return 1;
        }
    }

    //printf("point 5\n");
    if (!strcmp(arr[0], commandList[2]) && length != 1) { return 1; }
        //printf("point 6\n");
    if (!strcmp(arr[0], commandList[2]) && length == 1) {
        printf("EXITING.....  \n");
        exit(0);
    }
    //printf("point 9\n");

    return 0;
}

int executeCommand(char **arr) {
    if (arr == NULL || arr[0] == NULL) { return 1; }
    printf("point 5\n");
    char **paths = getPaths();
    char path[100] = {'\0'};

    printf("point 6\n");

    int x = 0;
    while (arr[x]!=NULL) {
        printf("%s\n", arr[x++]);
    }

    printf("point 7\n");

    int i = 0;
    while (paths[i] != NULL) {
        strcat(path, paths[i]);
        printf("%s\n", path);
        strcat(path, "/");
        printf("%s\n", path);
        strcat(path, arr[0]);
        printf("%s\n", path);
        execv(path, arr);
        i++;
    }

printf("%s\n", path);
    freeArr(paths);
    return 0;
}

/**
 * Reset global values to 0 so consecutive use from interactive mode will be defined.
*/
void resetGlobalVariables() {
    ampNum = 0;
    redirNum = 0;
    redirLink = 0;
    numCommands = 0;
    localCommands = 0;
    length = 0;
}
