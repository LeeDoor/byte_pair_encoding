#include "bpe.h"
#include "file_essential.h"
#include <wchar.h>
#include <stdlib.h>

// reads one chunk of text from file.
// from - stream to opened file to read from
// buffer - returned string
// returns string's size, or negative value if error.
int read_file_chunk(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from) + 1; // + '0' 
    *buffer = (wchar_t*)malloc(sizeof(wchar_t) * source_size);
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
    return source_size; // + '0'
}

// counts the most frequent pair of characters.
// str - given wide string
// str_size - size of str
// pair - pointer to an array to assign the most frequent pair
// returns the frequency of returned pair. 0 if no pair occurences.
size_t most_frequent_pair(const wchar_t* str, size_t str_size, wchar_t (*pair)[2]) {
    size_t freq_table[256][256];
    size_t mfreq = 0; // max freq
    for(size_t i = 0; i < str_size - 1; ++i) {
        if(++freq_table[str[i]][str[i + 1]] > mfreq) {
            mfreq = freq_table[str[i]][str[i + 1]];
            (*pair)[0] = str[i];
            (*pair)[1] = str[i + 1];
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
int copy_with_replaced_pair(const wchar_t* from_buffer, 
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
    return 0;
}

// returns new size of encoded text. changes given string's pointer to a new memory block.
size_t encode(wchar_t** from_buffer, size_t buffer_size) {
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
    wchar_t* to_buffer = malloc(sizeof(wchar_t) * buffer_size);
   
    // two characters defining pair
    wchar_t pair[2];
    // amount of occurences of pair above
    size_t freq;
    
    freq = most_frequent_pair(*from_buffer, buffer_size, &pair);
    copy_with_replaced_pair(*from_buffer, buffer_size, to_buffer, pair, 'X');

    free(*from_buffer);
    *from_buffer = to_buffer;
    *from_buffer[buffer_size] = '\0';

    return buffer_size - freq;
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

    buffer_size = encode(&buffer, buffer_size);
    if(write_chunk_to_file(dest, buffer)) {
        printf("Failed while writing string: %ls.\n", buffer);
        BPE_END(-3);
    }

    free(buffer);
    return 0;
}
