#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "header.h"

#define AMPER_CHAR '&'
#define REDIR_CHAR '>'
#define LINELIMIT 301

char** getLines(const char *file);

/**
 * Takes in a file name and attempts to execute commands line by line in file.
 * 
 * Author: Kenyon Leblanc
*/
int bash(const char* word) {
    char **textCommands = getLines(word);
    if (textCommands == NULL && checkForFile(word)) { return 0; } else if (textCommands == NULL) { return 1; }
    char *words[101];
    char *str;
    int size = 0;
    int i = 0;

    while (textCommands[i] != NULL) {

    str = strtok(textCommands[i++], " \t\r\n");
    while (str != NULL) {
        words[size++] = str;
        str = strtok(NULL, " \t\r\n");
    }
    words[size] = NULL;
    
    if ( commandMode(words, size) && words != NULL && words[0] != NULL ) { printerror(); }
        
    size--;
    while (size > -1) {
        words[size] = NULL;
        size--;
    }
    size = 0;

    }

    freeArr(textCommands);
    return 0;
}

/**
 * Read text from a file. MUST FREE RETURNED POINTERS.
 *
 * @return Array of strings terminating with NULL. Returns NULL if error.
*/
char** getLines(const char *file) {
    if (file == NULL) { return NULL; }
    FILE *pf = fopen(file, "r");
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

    char c;
    while (feof(pf) == 0) {
        if ((c = fgetc(pf)) == '\n') { numlines++; }
    }
    rewind(pf);

    char **paths = malloc((numlines + 1) * sizeof(char*));
    if (paths == NULL) { return NULL; }
   
    char *line = malloc(LINELIMIT);
    if (!line) { exit(1); }

    i = 0;
    while (fgets(line, LINELIMIT, pf) != NULL) { 
        addSpaces(&line, REDIR_CHAR);
        addSpaces(&line, AMPER_CHAR);
        paths[i] = strdup(line); 
        free(line);
        line = malloc(LINELIMIT);
        i++;
    }
    free(line);
    paths[i] = NULL;

    fclose(pf);

    return paths;
}
