#ifndef METADATA_H
#define METADATA_H
#include <stddef.h>
#include "replacement.h"
// #define VISUAL_METADATA
typedef struct {
    size_t initial_size;
    replacement_t* replacement_table;
    size_t table_size;
} metadata_t;

// allocates memory for metadata
metadata_t* metadata_new(size_t table_size);
size_t metadata_accumulate_string_size(const metadata_t*);
int metadata_to_string(const metadata_t*, wchar_t* buffer, size_t buffer_size);
int metadata_from_string(const wchar_t* str, size_t str_size, metadata_t* metadata);

#endif
