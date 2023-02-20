#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void freeArr(char**);
char** getLines(const char*);

/**
 * Main file. Depending on the input, shell will either go into interactive mode or bash mode.
 *
 * Author: Kenyon Leblanc
*/
int main(int argc, char const *argv[]) {

    if (argc==1) {
        if (interactiveMode()) { return 1; }
    }
    else {
        //printf("point 1\n");
        char **textCommands = getLines(argv[1]);
        char *words[101];
        char *str;
        int size = 0;
        int i = 0;
        int result;

        //printf("point 2\n");
        while (textCommands[i] != NULL) {


      //  printf("point 3\n");
        str = strtok(textCommands[i++], " \t\r\n");

        while (str != NULL) {
        //    printf("point 4\n");
            words[size++] = str;
            //printf("%s\n", str);
            str = strtok(NULL, " \t\r\n");
        }
        words[size] = NULL;
        //printf("%d", size);

        //printf("point 5\n");
        result = bashMode(words, size);
        if (result) { printerror(); }
        size = 0;




        }
    }

    return 0;
}

/**
 * Free a char** pointer.
*/
void freeArr(char **arr) {
    if (arr != NULL){
        int i = 0;
        while (arr[i] != NULL) {
            free(arr[i++]);
        }
        free(arr);
    }
}

/**
 * Attempts to read a file.
 *
 * @return Array of strings.
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
