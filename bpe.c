#include "bpe.h"
#include "file_essential.h"
#include <wchar.h>
#include <stdlib.h>

int read_file_chunk(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from);
    *buffer = (wchar_t*)malloc(sizeof(wchar_t) * (source_size + 1));
    if(*buffer == NULL) {
        printf("Error when allocating memory.\n");
        return -1;
    }
    int scan_res = fscanf(from, "%ls", *buffer); 
    if(scan_res != 1) {
        printf("Unable to scanf text.\n");
        free(*buffer);
        return -2;
    }
    return source_size;
}
int encode(wchar_t* buffer, size_t buffer_size) {
    // Assume that initial string contains any ASCII characters [0; 255].
    // To generate additional characters, we can only use unused characters.
    // So we'll be using characters from [256;4294967295].
    //
    // 1. count the pair frequency
    // 2. count the most frequent pair
    // 3. replace these pairs with some character
    // 4. store in stack(?) replacement data
    // 5. repeat until no pairs met
    // 6. smh return that replacement stack
    
    return 0;
}

int write_chunk_to_file(FILE* dest, wchar_t* buffer) {
     return fprintf(dest, "%ls", buffer) < 0 ? -1 : 0;
}

#define BPE_END(code) \
    free(buffer); \
    return code

int bpe(FILE* source, FILE* dest) {
    wchar_t* buffer;
    int res = read_file_chunk(source, &buffer);
    if(res < 0) return -1;
    size_t buffer_size = res;
    printf("scanned %zd bytes.\n", buffer_size);

    if(encode(buffer, buffer_size)) {
        printf("Failed while encoding string.\n");
        BPE_END(-2);
    }
    if(write_chunk_to_file(dest, buffer)) {
        printf("Failed while writing string.\n");
        BPE_END(-3);
    }

    free(buffer);
    return 0;
}
