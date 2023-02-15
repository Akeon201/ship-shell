#include <stdio.h>

//Author: Kenyon Leblanc
//Date Modified: 02/11/23

void bashMode(char **argv) {

    FILE *fp = fopen(argv[1], "r");
    char line[301];

    if (fp == NULL) {
        printf("ERROR: FILE NOT FOUND\n");
    }
    else {
        while (fgets(line, 300, fp)) {
            printf("%s", line);
        }
        printf("\n");
    }
}
