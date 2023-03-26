#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "header.h"

#define AMPER_STR "&"
#define REDIR_STR ">"

int checkOperators(char**);
int checkForLocalCommands(char**);
int executeCommand(char**);
int performLocalCommand(char**);
void resetGlobalVariables();
void setNull(char*);
char** getLines(const char *file);
int checkForFile(char *file);
void addSpaces(char **str, char key);
void redirectStream(char **arr);
int executeMultiCommands(char **voice);
char*** convToMultiLines(char **arr);
void freeMultiLines(char ***arr);

int ampNum;                                             //# of ampersands
int redirNum;                                           //# of redirects
int redirSpot;                                          //spot in array that > appears
int lastAmpSpot;                                        //spot in array that & appears
int numCommands;                                        //number of arguments. If no ampersents 1, otherwise N+1
int localCommands;                                      //# of local built in commands found
int length;                                             //size of array voice
int result;                                             //results
char *commandList[] = {"cd", "path", "exit", NULL};     //array of built in commands ending with NULL

/**
 * This provides the command functionality.
 *
 * Author: Kenyon Leblanc
 * 
 * @param voice Array of string commands terminating with NULL
 * @param size Number of strings
 * @return 0 for success, 1 for failure
*/
int commandMode(char **voice, int size) {
    if (voice == NULL || voice[0] == NULL) { return 1; }
    
    resetGlobalVariables();
    int checkLC = checkForLocalCommands(voice);         //checks for local commands
    int checkOP = checkOperators(voice);                //checks for operators
    length = size;

    if (checkLC == -1 || checkOP == -1) { return 1; }   //Check for errors in check functions
    if (ampNum && !strcmp(voice[0], AMPER_STR)) { return 0; }
    
    if (ampNum) { 
        if (executeMultiCommands(voice)) { return 1; } 
        return 0;
    }

    if (checkLC && !checkOP) {                     //if there are local commands and Check that & and > are not used with local variables
        if (performLocalCommand(voice)) { return 1; }
        return 0;
    }

    if (executeCommand(voice) && !checkLC && !ampNum) { return 1; }

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
        if (!strcmp(arr[i], AMPER_STR)) {  
            ampNum++; 
            lastAmpSpot = i;
        }
        if (!strcmp(arr[i], REDIR_STR)) {
            redirNum++;
            redirSpot = i;
        }
        i++;
    }
    numCommands = ampNum + 1;                       //for future when & is incorporated

    //If ampersands and redirections found.
    if (ampNum > 0 && redirNum > 0) { return 1; }

    //If more than one redirection AND NO AMPS throw an error.
    if (redirNum > 1 && !ampNum) { return -1; }

    //If too many '>' or no file trailing '>'
    if (redirNum && (arr[redirSpot+1] == NULL || arr[redirSpot + 2] != NULL || !strcmp(arr[0], REDIR_STR))) { return -1; }        // || arr[redirSpot + 2] != NULL


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
            if (!strcmp(arr[i], commandList[j])) { localCommands++; }
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
    if (!strcmp(arr[0], commandList[2]) && length == 1) { exit(0); }

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
    int i = 0;
    pid_t pid;
    int rwPipe[2];
    char message[] = "N";               //Message sent in pipe if no executable is found
    char buff[2];
    char **paths = getPaths();
    char path[100] = {'\0'};

    if (pipe(rwPipe) == -1) { exit(1); }
    pid = fork();
    if (!pid) {
        close(rwPipe[0]);
        if (redirNum) { redirectStream(arr); }
        while (paths[i] != NULL) {
            strcat(path, paths[i]);
            strcat(path, "/");
            strcat(path, arr[0]);
            execv(path, arr);
            setNull(path);
            i++;
        }
        write(rwPipe[1], message, sizeof(message));
        close(rwPipe[1]);
        exit(1);
    } else {
        waitpid(pid, &result, 0);
        close(rwPipe[1]);
        read(rwPipe[0], buff, sizeof(buff));
        close(rwPipe[0]);
        if (result && length == 1 && checkForFile(arr[0]) && !strcmp(buff, message)) { 
            return bash(arr[0]);
        } else if (!result) {
            return 0;
        } else if (result && strcmp(buff, message)) {
            return 0;
        }
        return 1;
    }

    return 1;
}

/**
 * Modified execute command for children processes to use only.
*/
void runMultiCommand(char **arr) {

    if (arr == NULL || arr[0] == NULL) { exit(0); }
    resetGlobalVariables();
    int checkOP = checkOperators(arr);                //checks for operators
    if (checkOP == -1) { exit(1); }   //Check for errors in check functions
    if (ampNum && !strcmp(arr[0], AMPER_STR)) { exit(0); }

    int i = 0;
    char **paths = getPaths();
    char path[100] = {'\0'};

    if (redirNum) { redirectStream(arr); }
    while (paths[i] != NULL) {
        strcat(path, paths[i]);
        strcat(path, "/");
        strcat(path, arr[0]);
        execv(path, arr);
        setNull(path);
        i++;
    }

    printerror();
    exit(1);
}

/**
 * Forks process for multiple line inputs and then waits for all children
*/
int executeMultiCommands(char **voice) {

    char ***arr = convToMultiLines(voice);
    pid_t pid;
    int i = 0;
    while (arr[i]) {
        pid = fork();
        if (pid == -1) { return 1; }
        if (!pid) { runMultiCommand(arr[i]); }
        i++;
    }

    while ((pid = waitpid(-1, NULL, 0)) > 0) { ; }
    freeMultiLines(arr);
    return 0;
}

