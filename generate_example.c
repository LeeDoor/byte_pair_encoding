#include "generate_example.h"
#include <stdio.h>
#include <stdlib.h>

#define FERROR(errc, text, fs) \
    printf(text); \
    fclose(fs); \
    return errc

#define CHUNK_SIZE 200
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a
int generate_example(const char *filename, 
                     charAmount char_amount) {
    FILE* fstream = fopen(filename, "w");
    if(fstream == NULL) {
        printf("Failed on creating file named: %s.\n", filename);
        return -1;
    }
    
    char chunk[CHUNK_SIZE + 1];
    for(charAmount chid = 0; chid < char_amount / (charAmount)CHUNK_SIZE + (char_amount % CHUNK_SIZE != 0); ++chid) {
        size_t actual_chunk_size = MIN(char_amount - chid * CHUNK_SIZE, CHUNK_SIZE);
        for(size_t ch = 0; ch < actual_chunk_size; ++ch) {
            chunk[ch] = rand() % 2 ? 'A' : 'a'; // is capital
            chunk[ch] += rand() % 26; // which letter
        }
        chunk[actual_chunk_size] = '\0';
        fprintf(fstream, "%s", chunk);
    }

    fclose(fstream);
    return 0;
}
