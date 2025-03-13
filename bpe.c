#include "bpe.h"
#include "file_essential.h"
#include <wchar.h>
#include <stdlib.h>

int bpe(FILE* source, FILE* dest) {
    char_amount_t source_size = get_file_size(source);
    wchar_t *buffer = (wchar_t*)malloc(sizeof(wchar_t) * (source_size + 1));
    if(buffer == NULL) {
        printf("Error when allocating memory.\n");
        return -1;
    }
    if(fscanf(source, "%ls", buffer) < 0) {
        printf("Unable to scanf text.\n");
        free(buffer);
        return -2;
    }
    
    for(char_amount_t i = 0; i < source_size; ++i) {
        ++buffer[i];
    }
    printf("modified line:\n%ls\n", buffer);
    free(buffer);
    return 0;
}
