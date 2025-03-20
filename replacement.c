#include <stdio.h>
#include <stdlib.h>
#include "replacement.h"

replacement_t* rep_table_new(size_t size) {
    return (replacement_t*) calloc(size, sizeof(replacement_t));
}
int rep_table_to_string(wchar_t* dest,
                        const replacement_t* table,
                        size_t table_size) {
    dest[0] = table_size;
#ifdef VISUAL_METADATA
    dest[0] += '0';
#endif
    for(size_t i = 0; i < table_size; ++i){
        size_t rev_id = table_size - i - 1;
        dest[i * 3 + 1] = table[rev_id].to;
        dest[i * 3 + 2] = table[rev_id].first;
        dest[i * 3 + 3] = table[rev_id].second;
    }
    return 0;
}
int rep_table_from_string(const wchar_t* str, size_t str_size, 
                          replacement_t** table, size_t* table_size) {
    if(str_size == 0) {
        *table = rep_table_new(0);
        return 0;
    }
    *table_size = str[0];
#ifdef VISUAL_METADATA
    *table_size -= '0';
#endif
    str += 1; // shift to avoid src[0] later
    *table = rep_table_new(*table_size);
    if(table == NULL) {
        printf("Unable to allocate memory for table.\n");
        return -1;
    }
    if(str_size <= *table_size * 3) {
        printf("Table data is too short. File corrupted.\n");
        free(*table);
        return -2;
    }
    for(size_t i = 0; i < *table_size; ++i) {
        (*table)[i].to = str[i * 3];
        (*table)[i].first = str[i * 3 + 1];
        (*table)[i].second = str[i * 3 + 2];
    }
    return 0;
}
