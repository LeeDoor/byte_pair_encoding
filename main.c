#include "cli_parameters.h"
#include "generate_example.h"
#include "bpe.h"
#include "file_essential.h"
#include <locale.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    setlocale(LC_ALL, "C.UTF-8");
    srand(time(NULL));
    cli_parameters_t cli = resolve_cli_parameters(argc, argv);
    if(cli.status != 0) return cli.status;

    int res;
    if(cli.generate_tests) {
        res = generate_example(cli.source, cli.size_of_generated);
        if(res) return res;
    }
    FILE* source = fopen(cli.source, "r");
    FILE* destination = fopen(cli.destination, "w");
    if(source == NULL || destination == NULL) {
        printf("Failed to open source or destination.\n");
        return -1;
    }
    switch(cli.action) {
        case ENCODE:
            printf("Encoding.\n");
            res = from_file(source, destination, bpe_encode);
        break;
        case DECODE:
            printf("Decoding.\n");
            //res = from_file(source, destination, bpe_decode);
        break;
        case NONE:
            printf("Action type not selected. Please use -e or -d to encode/decode.\n");
            res = 1;
        break;
    }
    fclose(source);
    fclose(destination);
    return res;
}
