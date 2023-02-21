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
void resetGlobalVariables();

int ampNum;                                             //# of ampersands
int redirNum;                                           //# of redirects
int redirLink;                                          //spot in array that filestream should be pointed to
int numCommands;                                        //number of arguments. If no ampersents 1, otherwise N+1
int localCommands;                                      //# of local built in commands found
int length;                                             //size of array voice
int result;                                             //results
char *commandList[] = {"cd", "path", "exit", NULL};     //array of built in commands ending with NULL

/**
 * This provides the bash functionality.
 *
 * Author: Kenyon Leblanc
 * 
 * @param voice Array of string commands terminating with NULL
 * @param size Number of strings
 * @return 0 for success, 1 for failure
*/
int bashMode(char **voice, int size) {
    if (voice == NULL || voice[0] == NULL) { return 1; }
    
    resetGlobalVariables();
    int checkLC = checkForLocalCommands(voice);         //checks for local commands
    int checkOP = checkOperators(voice);                //checks for operators
    length = size;

    if (checkLC == -1 || checkOP == -1) { return 1; }   //Check for errors in check functions

    if (checkLC == 1 && !checkOP) {                     //if there are local commands and Check that & and > are not used with local variables
        if (performLocalCommand(voice)) {
            return 1;
        }
        return 0;
    }

    if (executeCommand(voice)) {
        return 1;
    } else {
        return 0;
    }

    return 0;
}

/**
 * Checks and keeps tally of & and >
 *
 * @param arr Array of strings terminating with NULL
 * @return 1 if yes, -1 if error, 0 if none
*/
int checkOperators(char **arr) {
    if (arr == NULL || arr[0] == NULL) { return 1; }

    int i = 0;
    while (arr[i] != NULL) {
        if (!strcmp(arr[i], AMPER)) { 
            ampNum++; 
        }
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

    //If ampersands and redirections found.
    if (ampNum > 0 || redirNum > 0) { return 1; }

    return 0;
}

/**
 * Checks for number of built in commands
 *
 * @param arr Array of strings NULL terminating
 * @return -1 if error, 0 if none, 1 if yes
*/
int checkForLocalCommands(char **arr) {
    if (arr == NULL || arr[0] == NULL) { return 1; }

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

    if (localCommands > 1) { return -1; }               //throw error if more than 1 instance of command is found

    if (localCommands == 1) { return 1; }

    return 0;
}

/**
 * Execute built in commands
 *
 * @param arr Array of strings terminating with NULL
 * @return 0 if successful, 1 if not
*/
int performLocalCommand(char **arr) {
    if (arr == NULL || arr[0] == NULL) { return 1; }

    //cd command
    if (!strcmp(arr[0], commandList[0]) && length != 2) { return 1; }   //cd must be used with 1 other argument or throw this error
    if (!strcmp(arr[0], commandList[0]) && length == 2) {
        result = chdir(arr[1]);
        if (result) {
            return 1;
        } else {
            return 0;
        }
    }

    //path command
    if (!strcmp(arr[0], commandList[1])) {
        result = setPaths(arr);
        if (result) {
            return 1;
        } else {
            return 0;
        }
    }

    //exit command
    if (!strcmp(arr[0], commandList[2]) && length != 1) { return 1; }   //exit must be alone or throw error
    if (!strcmp(arr[0], commandList[2]) && length == 1) {
        exit(0);
    }

    return 0;
}

/**
 * Execute non built-in commands
 * 
 * @param arr array of strings terminating with NULL
 * @return 0 if success, 1 if failure
*/
int executeCommand(char **arr) {
    if (arr == NULL || arr[0] == NULL) { return 1; }

    pid_t pid;
    char **paths = getPaths();
    char path[100] = {'\0'};

    int i = 0;
    pid = fork();
    if (!pid) {
        while (paths[i] != NULL) {
            strcat(path, paths[i]);
            strcat(path, "/");
            strcat(path, arr[0]);
            if (execv(path, arr) != -1) { exit(0); }
            i++;
        }
    } else {
        waitpid(pid, &result, 0);
        if (result == -1) { 
            return 1; 
        } else {
            return 0;
        }
    }

    printerror();
    exit(1);
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
    result = 0;
}
