#include "cli_parameters.h"
#include "verbose.h"
#include <string.h>
#include <stdio.h>

#define DEFAULT_ENCODED "encoded.txt"
#define DEFAULT_ORIGINAL "original.txt"
#define CLI_DEFAULT_GENERATE_SIZE 500
#define PARAM_ERROR(err, msg, cli) \
    printf(msg); \
    cli.status = err; \
    return cli

void cli_parameters_ctor(cli_parameters_t* cli) {
    cli->source = DEFAULT_ENCODED;
    cli->destination = DEFAULT_ORIGINAL;
    cli->generate_tests = false;
    cli->size_of_generated = CLI_DEFAULT_GENERATE_SIZE;
    cli->verbose = false;
    cli->status = 0;
    cli->action = NONE;
}
cli_parameters_t resolve_cli_parameters(int argc, char** argv) {
    cli_parameters_t cli;
    cli_parameters_ctor(&cli);
    bool overrided_src = false;
    bool overrided_dest = false;
    for(int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("This application is compressing ASCII text. It is more useful when compressing some default speech, "
                   "because it contains more repeating letter sequences. There are some useful flags:\n"
                   "-g --gen-tests\t\tgenerates tests for this application. next digit optional parameters is optional, defining the size of generated sequence. Test will be written to source directory.\n"
                   "--src\t\t\tdefines source directory. Next word should be the directory to source file(to read from)\n"
                   "--dest\t\t\t defines destination drectory. Next word shoulb be the directory to the destination file"
                   "(where to store result of compression)\n");
            exit(0);
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            set_verbose();
        }
        else if (strcmp(argv[i], "--gen-tests") == 0 || strcmp(argv[i], "-g") == 0) {
            if(cli.action == DECODE) {
                PARAM_ERROR(-4, "Can`t generate tests if decoding.\n", cli);
            }
            ++i;
            cli.generate_tests = true;
            if (i < argc) {
                size_t gen_chars;
                if(sscanf(argv[i], "%zu", &gen_chars) == 1) {
                    cli.size_of_generated = gen_chars;
                } else --i;
            }
        }
        else if (strcmp(argv[i], "--src") == 0) {
            ++i;
            if (i >= argc) {
                PARAM_ERROR(-2, "Wrong --source usage.\n", cli);
            }
            cli.source = argv[i];
            overrided_src = true;
        }
        else if (strcmp(argv[i], "--dest") == 0) {
            ++i;
            if (i >= argc) {
                PARAM_ERROR(-3, "Wrong --destination usage.\n", cli);
            }
            cli.destination = argv[i];
            overrided_dest = true;
        }
        else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encode") == 0) {
            if(cli.action != NONE) {
                PARAM_ERROR(-5, "Can`t set action twice or overriding it.\n", cli);
            }
            cli.action = ENCODE;
            if(!overrided_src)
                cli.source = DEFAULT_ORIGINAL;
            if(!overrided_dest)
                cli.destination = DEFAULT_ENCODED;
        }
        else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decode") == 0) {
            if(cli.action != NONE) {
                PARAM_ERROR(-5, "Can`t set action twice or overriding it.\n", cli);
            }
            if(cli.generate_tests) {
                PARAM_ERROR(-4, "Wrong CLI parameters: can`t generate tests if decoding.\n", cli);
            }
            cli.action = DECODE;
            if(!overrided_src)
                cli.destination = DEFAULT_ORIGINAL;
            if(!overrided_dest)
                cli.source = DEFAULT_ENCODED;
        }
        else {
            if(!overrided_src) {
                cli.source = argv[i];
                overrided_src = true;
            }
            else if (!overrided_dest) {
                cli.destination = argv[i];
                overrided_dest = true;
            } else {
                PARAM_ERROR(-100, "Undefined CLI parameter.\n", cli);
            }
        }
    }
    return cli;
}

