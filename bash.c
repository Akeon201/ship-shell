#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "header.h"

#define AMPER_STR "&"
#define REDIR_STR ">"
#define AMPER_CHAR '&'
#define REDIR_CHAR '>'
#define LINELIMIT 301

int checkOperators(char**);
int checkForLocalCommands(char**);
int executeCommand(char**);
int performLocalCommand(char**);
void resetGlobalVariables();
void setNull(char*);
char** getLines(const char *file);
void setNull(char *arr);
int checkForFile(char *file);
void addPathToFileString(char **file);
void addSpaces(char **str, char key);
void redirectStream(char **arr);

int ampNum;                                             //# of ampersands
int redirNum;                                           //# of redirects
int redirSpot;                                          //spot in array that > appears
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
        if (!strcmp(arr[i], AMPER_STR)) { 
            ampNum++; 
        }
        if (!strcmp(arr[i], REDIR_STR)) {
            redirNum++;
            redirSpot = i;
        }
        i++;
    }
    numCommands = ampNum + 1;

    //If more than one redirection throw an error.
    if (redirNum > 1) { return -1; }
    if (ampNum > 0 && redirNum > 0) { return -1; }

    //If too many or no file trailing >
    if (redirNum && (arr[redirSpot+1] == NULL || arr[redirSpot + 2] != NULL || !strcmp(arr[0], REDIR_STR))) { return -1; }

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
        //freePaths();
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
    int rwPipe[2];
    char message[] = "N";               //Message sent in pipe if no executable is found
    char buff[2];
    char **paths = getPaths();
    char path[100] = {'\0'};

    int i = 0;
   // while (arr[i] != NULL) {
   //     printf("<<<<<<<<  %s  >>>>>>>>>\n", arr[i++]);
    //}
  //  i = 0;
   //printf("1-1\n");
    if (pipe(rwPipe) == -1) { exit(1); }
    pid = fork();
    if (!pid) {
       // printf("1-1\n");
        close(rwPipe[0]);
        //printf("%d\n", redirNum);
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
       // printf("2-2: %s, %s, %d\n", arr[0], arr[1], length);
        waitpid(pid, &result, 0);
        close(rwPipe[1]);
        read(rwPipe[0], buff, sizeof(buff));
        close(rwPipe[0]);
        if (result && length == 1 && checkForFile(arr[0]) && !strcmp(buff, message)) { 
            //printf("3-3\n");
            return bash(arr[0]);
        } else if (!result) {
            return 0;
        } else if (result && strcmp(buff, message)) {
            return 0;
        }
        //printf("Result: %d strcmp: %d", result, strcmp(buff, message));
        return 1;
    }
    return 1;

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
 * Takes in a file name and attempts to execute commands line by line in file.
*/
int bash(const char* word) {
   // printf("1\n");
    char **textCommands = getLines(word);
    if (textCommands == NULL && checkForFile(word)) { return 0; } else if (textCommands == NULL) { return 1; }
    char *words[101];
    char *str;
    int size = 0;
    int i = 0;
    //printf("7\n");

    //while (textCommands[i] == NULL) { i++; }
    //int x = 0;
    while (textCommands[i] != NULL) {

    str = strtok(textCommands[i++], " \t\r\n");

    while (str != NULL) {
        words[size++] = str;
        str = strtok(NULL, " \t\r\n");
    }
    words[size] = NULL;
    
   // printf("Command%d: %s\n", x++, words[0]);
    //x = 0;
    if ( commandMode(words, size) && words != NULL && words[0] != NULL ) { printerror(); }
   // x++;
        
    size--;
    while (size > -1) {
        words[size] = NULL;
        size--;
    }
    size = 0;

    }
    //printf("9\n");

    freeArr(textCommands);
    return 0;
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
 * Read text from a file. MUST FREE RETURNED POINTERS.
 *
 * @return Array of strings terminating with NULL. Returns NULL if error.
*/
char** getLines(const char *file) {
    if (file == NULL) { return NULL; }
    FILE *pf = fopen(file, "r");
   // printf("2\n");
    int numlines = 0;
    char **pths = getPaths();
    int i = 0;

    if (pf == NULL) {
        char path[100] = {'\0'};
        while (pf == NULL && pths[i] != NULL) {
            strcat(path, pths[i]);
            strcat(path, "/");
            strcat(path, file);
            pf = fopen(path, "r");
            setNull(path);
            i++;
        }
    }
    if (pf == NULL) { return NULL; }
    //printf("3\n");

    char c;
    while (feof(pf) == 0) {
        if ((c = fgetc(pf)) == '\n') { numlines++; }
    }
    rewind(pf);
    //printf("4\n");

    char **paths = malloc((numlines + 1) * sizeof(char*));
    if (paths == NULL) { return NULL; }
   // printf("5\n");

   
    char *line = malloc(LINELIMIT);
    if (!line) { exit(1); }
    i = 0;
    while (fgets(line, LINELIMIT, pf) != NULL) { 
       // printf("6\n");
        //printf("File lines: %s", line);
        addSpaces(&line, REDIR_CHAR);
        addSpaces(&line, AMPER_CHAR);
      //  printf("7\n");
       // printf("File lines AFTER: %s", line);
        paths[i] = strdup(line); 
        free(line);
        line = malloc(LINELIMIT);
        i++;
    }
    free(line);
    paths[i] = NULL;
   // printf("6\n");

    fclose(pf);

    return paths;
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
\
/**
 * Feed in an existing file name to change string to absolute path
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
    //setNull(path);
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

    //printf("POINT 1\n");
    while ((*str)[i]) { 
                //printf("POINT 2\n");

        if ((*str)[i] == key && (*str)[i-1] != ' ') { spaceNum++; }
        if ((*str)[i] == key && (*str)[i+1] != ' ') { spaceNum++; }
        i++;
    }
    i = 0;
        //printf("POINT 3 - %d\n", spaceNum);


    if (!spaceNum || (*str)[0] == key || (*str)[1] == key) { return; }

    int len = strlen(*str);
    int x = 0;
    char *temp = malloc(len + spaceNum + 1);
    //printf("POINT 3\n");

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
 * Redirect file stream and frees > and file
*/
void redirectStream(char **arr) {
    //int len = strlen(arr[redirSpot+1]);
    //char temp[len+1];
    //strcpy(temp, arr[redirSpot+1]);
    //temp[len] = '\0';
    //int i = 0;
    //while (arr[i]) { printf("%s\n", arr[i++]); }
    close(STDOUT_FILENO);
    freopen(arr[redirSpot+1], "w", stdout);
    //free(arr[redirSpot+1]);
    arr[redirSpot] = NULL;
    //i = 0;
    //while (arr[i]) { printf("%s", arr[i++]); }
}
