#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Author: Kenyon Leblanc
//Date Modified: 02/11/23

#define LINELIMIT 300

void strLower();

char line[LINELIMIT];
char *words[LINELIMIT/2];
char *str;
int num;
int num2;

void interactiveMode() {


    while(1) {
        printf("ship-shell:> ");
        fgets(line, LINELIMIT, stdin);
        line[strcspn(line, "\n")] = '\0';
        //strLower(line);
        str = strtok(line, " \t\r\n");
        num = 0;

        while (str != NULL) {
            words[num++] = str;
            str = strtok(NULL, " \t\r\n");
        }

        num2 = 0;
        while (num2 < num ) {
            printf("%s ", words[num2++]);
        }
        printf("\n");

        if (strcmp("exit", words[0]) == 0)
            break;
    }
}

//Takes a string and changes all characters to lower case.
void strLower(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i++]);
    }
}
