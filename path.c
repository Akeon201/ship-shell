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
int check = 0;

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
   // printf("p1\n");
    char *cwd;
    if ((cwd = getcwd(NULL,0)) == NULL) { exit(1); }
    int i = 0;
    int j = 0;
    //printf("p2\n");

    if (!check) {
        while (paths[i] != NULL) { paths[i++] = NULL; }
        check++;
    } else {
        while (paths[i] != NULL) { free(paths[i++]); }
    }
   // printf("p3\n");

    if (arr[1] == NULL) { 
        free(cwd);
        return 0; 
    }
  //  printf("p4\n");
    
    i = 0;
    while (arr[i] != NULL) {
        if (strcmp(arr[i], "path") != 0 && arr[i][0] == '/') { 
            paths[j] = malloc(strlen(arr[i]) + 1);
            if (paths[j] == NULL) { exit(1); }
            strcpy(paths[j], arr[i]);
            j++;
        } else if (strcmp(arr[i], "path") != 0) {
            paths[j] = malloc(strlen(cwd) + 1 + strlen(arr[i]) + 1);
            strcpy(paths[j], cwd);
            strcat(paths[j], "/");
            strcat(paths[j], arr[i]);
            j++;
        }
        i++;
    }
   // printf("p7\n");


    paths[j] = NULL;

   // printf("p8\n");

    free(cwd);
    return 0;
}

/**
 * Free paths
*/
void freePaths() {
    int i = 0;
    if (check) { 
        while (paths[i] != NULL) { free(paths[i++]); } 
}
}
