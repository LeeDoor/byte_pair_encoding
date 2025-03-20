#include "metadata.h"
#include <stdlib.h>

metadata_t* metadata_new(size_t table_size) {
    metadata_t* metadata = (metadata_t*) malloc(sizeof(metadata_t));
    metadata->replacement_table = rep_table_new(table_size);
    return metadata;
}
size_t metadata_accumulate_string_size(const metadata_t* metadata) {
    return 2 + metadata->table_size * 3;
}
int metadata_to_string(const metadata_t* metadata, 
                       wchar_t* buffer, size_t buffer_size) {
    if(buffer_size == 0) return 0;
    if(buffer_size == 1) return 1;
    buffer[0] = metadata->initial_size;
#ifdef VISUAL_METADATA
    buffer[0] += 'a';    
#endif
    return rep_table_to_string(buffer + 1, 
                        metadata->replacement_table, metadata->table_size);
}
int metadata_from_string(const wchar_t* str, size_t str_size, metadata_t* metadata) {
    metadata->initial_size = str[0];
#ifdef VISUAL_METADATA
    metadata->initial_size -= 'a';        
#endif
    str += sizeof(wchar_t);
    return rep_table_from_string(str, str_size, 
                                 &metadata->replacement_table, 
                                 &metadata->table_size);
}
