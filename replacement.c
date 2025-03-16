#include "replacement.h"

int rep_table_to_string(const replacement_t* table,
                        size_t table_size,
                        wchar_t* dest) {
    dest[0] = iteration;
    for(size_t i = 0; i < iteration; ++i){
        dest[i + 1] = rep_table[i].to;
        dest[i + 2] = rep_table[i].first;
        dest[i + 3] = rep_table[i].second;
    }
}
replacement_t* rep_table_from_string(const wchar_t* src);
