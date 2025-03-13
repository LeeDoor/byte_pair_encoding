#include "generate_example.h"
#include "bpe.h"
#include <locale.h>
#include <time.h>
#include <stdlib.h>

int main() {
    setlocale(LC_ALL, "C.UTF-8");
    srand(time(NULL));
    int res = generate_example("example.txt", 50);
    if(res) return res;
    FILE* source = fopen("example.txt", "r");
    FILE* destination = fopen("destination.txt", "w");
    if(source == NULL || destination == NULL) {
        printf("Failed to open source or destination.\n");
        return -1;
    }
    res = bpe(source, destination);
    fclose(source);
    fclose(destination);
    return res;
}
