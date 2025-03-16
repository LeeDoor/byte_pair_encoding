#ifndef REPLACEMENT_T
#define REPLACEMENT_T

#include <wchar.h>

typedef struct {
    wchar_t first;
    wchar_t second;
    wchar_t to;
} replacement_t;

replacement_t* rep_table_new(size_t size);
int rep_table_to_string(const replacement_t* src,
                        size_t table_size,
                        wchar_t* dest);
replacement_t* rep_table_from_string(const wchar_t* src);
#endif
