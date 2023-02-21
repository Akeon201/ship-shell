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

char *paths[20] = { "/bin", NULL};

/**
 * Gets known paths.
 *
 * @return pointer to N string arrays
*/
char** getPaths() {
    return paths;
  /**  FILE *pf = fopen("path.txt", "r");
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
        paths[i++] = strtok(strdup(line), " \t\r\n");
    }
    paths[i] = NULL;

    fclose(pf);

    return paths;
    */
}

/**
 * Set the paths.
 *
 * @param paths Character array of strings
 * @return 0 if successful, 1 if problem
*/

int setPaths(char **arr) {
    int i = 0;
    int j = 0;
  /**      int x = 0;
    while (paths[x]!=NULL) {
        printf("%s\n", paths[x++]);
    }

    x = 0;
    while (arr[x]!=NULL) {
        printf("%s\n", arr[x++]);
    }
*/
    if (paths[i] != NULL) { paths[i++] = NULL; }
    i = 0;
//printf("point 111\n");
    if (arr[1] == NULL) { return 0; }
//printf("point 222\n");
    while (arr[i] != NULL) {
        if (strcmp(arr[i], "path") != 0) { 
            paths[j] = strdup(arr[i]);
            j++;
        }
        i++;
    }
    //printf("point 333\n");
    paths[i] = NULL;
    /**printf("point 444\n");
             x = 0;
    while (paths[x]!=NULL) {
        printf("%s\n", paths[x++]);
    }
    */

    return 0;

    /**
    FILE *pf = fopen("path.txt", "w");

    if (pf == NULL || paths == NULL) {
        return 1;
    }

    if (paths[1] == NULL) {
        ftruncate(fileno(pf), 0);
        fclose(pf);
        return 0;
    }

    int i = 0;
    while (paths[++i] != NULL) {
        fputs(paths[i], pf);
        fputc('\n', pf);
    }

    fclose(pf);
    return 0;
    */
}
