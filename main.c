#include "cli_parameters.h"
#include "generate_example.h"
#include "bpe.h"
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
    res = bpe_encode(source, destination);
    fclose(source);
    fclose(destination);
    return res;
}
