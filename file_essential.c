#include "file_essential.h"

char_amount_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    char_amount_t result = ftell(file);
    fseek(file, 0, SEEK_SET);
    return result;
}
