#ifndef FILE_ESSENTIAL_H
#define FILE_ESSENTIAL_H

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <unistd.h>

#define FERROR(errc, text, fs) \
    printf(text); \
    fclose(fs); \
    return errc

// reads one chunk of text from file.
// from - stream to opened file to read from
// buffer - returned string
// returns string's size, or negative value if error.
int read_file(FILE* from, wchar_t** buffer);

int write_chunk_to_file(FILE* dest, wchar_t* buffer);

typedef int (*bpe_func)(wchar_t** from_buffer, size_t buffer_size);
typedef enum {
    CHAR,
    WIDE,
} CHAR_TYPE;
int from_file(FILE* source, FILE* dest, bpe_func func);
#endif
