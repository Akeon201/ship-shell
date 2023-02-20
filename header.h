#if !defined(header)
#define header

/// @brief User has choosen the interactive mode.
int interactiveMode();
/// @brief User has choosen the bash mode.
int bashMode(char **voice, int size);
/// @brief prints error
void printerror();
/// @brief Gets executable paths
/// @return returns pointer to array of strings
char** getPaths();
/// @brief Set the executable paths, persisting
/// @param paths array of strings to set path to
int setPaths(char **paths);
/// @brief Free char** array
void freeArr(char**);

#endif // header
