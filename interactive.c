#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "header.h"

#define LINELIMIT 300

/**
 * This is the interactive mode for my shell.
 *
 * Author: Kenyon Leblanc
*/

char line[LINELIMIT];
char *words[(LINELIMIT/3)+1];
char *path;
char *str;
int size;
int result1;

int interactiveMode() {


    while(1) {
        if ((path = getcwd(NULL, 0)) == NULL) { exit(1); }

        printf("ship-shell:> %s$ ", path);
        free(path);
        fgets(line, LINELIMIT, stdin);
        printf("point 1\n");
        line[strcspn(line, "\n")] = '\0';
        str = strtok(line, " \t\r\n");
        printf("point 2\n");
        size = 0;
        result1 = 0;

        while (str != NULL) {
            words[size++] = str;
            str = strtok(NULL, " \t\r\n");
        }
        printf("point 3\n");
        //int x = 0;
        //while (words[x] != NULL) {
            //printf("%s\n", words[x++]);
        //}

        result1 = strcmp("exit", words[0]);
        if (size == 1 && !result1) { 
            exit(0); 
        }
        
        
        printf("point 4\n");
        if (bashMode(words, size)) { printerror(); }
        printf("point 5\n");

        while (size > 0) { 
            words[--size] = NULL; 
        }
    }

}
