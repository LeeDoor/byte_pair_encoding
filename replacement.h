#ifndef REPLACEMENT_T
#define REPLACEMENT_T

#include <wchar.h>

typedef struct {
    wchar_t first;
    wchar_t second;
    wchar_t to;
} replacement_t;

replacement_t* rep_table_new(size_t size);
int rep_table_to_string(wchar_t* dest,
                        const replacement_t* src,
                        size_t table_size);
int rep_table_from_string(const wchar_t* str, size_t str_size, 
                          replacement_t** table, size_t* table_size);
int rep_table_add(replacement_t* table,
                  size_t id, 
                  wchar_t first,
                  wchar_t second,
                  wchar_t to);
#endif
