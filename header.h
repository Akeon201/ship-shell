#if !defined(header)
#define header

extern char *abPath;

/**
 * This is the interactive mode for my shell.
*/
void interactiveMode();

/**
 * This provides the bash functionality.
 * 
 * @param voice Array of string commands terminating with NULL
 * @param size Number of strings
 * @return 0 for success, 1 for failure
*/
int commandMode(char **voice, int size);

/**
 * Prints an error, what else would it do?
*/
void printerror();

/**
 * Gets known paths.
 *
 * @return array of strings terminating with null. It may return NULL.
*/
char** getPaths();

/**
 * Set the paths
 *
 * @param arr array of strings terminating with NULL
 * @return 0 if success, 1 if failure
*/
int setPaths(char **paths);

/**
 * Free paths
*/
void freePaths();

/**
 * Free pointer to pointers.
*/
void freeArr(char **arr);

/**
 * Takes in a file name and attempts to execute commands line by line in file.
*/
int bash(const char* word);

#endif // header
