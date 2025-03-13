#include "bpe.h"
#include "file_essential.h"
#include <wchar.h>
#include <stdlib.h>
#include <unistd.h>

// reads one chunk of text from file.
// from - stream to opened file to read from
// buffer - returned string
// returns string's size, or negative value if error.
int read_file_chunk(FILE* from, wchar_t** buffer){
    size_t source_size = get_file_size(from) + 1; // + '0' 
    printf("loaded %zd characters.\n", source_size);
    *buffer = (wchar_t*)malloc(sizeof(wchar_t) * source_size);
    if(*buffer == NULL) {
        printf("Error when allocating memory.\n");
        return -1;
    }
    for(size_t i = 0; i < source_size; ++i) {
        if(feof(from)) {
            printf("Unable to scanf text.\n");
            free(*buffer);
            return -2;
        }

        (*buffer)[i] = fgetc(from);
    }
    (*buffer)[source_size - 1] = '\0'; 
    return source_size; 
}

// counts the most frequent pair of characters.
// str - given wide string
// str_size - size of str
// pair - pointer to an array to assign the most frequent pair
// returns the frequency of returned pair.
size_t most_frequent_pair(const wchar_t* str, size_t str_size, wchar_t (*pair)[2]) {
    size_t mfreq = 0; // max freq
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
    wchar_t replace_to_char = 256;

    size_t iteration = 0;
    for(;;++iteration) {
        printf("%ls\n", *from_buffer);
        freq = most_frequent_pair(*from_buffer, buffer_size, &pair);
        printf("current pair is %ls.\n", pair);
        if(freq == 1) break;
        buffer_size = copy_with_replaced_pair(*from_buffer, buffer_size, to_buffer, pair, replace_to_char++);
        to_buffer[buffer_size] = '\0';

        wchar_t* swap = *from_buffer;
        *from_buffer = to_buffer;
        to_buffer = swap;
    }
    free(to_buffer);
    return buffer_size;
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

    buffer_size = encode(&buffer, buffer_size);
    if(write_chunk_to_file(dest, buffer)) {
        printf("Failed while writing string: %ls.\n", buffer);
        BPE_END(-3);
    }

    free(buffer);
    return 0;
}
