#include "verbose.h"
#include "file_essential.h"

size_t get_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t result = ftell(file);
    fseek(file, 0, SEEK_SET);
    return result;
}
int read_file(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from);
#ifdef VERBOSE
    printf("loaded %zd bytes.\n", source_size);
#endif

    char* read_buffer = (char*)malloc(sizeof(char)*source_size);
    size_t res = fread(read_buffer, sizeof(char), source_size, from);
    if(res < source_size) {
        printf("Failed to read string.\n");
        return -2;
    }
    size_t buffer_size = source_size + 1; // + \0
    *buffer = (wchar_t*)malloc(sizeof(wchar_t) * buffer_size);
    if(*buffer == NULL) {
        printf("Error when allocating memory.\n");
        return -1;
    }
    res = mbstowcs(*buffer, read_buffer, source_size);
    if(res == 0) {
        printf("Error when copying to wide string.\n");
        return -3;
    }
    buffer_size = res + 1;
    (*buffer)[buffer_size - 1] = '\0';
    return source_size; 
}

int write_to_file(FILE* dest, wchar_t* buffer) {
    return fprintf(dest, "%ls", buffer) < 0 ? -1 : 0;
}

#define BPE_END(code) \
free(buffer); \
return code

int from_file(FILE* source, FILE* dest, bpe_func func) {
    wchar_t* buffer;
    int res = read_file(source, &buffer);
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
    res = write_to_file(dest, buffer);
    if(res) {
        printf("Failed while writing string: %ls.\n", buffer);
        BPE_END(-3);
    }
#ifdef VERBOSE
    printf("Result line:\n%ls\n", buffer);
#endif
    BPE_END(0);
}