/**
 * Reset global values to 0 so consecutive use from interactive mode will be defined.
*/
void resetGlobalVariables() {
    ampNum = 0;
    redirNum = 0;
    redirSpot = 0;
    numCommands = 0;
    localCommands = 0;
    length = 0;
    result = 0;
}

/**
 * Reset the path string.
*/
void setNull(char *arr) {
    int i = 0;
    while (arr[i] != '\0') {
        arr[i++] = '\0';
    }
}

/**
 * Free pointer to pointers.
*/
void freeArr(char **arr) {
    int i = 0;
    while( arr[i] != NULL) { free(arr[i++]); }
    free(arr);
}


/**
 * Check if file exists.
 * 
 * @return 1 for yes, 0 for no
*/
int checkForFile(char *file) {
    int temp = 0;
    char **paths = getPaths();
    char path[100] = {'\0'};
    int i = 0;

    while (paths[i] != NULL && !temp) {
        strcat(path, paths[i]);
        strcat(path, "/");
        strcat(path, file);
        if (!access(path, F_OK | R_OK)) { temp++; }
        setNull(path);
        i++;
    }
    if (temp) { return 1; }

    strcat(path, abPath);
    strcat(path, "/");
    strcat(path, file);
    if (!access(path, F_OK | R_OK)) { temp++; }
    setNull(path);

    if (temp) { return 1; }

    return 0;
}

/**
 * Feed in an existing *string file name to change *string to absolute path to file
*/
void addPathToFileString(char **file) {
    int temp = 0;
    char **paths = getPaths();
    char path[100] = {'\0'};
    char *tempName = strdup(*file); //free
    int i = 0;

    while (paths[i] != NULL && !temp) {
        strcat(path, paths[i]);
        strcat(path, "/");
        strcat(path, *file);
        if (!access(path, F_OK | R_OK)) { temp++; }
        setNull(path);
        i++;
    }
    i--;
    if (temp) {
        free(*file);
        *file = malloc(sizeof(char) * (strlen(paths[i]) + strlen(tempName) + 2));
        *file = strdup(paths[i]);
        strcat(*file, "/");
        strcat(*file, tempName);
        free(tempName);
        return;        
    }

    strcat(path, abPath);
    strcat(path, "/");
    strcat(path, *file);
    if (!access(path, F_OK | R_OK)) { temp++; }
    if (temp) {
        free(*file);
        *file = malloc(sizeof(char) * (strlen(abPath) + strlen(tempName) + 2));
        *file = strdup(abPath);
        strcat(*file, "/");
        strcat(*file, tempName);
        free(tempName);
        return;        
    }

    free(tempName);
    return;
}

/**
 * Adds spaces between key characters. Takes a *string and character key.
*/
void addSpaces(char **str, char key) {
    int spaceNum = 0;
    int i = 0;

    while ((*str)[i]) { 
        if ((*str)[i] == key && (*str)[i-1] != ' ') { spaceNum++; }
        if ((*str)[i] == key && (*str)[i+1] != ' ') { spaceNum++; }
        i++;
    }

    if (!spaceNum || (*str)[0] == key || (*str)[1] == key) { return; }

    int len = strlen(*str);
    int x = 0;
    char *temp = malloc(len + spaceNum + 1);
    i = 0;
    while ((*str)[i]) {
        if ((*str)[i] == key && (*str)[i-1] != ' ' && i != 0) { temp[x++] = ' '; }
        if ((*str)[i-1] == key && (*str)[i] != ' ' && i != len) { temp[x++] = ' '; }
        temp[x++] = (*str)[i++];
    }
    temp[x] = '\0';

    free(*str);
    *str = temp;
}

/**
 * Redirect file stream, setting '>' to NULL
*/
void redirectStream(char **arr) {
    close(STDOUT_FILENO);
    freopen(arr[redirSpot+1], "w", stdout);
    arr[redirSpot] = NULL;
}

char*** convToMultiLines(char **arr) {

    int greatLineLength = 0;
    int lineCount = 0;
    int wordCount = 0;
    int i = 0;
    while (arr[i]) {

        if (strcmp(arr[i], AMPER_STR)) { 
            wordCount++; 
        } else if (arr[i+1] != NULL) { 
            lineCount++;
            if (wordCount > greatLineLength) { greatLineLength = wordCount; }
            wordCount = 0;
         }

         if (arr[i+1] == NULL) { 
            lineCount++;
            if (wordCount > greatLineLength) { greatLineLength = wordCount; }
          }

        i++;
    }

    char ***temp = malloc(sizeof(char **) * (lineCount + 1));
    for (i = 0; i < lineCount + 1; i++) {
        temp[i] = malloc(sizeof(char *) * (greatLineLength + 1));
    }

    i = 0;
    lineCount = 0;
    wordCount = 0;
    while (arr[i]) {
        if (!strcmp(arr[i], AMPER_STR)) {
            temp[lineCount++][wordCount] = NULL;
            wordCount = 0;
        } else {
            temp[lineCount][wordCount++] = arr[i];
        }
        i++;
    }

    if (!strcmp(arr[i-1], AMPER_STR)) { 
        temp[lineCount] = NULL;
    } else { 
        temp[lineCount][wordCount + 1] = NULL;
        temp[lineCount + 1] = NULL;
    }

    
    return temp;
}

/**
 * Free allocated memory that multilines creates
*/
void freeMultiLines(char ***arr) {
    int i = 0;
    while (arr[i]) {
        free(arr[i]);
        i++;
    }
    free(arr);
}

