#include "verbose.h"
#include "bpe.h"
#include "file_essential.h"
#include <wchar.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    wchar_t first;
    wchar_t second;
    wchar_t to;
} replacement_t;

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

#define SWAP_BUFFERS(a, b) \
    wchar_t* swap = a; \
    a = b; \
    b = swap
#define ADD_TO_TABLE(table, id, arg_first, arg_second, arg_to) \
    replacement_t rep; \
    rep.first = arg_first; \
    rep.second = arg_second; \
    rep.to = arg_to; \
    table[id] = rep

// returns new size of encoded text. changes given string's pointer to a new memory block.
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

// instead of allocating new memory each time, we will exchange
// data from buffer to this pointer. result will be third block
// of memory with changed lower size.
size_t encode(wchar_t** from_buffer, size_t buffer_size) {
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * buffer_size);
    // two characters defining pair
    wchar_t pair[2];
    // amount of occurences of pair above
    size_t freq;
    wchar_t replace_to_char = 256;
    replacement_t *rep_table = (replacement_t*) malloc(sizeof(replacement_t) * buffer_size);
    size_t iteration = 0;
    for(;;++iteration, ++replace_to_char) {
        freq = most_frequent_pair(*from_buffer, buffer_size, &pair);
        if(freq <= 1) break;
#ifdef VERBOSE
        printf("%zu`th iteration. replacing pair %lc%lc to %lc with frequency %zu.\n", 
               iteration + 1, pair[0], pair[1], replace_to_char, freq);
#endif
        buffer_size = copy_with_replaced_pair(*from_buffer, buffer_size, to_buffer, pair, replace_to_char);
        to_buffer[buffer_size] = '\0';
        SWAP_BUFFERS(*from_buffer, to_buffer);
        ADD_TO_TABLE(rep_table, iteration, pair[0], pair[1], replace_to_char);
    }
    free(to_buffer);
    wchar_t* short_buffer = malloc(sizeof(wchar_t) * buffer_size + iteration);
    for(size_t i = 0; i < iteration; ++i){
        short_buffer[i] = 'Z';
    }
    //wcscpy(short_buffer + iteration, *from_buffer);
    free(*from_buffer);
    *from_buffer = short_buffer;
    return buffer_size + iteration;
}

#define BPE_END(code) \
free(buffer); \
return code

int bpe_encode(FILE* source, FILE* dest) {
    wchar_t* buffer;
    int res = read_file_chunk(source, &buffer);
    if(res < 0) return -1;
    size_t buffer_size = res;
#ifdef VERBOSE
    printf("Initial line:\n%ls\n", buffer);
#endif
    buffer_size = encode(&buffer, buffer_size);
    if(write_chunk_to_file(dest, buffer)) {
        printf("Failed while writing string: %ls.\n", buffer);
        BPE_END(-3);
    }
#ifdef VERBOSE
    printf("Result line:\n%ls\n", buffer);
#endif
    BPE_END(0);
}
