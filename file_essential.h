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

// gets file's size. moving cursor to the begining of the file.
// takes pointer to opened file
size_t get_file_size(FILE*);

// reads one chunk of text from file.
// from - stream to opened file to read from
// buffer - returned string
// returns string's size, or negative value if error.
int read_file_chunk(FILE* from, wchar_t** buffer);

int write_chunk_to_file(FILE* dest, wchar_t* buffer);
#endif
