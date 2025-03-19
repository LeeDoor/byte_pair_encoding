#include "verbose.h"
#include "file_essential.h"

size_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t result = ftell(file);
    fseek(file, 0, SEEK_SET);
    return result;
}
int read_file_chunk(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from) + 1; // + '0' 
#ifdef VERBOSE
    printf("loaded %zd characters.\n", source_size - 1);
#endif
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

#define BPE_END(code) \
free(buffer); \
return code

int from_file(FILE* source, FILE* dest, bpe_func func) {
    wchar_t* buffer;
    int res = read_file_chunk(source, &buffer);
    if(res < 0) return -1;
    size_t buffer_size = res;
#ifdef VERBOSE
    printf("Initial line:\n%ls\n", buffer);
#endif
    res = func(&buffer, buffer_size);
    if(res < 0) {
        printf("Failed while en(de)coding the file.\n");
        BPE_END(-2);
    }
    buffer_size = res;
    if(write_chunk_to_file(dest, buffer)) {
        printf("Failed while writing string: %ls.\n", buffer);
        BPE_END(-3);
    }
#ifdef VERBOSE
    printf("Result line:\n%ls\n", buffer);
#endif
    BPE_END(0);
}
