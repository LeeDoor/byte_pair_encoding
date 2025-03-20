#include "verbose.h"
#include "bpe.h"
#include "replacement.h"
#include "metadata.h"
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// counts the most frequent pair of characters.
// str - given wide string
// str_size - size of str
// pair - pointer to an array to assign the most frequent pair
// returns the frequency of returned pair.
size_t most_frequent_pair(const wchar_t* str, size_t str_size, replacement_t* rep) {
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
            rep->first = a;
            rep->second = b;
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
size_t encode_copy_with_replacement(const wchar_t* from_buffer, 
                               size_t from_buffer_size, 
                               wchar_t* to_buffer, 
                               replacement_t rep) {

    size_t fri = 0, toi = 0;
    for(; fri < from_buffer_size - 1; ++fri, ++toi){ 
        if(from_buffer[fri] == rep.first && from_buffer[fri + 1] == rep.second) {
            to_buffer[toi] = rep.to;
            ++fri;
        }
        else {
            to_buffer[toi] = from_buffer[fri];
        }
    }
    to_buffer[toi++] = '\0';
    return toi;
}

wchar_t* form_the_result_string(const wchar_t* from_buffer, 
                                size_t buffer_size,
                                metadata_t* metadata) {
    size_t metadata_size = metadata_accumulate_string_size(metadata);
    size_t result_size = buffer_size + metadata_size;
    wchar_t* result = (wchar_t*)malloc(sizeof(wchar_t) * result_size);
    if(result == NULL) {
        printf("Failed while allocating memory for result string.\n");
        return NULL;
    }
    int res = metadata_to_string(metadata, result, result_size);
    if(res) {
        printf("Failed while forming metadata to string.\n");
        free(result);
        return NULL;
    }
    wcscpy(result + metadata_size, from_buffer); 
    return result;
} 

int encode_one_pair(wchar_t** from_buffer, size_t* buffer_size,
                    wchar_t** to_buffer,
                    size_t iteration, replacement_t* cur_rep) {
    size_t freq = 0;
    freq = most_frequent_pair(*from_buffer, *buffer_size, cur_rep);
    if(freq <= 2) return 1;
#ifdef VERBOSE
    printf("%zu`th iteration. replacing pair %lc%lc to %lc with frequency %zu\n",
           iteration + 1, cur_rep->first, cur_rep->second, cur_rep->to, freq);
#endif
    *buffer_size = encode_copy_with_replacement(*from_buffer, *buffer_size, 
                                        *to_buffer, 
                                        *cur_rep);
    (*to_buffer)[*buffer_size] = '\0';
    SWAP_BUFFERS(*from_buffer, *to_buffer);
    return 0;
}

int bpe_encode(wchar_t** from_buffer, size_t buffer_size) {
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * buffer_size);
    metadata_t* metadata = metadata_new(buffer_size);
    if(metadata == NULL) {
        printf("Failed to allocate metadata memory.\n");
        free(to_buffer);
        return -1;
    }
    metadata->initial_size = buffer_size;
    size_t iteration = 0;
    replacement_t cur_rep;
    cur_rep.to = 256;
    for(;;++iteration, ++cur_rep.to) {
        if(encode_one_pair(from_buffer, &buffer_size, &to_buffer, iteration, &cur_rep))
            break;
        metadata->replacement_table[iteration] = cur_rep;
        ++metadata->table_size;
    }
    wchar_t* short_buffer = form_the_result_string(*from_buffer, buffer_size,
                                                   metadata);
    if(short_buffer == NULL) {
        printf("Failed to allocate buffer.\n");
        free(to_buffer);
        free(metadata);
        return -2;
    }
    free(*from_buffer);
    *from_buffer = short_buffer;
    free(to_buffer);
    free(metadata);
    return buffer_size;
}

