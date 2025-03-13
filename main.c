#include "generate_example.h"
#include <time.h>
#include <stdlib.h>

int main() {
    srand(time(NULL));
    int result = generate_example("example.txt", 4621);
    return result;
}
