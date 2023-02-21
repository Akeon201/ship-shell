#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

char** getLines(const char*);

/**
 * Main file. Depending on the input, shell will either go into interactive mode or bash mode.
 *
 * Author: Kenyon Leblanc
*/
int main(int argc, char const *argv[]) {

    //Enter interactive mode
    if (argc==1) {
        interactiveMode();
    }
    //Run bash mode with a text file
    else {
        char **textCommands = getLines(argv[1]);
        char *words[101];
        char *str;
        int size = 0;
        int i = 0;
        int result;

        while (textCommands[i] != NULL) {

        str = strtok(textCommands[i++], " \t\r\n");

        while (str != NULL) {
            words[size++] = str;
            str = strtok(NULL, " \t\r\n");
        }
        words[size] = NULL;

        result = bashMode(words, size);

        if (result) { printerror(); }
        
        size--;
        while (size > -1) {
            words[size] = NULL;
            size--;
        }
        size = 0;




        }
    }

    return 0;
}

/**
 * Read text from a file.
 *
 * @return Array of strings terminating with NULL
*/
char** getLines(const char *file) {
    if (file == NULL) {
        printerror();
        exit(1);
    }
    FILE *pf = fopen(file, "r");
    char line[100];
    int numlines = 0;

    if (pf == NULL) {
        printerror();
        exit(1);
    }

    char c;
    while (feof(pf) == 0) {
        if ((c = fgetc(pf)) == '\n') {
            numlines++;
        }
    }
    rewind(pf);

    char **paths = malloc((numlines + 1) * sizeof(char*));
    if (paths == NULL) {
        printerror();
        exit(1);
    }

    int i = 0;
    while (fgets(line, sizeof(line), pf) != NULL) {
        paths[i++] = strdup(line);
    }
    paths[i] = NULL;

    fclose(pf);

    return paths;
}
