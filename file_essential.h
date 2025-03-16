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
int read_file_chunk(FILE* from, wchar_t** buffer);

int write_wide_chunk_to_file(FILE* dest, wchar_t* buffer);
int write_char_chunk_to_file(FILE* dest, char* buffer);
#endif
