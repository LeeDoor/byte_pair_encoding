#include <stdio.h>
#include <stdlib.h>
#include "metadata.h"
#include "verbose.h"
#include "bpe.h"
#include "replacement.h"

int decode_copy_with_replacement(const wchar_t* from_buffer, size_t buffer_size,
                          wchar_t* to_buffer, size_t to_buffer_size,
                          replacement_t replacement) {
    size_t fi = 0, ti = 0;
    for(; from_buffer[fi] != '\0'; ++fi, ++ti) {
        if(fi >= buffer_size || ti >= to_buffer_size) return -1;
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
    if(ti == to_buffer_size) return -1;
    to_buffer[ti] = '\0';
    return 0;
}

#define DECODE_EXIT(ecode) \
    free(metadata.replacement_table); \
    free(to_buffer); \
    return ecode
int bpe_decode(wchar_t** from_buffer, size_t buffer_size) {
    metadata_t metadata;
    int res = metadata_from_string(*from_buffer, buffer_size, &metadata);
    if(res) {
        printf("Failed to load replacement table. File corrupted.\n");
        return -1;
    }
    buffer_size = metadata.initial_size;
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * buffer_size);
    if(to_buffer == NULL) {
        free(metadata.replacement_table);
        printf("Failed to allocate memory for swap buffer.\n");
        return -2;
    }
    if(wcscpy(to_buffer, *from_buffer + metadata_accumulate_string_size(&metadata)) == 0) {
        printf("Failed while copying buffers.\n");
        DECODE_EXIT(-3);
    }
    free(*from_buffer);
    *from_buffer = malloc(sizeof(wchar_t) * buffer_size);
    SWAP_BUFFERS(*from_buffer, to_buffer);
    for(size_t i = 0; i < metadata.table_size; ++i) {
        replacement_t cur_rep = metadata.replacement_table[i];
        int copy_res = decode_copy_with_replacement(*from_buffer, buffer_size, 
                                             to_buffer, metadata.initial_size,
                                             cur_rep);
        if(copy_res) {
            printf("Failed to decode. Source is corrupted.\n");
            DECODE_EXIT(-3);
        }
        SWAP_BUFFERS(*from_buffer, to_buffer);
    }
    DECODE_EXIT(0);
}
