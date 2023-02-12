#include <stdio.h>
#include "header.h"

//Author: Kenyon Leblanc
//Date Modified: 02/11/23

int main(int argc, char const *argv[]) {

    if (argc==1)
        interactiveMode();
    else
        bashMode(argv);

    return 0;
}
