#include "generate_example.h"
#include "file_essential.h"
#include <stdio.h>
#include <stdlib.h>


#define CHUNK_SIZE 200
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a > b ? b : a
int generate_example(const char *filename, 
                     size_t char_amount) {
    FILE* fstream = fopen(filename, "w");
    if(fstream == NULL) {
        printf("Failed on creating file named: %s.\n", filename);
        return -1;
    }
    
    char chunk[CHUNK_SIZE + 1];
    for(size_t chid = 0; chid < char_amount / (size_t)CHUNK_SIZE + (char_amount % CHUNK_SIZE != 0); ++chid) {
        size_t actual_chunk_size = MIN(char_amount - chid * CHUNK_SIZE, CHUNK_SIZE);
        for(size_t ch = 0; ch < actual_chunk_size; ++ch) {
            if(ch > 0 && rand() % 10 == 0) {
                // same letter
                chunk[ch] = ' ';
            } else {
                chunk[ch] = rand() % 2 ? 'A' : 'a'; // is capital
                chunk[ch] += rand() % 2; // which letter
            }
        }
        chunk[actual_chunk_size] = '\0';
        if(fprintf(fstream, "%s", chunk) < 0) {
            FERROR(-2, "Error while writing random text to file.\n", fstream);
        }
    }

    fclose(fstream);
    return 0;
}
