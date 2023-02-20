#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * Prints an error, what else would it do?
 *
 * Author: Kenyon Leblanc
*/
void printerror() {

    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));

}
