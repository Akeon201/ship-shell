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
 * @return array of strings terminating with null. It may return NULL.
*/
char** getPaths() {
    return paths;
}

/**
 * Set the paths
 *
 * @param arr array of strings terminating with NULL
 * @return 0 if success, 1 if failure
*/
int setPaths(char **arr) {
    int i = 0;
    int j = 0;

    while (paths[i] != NULL) { paths[i++] = NULL; }
    i = 0;
    
    if (arr[1] == NULL) { return 0; }
    
    while (arr[i] != NULL) {
        if (strcmp(arr[i], "path") != 0) { 
            paths[j] = strdup(arr[i]);
            j++;
        }
        i++;
    }

    paths[i] = NULL;

    return 0;
}
