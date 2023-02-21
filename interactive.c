#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "header.h"

#define LINELIMIT 300

char line[LINELIMIT];
char *words[(LINELIMIT/3)+1];
char *path;
char *str;
int size;

/**
 * This is the interactive mode for my shell.
 *
 * Author: Kenyon Leblanc
*/
void interactiveMode() {


    while(1) {
        if ((path = getcwd(NULL, 0)) == NULL) { exit(1); }

        printf("ship-shell:> %s$ ", path);

        size = 0;
        
        fgets(line, LINELIMIT, stdin);
        line[strcspn(line, "\n")] = '\0';
        str = strtok(line, " \t\r\n");

        while (str != NULL) {
            words[size++] = str;
            str = strtok(NULL, " \t\r\n");
        }
        
        if (bashMode(words, size)) { printerror(); }

        while (size > 0) { 
            words[--size] = NULL; 
        }

        free(path);
    }
}
