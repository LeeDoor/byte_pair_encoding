#ifndef GENERATE_EXAMPLE
#define GENERATE_EXAMPLE
#include "file_essential.h"

// generates example file to test application.
// filename - file where to put example text.
// size - amount of characters to create.
// returns 0 if success.
int generate_example(const char *filename, 
                     size_t characters);

#endif
