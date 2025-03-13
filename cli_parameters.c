#include "cli_parameters.h"
#include <string.h>
#include <stdio.h>

#define DEFAULT_SOURCE "source.txt"
#define DEFAULT_DESTINATION "destination.txt"
#define CLI_DEFAULT_GENERATE_SIZE 500
#define PARAM_ERROR(err, msg, cli) \
    printf(msg); \
    cli.status = err; \
    return cli

void cli_parameters_ctor(cli_parameters_t* cli) {
    cli->source = DEFAULT_SOURCE;
    cli->destination = DEFAULT_DESTINATION;
    cli->generate_tests = false;
    cli->size_of_generated = CLI_DEFAULT_GENERATE_SIZE;
    cli->status = 0;
}
cli_parameters_t resolve_cli_parameters(int argc, char** argv) {
    cli_parameters_t cli;
    cli_parameters_ctor(&cli);
    for(int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("This application is compressing ASCII text. It is more useful when compressing some default speech,"
                   "because it contains more repeating letter sequences. There are some useful flags:\n"
                   "-g --generate-tests\t\tgenerates tests for this application. Tests will be written to source directory.\n"
                   "--gen-chars\t\t\tsets generating file length. Next word should be positive integer."
                   "Default length is %d\n"
                   "--source\t\t\tdefines source directory. Next word should be the directory to source file(to read from)\n"
                   "--destination\t\t\t defines destination drectory. Next word shoulb be the directory to the destination file"
                   "(where to store result of compression)\n", CLI_DEFAULT_GENERATE_SIZE);
            exit(0);
        }
        else if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--generate-tests") == 0) {
            cli.generate_tests = true;
        }
        else if (strcmp(argv[i], "--gen-chars") == 0) {
            ++i;
            if (i >= argc) {
                PARAM_ERROR(-1, "Wrong --gen-chars usage.\n", cli);
            }
            size_t gen_chars;
            if(sscanf(argv[i], "%zu", &gen_chars) != 1) {
                PARAM_ERROR(-1, "Wrong --gen-chars usage.\n", cli);
            }
            cli.size_of_generated = gen_chars;
        }
        else if (strcmp(argv[i], "--source") == 0) {
            ++i;
            if (i >= argc) {
                PARAM_ERROR(-2, "Wrong --source usage.\n", cli);
            }
            cli.source = argv[i];
        }
        else if (strcmp(argv[i], "--destination") == 0) {
            ++i;
            if (i >= argc) {
                PARAM_ERROR(-3, "Wrong --destination usage.\n", cli);
            }
            cli.destination = argv[i];
        }
        else {
            PARAM_ERROR(-100, "Undefined CLI parameter.\n", cli);
        }
    }
    return cli;
}

