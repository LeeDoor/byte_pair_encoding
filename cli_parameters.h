#ifndef CLI_PARAMETERS_H
#define CLI_PARAMETERS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    bool generate_tests;
    size_t size_of_generated;
    char* source;
    char* destination;
    bool verbose;
    int status;
} cli_parameters_t;

void cli_parameters_ctor(cli_parameters_t*);
cli_parameters_t resolve_cli_parameters(int argc, char** argv);

#endif
