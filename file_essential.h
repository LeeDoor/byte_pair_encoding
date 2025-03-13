#ifndef FILE_ESSENTIAL_H
#define FILE_ESSENTIAL_H

#include <stdio.h>

#define FERROR(errc, text, fs) \
    printf(text); \
    fclose(fs); \
    return errc

typedef unsigned long long char_amount_t;
// gets file's size. moving cursor to the begining of the file.
// takes pointer to opened file
char_amount_t get_file_size(FILE*);

#endif
