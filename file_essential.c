#include "file_essential.h"

size_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t result = ftell(file);
    fseek(file, 0, SEEK_SET);
    return result;
}
int read_file_chunk(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from) + 1; // + '0' 
    printf("loaded %zd characters.\n", source_size);
    *buffer = (wchar_t*)malloc(sizeof(wchar_t) * source_size);
    if(*buffer == NULL) {
        printf("Error when allocating memory.\n");
        return -1;
    }
    for(size_t i = 0; i < source_size - 1; ++i) {
        if(feof(from)) {
            printf("Unable to scanf text.\n");
            free(*buffer);
            return -2;
        }

        (*buffer)[i] = fgetc(from);
    }
    (*buffer)[source_size - 1] = '\0'; 
    return source_size; 
}

int write_chunk_to_file(FILE* dest, wchar_t* buffer) {
    return fprintf(dest, "%ls", buffer) < 0 ? -1 : 0;
}

