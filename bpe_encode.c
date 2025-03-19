#include "verbose.h"
#include "bpe.h"
#include "replacement.h"
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// counts the most frequent pair of characters.
// str - given wide string
// str_size - size of str
// pair - pointer to an array to assign the most frequent pair
// returns the frequency of returned pair.
size_t most_frequent_pair(const wchar_t* str, size_t str_size, wchar_t (*pair)[2]) {
    size_t mfreq = 0; // max freq
    if(str_size < 3) return 0;
    for(size_t i = 0; i < str_size - 3; ++i) {
        size_t cur = 1;
        wchar_t a = str[i], b = str[i + 1];
        for(size_t j = i + 2;j < str_size - 1; ++j) {
            if (a == str[j] && b == str[j + 1]) {
                ++cur;
                ++j;
            }
        }
        if(mfreq < cur) {
            mfreq = cur;
            (*pair)[0] = a;
            (*pair)[1] = b;
        }
    }
    return mfreq; 
}

// copies string from from_buffer to to_buffer.
// from_buffer - string to copy from
// from_buffer_size - size of from_buffer
// to_buffer - string to copy to
// pair - pair to replace
// replace_to - character to replace on
// returns size of copied buffer
size_t copy_with_replaced_pair(const wchar_t* from_buffer, 
                               size_t from_buffer_size, 
                               wchar_t* to_buffer, 
                               const wchar_t pair[2], 
                               wchar_t replace_to) {

    size_t fri = 0, toi = 0;
    for(; fri < from_buffer_size - 1; ++fri, ++toi){ 
        if(from_buffer[fri] == pair[0] && from_buffer[fri + 1] == pair[1]) {
            to_buffer[toi] = replace_to;
            ++fri;
        }
        else {
            to_buffer[toi] = from_buffer[fri];
        }
    }
    to_buffer[toi++] = '\0';
    return toi;
}

int copy_to_shorter_memory(wchar_t** from_buffer, size_t buffer_size) {
    wchar_t* new_buffer = malloc(sizeof(wchar_t) * buffer_size);
    if(new_buffer == NULL) {
        printf("Error while allocating shorter chunk of memory.\n");
        return -1;
    }
    for(size_t i = 0; i < buffer_size; ++i) {
        new_buffer[i] = (*from_buffer)[i];
    }
    free(*from_buffer);
    *from_buffer = new_buffer;
    return 0;
}
int metadata_to_string(wchar_t* short_buffer, size_t initial_size) {
    short_buffer[0] = initial_size;
    return 0;
}

wchar_t* form_the_result_string(const wchar_t* from_buffer, 
                                size_t buffer_size,
                                size_t initial_buffer_size,
                                const replacement_t* rep_table,
                                size_t table_size) {
    // buffer[0] = initial size, buffer[1] = amount of replacements
    size_t metadata_size = 1;
    size_t encode_table_size = table_size * 3 + 1;
    buffer_size += encode_table_size + metadata_size;
    wchar_t* short_buffer = malloc(sizeof(wchar_t) * buffer_size);
    metadata_to_string(short_buffer, initial_buffer_size);
    rep_table_to_string(rep_table, table_size, initial_buffer_size, short_buffer); 
    wcscpy(short_buffer + encode_table_size + metadata_size, from_buffer);
    return short_buffer;
} 

#define SWAP_BUFFERS(a, b) \
    wchar_t* swap = a; \
    a = b; \
    b = swap
size_t bpe_encode(wchar_t** from_buffer, size_t buffer_size) {
    const size_t initial_buffer_size = buffer_size;
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * buffer_size);
    wchar_t pair[2];
    size_t freq = 0;
    wchar_t replace_to_char = 256;
    replacement_t *rep_table = rep_table_new(buffer_size);
    size_t iteration = 0;
    for(;;++iteration, ++replace_to_char) {
        freq = most_frequent_pair(*from_buffer, buffer_size, &pair);
        if(freq <= 2) break;
#ifdef VERBOSE
        printf("%zu`th iteration. replacing pair %lc%lc to %lc with frequency %zu.\n", 
               iteration + 1, pair[0], pair[1], replace_to_char, freq);
#endif
        buffer_size = copy_with_replaced_pair(*from_buffer, buffer_size, 
                                              to_buffer, 
                                              pair, replace_to_char);
        to_buffer[buffer_size] = '\0';
        SWAP_BUFFERS(*from_buffer, to_buffer);
        rep_table_add(rep_table, iteration, pair[0], pair[1], replace_to_char);
    }
    wchar_t* short_buffer = form_the_result_string(*from_buffer, buffer_size, 
                                                   initial_buffer_size,
                                                   rep_table, iteration);
    free(*from_buffer);
    *from_buffer = short_buffer;
    free(to_buffer);
    free(rep_table);
    return buffer_size;
}

