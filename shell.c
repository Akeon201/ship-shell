#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "header.h"

void setabPath();

char *abPath;

/**
 * Main file. Depending on the input, shell will either go into interactive mode or bash mode.
 *
 * Author: Kenyon Leblanc
*/
int main(int argc, char const *argv[]) {
    if (argc > 2) {
        printerror();
        return 1;
    }
    setabPath();
    //char **arr = getPaths();
   // int i = 0;
   // while (argv[i] != NULL) {
      //  printf("Arg: %s PATH: %s\n", argv[i], arr[i]);
      //  i++;
  //  }

    //Enter interactive mode
    if (argc==1) {
        interactiveMode();
    }
    //Run bash mode with a text file
    else {
         if (bash(argv[1])) { 
            printerror();
            return 1; 
        }
    }

    return 0;
}

void setabPath() {
    abPath = getcwd(NULL, 0);
    if (abPath == NULL) {
        printerror();
        exit(1);
    }
}
