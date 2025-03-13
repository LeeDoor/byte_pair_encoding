#ifndef GENERATE_EXAMPLE
#define GENERATE_EXAMPLE

// generates example file to test application.
// filename - file where to put example text.
// size - amount of characters to create.
// returns 0 if success.
typedef unsigned long long charAmount;
int generate_example(const char *filename, 
                     charAmount haracters);

#endif
