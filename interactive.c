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

int interactiveMode() {


    while(1) {
        if ((path = getcwd(NULL, 0)) == NULL) { exit(1); }

        printf("ship-shell:> %s$ ", path);
        free(path);
        fgets(line, LINELIMIT, stdin);
        line[strcspn(line, "\n")] = '\0';
        str = strtok(line, " \t\r\n");
        size = 0;

        while (str != NULL) {
            words[size++] = str;
            str = strtok(NULL, " \t\r\n");
        }

        if (strcmp("exit", words[0]) == 0 && size == 1) { exit(0); }

        if (bashMode(words, size)) { printerror(); }
    }

}
