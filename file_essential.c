#include "file_essential.h"

size_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t result = ftell(file);
    fseek(file, 0, SEEK_SET);
    return result;
}
