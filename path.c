/***
 * For storing and getting path values.
 *
 * Author: Kenyon Leblanc
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "header.h"

/**
 * Gets known paths.
 *
 * @return pointer to N string arrays
*/
char** getPaths() {
    FILE *pf = fopen("path.txt", "r");
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
    int i = 0;
    while (fgets(line, sizeof(line), pf) != NULL) {
        paths[i++] = strdup(line);
    }
    paths[i] = NULL;

    fclose(pf);

    return paths;
}

/**
 * Set the paths.
 *
 * @param paths Character array of strings
 * @return 0 if successful, 1 if problem
*/
int setPaths(char **paths) {
    FILE *pf = fopen("path.txt", "w");

    if (pf == NULL || paths == NULL) {
        return 1;
    }

    if (paths[1] == NULL) {
        ftruncate(fileno(pf), 0);
        return 0;
    }

    int i = 0;
    while (paths[++i] != NULL) {
        fputs(paths[i], pf);
        fputc('\n', pf);
    }

    fclose(pf);
    return 0;
}
