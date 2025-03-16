#include <stdlib.h>
#include "replacement.h"

replacement_t* rep_table_new(size_t size) {
    return (replacement_t*) malloc(sizeof(replacement_t) * size);
}
int rep_table_to_string(const replacement_t* table,
                        size_t table_size,
                        wchar_t* dest) {
    dest[0] = table_size;
    for(size_t i = 0; i < table_size; ++i){
        dest[i + 1] = table[i].to;
        dest[i + 2] = table[i].first;
        dest[i + 3] = table[i].second;
    }
    return 0;
}
replacement_t* rep_table_from_string(const wchar_t* src);
