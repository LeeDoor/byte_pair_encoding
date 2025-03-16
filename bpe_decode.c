#include "verbose.h"
#include "bpe.h"
#include "file_essential.h"

int bpe_decode_file(FILE* source, FILE* dest) {
    wchar_t* from_buffer;
    int res = read_file_chunk(source, &from_buffer);
    if(res < 0) {
        printf("Failed to read from file.\n");
        return -1;
    }
    size_t buffer_size = res;
#ifdef VERBOSE
    printf("Initial line:\n%ls\n", from_buffer); 
#endif
    
#ifdef VERBOSE
    printf("Decoded line:\n%ls\n", from_buffer); 
#endif
    free(from_buffer);
    return 0;
}
