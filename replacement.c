#include <stdlib.h>
#include "replacement.h"

replacement_t* rep_table_new(size_t size) {
    return (replacement_t*) malloc(sizeof(replacement_t) * size);
}
int rep_table_to_string(const replacement_t* table,
                        size_t table_size,
                        size_t initial_buffer_size,
                        wchar_t* dest) {
    dest[0] = table_size;
    dest[1] = initial_buffer_size;
    for(size_t i = 0; i < table_size; ++i){
        size_t rev_id = table_size - i - 1;
        dest[i * 3 + 2] = table[rev_id].to;
        dest[i * 3 + 3] = table[rev_id].first;
        dest[i * 3 + 4] = table[rev_id].second;
    }
    return 0;
}
replacement_t* rep_table_from_string(const wchar_t* src);
int rep_table_add(replacement_t* table,
                  size_t id, 
                  wchar_t first,
                  wchar_t second,
                  wchar_t to) {
    replacement_t rep; 
    rep.first = first; 
    rep.second = second; 
    rep.to = to;
    table[id] = rep;
    return 0;
}
