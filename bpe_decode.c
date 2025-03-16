#include "bpe.h"
#include "file_essential.h"

int bpe_decode_file(FILE* source, FILE* dest) {
    char* buffer = malloc(sizeof(char) * 20);
    if(buffer == NULL) {
        printf("Buffer allocation failed.\n");
        return -1;
    }
    for(int i = 0; i < 20; ++i) buffer[i] = 'a' + i;
    int res = write_char_chunk_to_file(dest, buffer);
    free(buffer);
    if(res) {
        printf("Char chunk writing failed.\n");
        return res;
    }
    return 0;
}
