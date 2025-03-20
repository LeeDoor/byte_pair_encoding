#include <stdio.h>
#include <stdlib.h>
#include "verbose.h"
#include "bpe.h"
#include "replacement.h"

int decode_copy_with_replacement(const wchar_t* from_buffer, size_t buffer_size,
                          wchar_t* to_buffer, size_t to_buffer_size,
                          replacement_t replacement) {
    for(size_t fi = 0, ti = 0; fi < buffer_size; ++fi, ++ti) {
        if(from_buffer[fi] == replacement.to) {
            to_buffer[ti] = replacement.first;
            if(ti == to_buffer_size - 1) return -1;
            ++ti;
            to_buffer[ti] = replacement.second;
        } else {
            if(ti == to_buffer_size) return -1;
            to_buffer[ti] = from_buffer[fi];
        }
    }
    return 0;
}

#define DECODE_EXIT(ecode) \
    free(table); \
    free(to_buffer); \
    return ecode
int bpe_decode(wchar_t** from_buffer, size_t buffer_size) {
#ifdef VERBOSE
    printf("Initial file: %ls\n", *from_buffer);    
#endif
    size_t initial_size = (*from_buffer)[0];
    size_t table_size = 0;
    replacement_t* table = NULL;
    int res = rep_table_from_string(*from_buffer + 1, buffer_size,
                                                 &table, &table_size);
    if(table == NULL || res) {
        printf("Failed to load replacement table. File corrupted.\n");
        return -1;
    }
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * initial_size);
    if(to_buffer == NULL) {
        free(table);
        printf("Failed to allocate memory for swap buffer.\n");
        return -2;
    }
    wcscpy(*from_buffer, to_buffer);
    free(*from_buffer);
    *from_buffer = malloc(sizeof(wchar_t) * initial_size);
    buffer_size = initial_size;
    SWAP_BUFFERS(*from_buffer, to_buffer);
    for(size_t i = 0; i < table_size; ++i) {
        replacement_t cur_rep = table[i];
        int copy_res = decode_copy_with_replacement(*from_buffer, buffer_size, 
                                             to_buffer, initial_size,
                                             cur_rep);
        if(copy_res) {
            printf("Failed to decode. Source is corrupted.\n");
            DECODE_EXIT(-3);
        }
        SWAP_BUFFERS(*from_buffer, to_buffer);
    }
#ifdef VERBOSE
    printf("Final file: %ls\n", *from_buffer);    
#endif
    DECODE_EXIT(0);
}
