#include <stdio.h>
#include <stdlib.h>
#include "verbose.h"
#include "bpe.h"
#include "replacement.h"

//int copy_with_replacement(const wchar_t* from_buffer, size_t buffer_size,
//                          wchar_t* to_buffer, size_t to_buffer_size,
//                          replacement_t replacement) {
//    
//}
//
//int bpe_decode(wchar_t** from_buffer, size_t buffer_size) {
//    size_t initial_size = (*from_buffer)[0];
//    size_t table_size = 0;
//    replacement_t* table = rep_table_from_string(*from_buffer + 1, &table_size);
//    if(table == NULL) {
//        printf("Failed to load replacement table. File corrupted.\n");
//        return -1;
//    }
//    wchar_t* to_buffer = malloc(sizeof(wchar_t) * initial_size);
//    if(to_buffer == NULL) {
//        printf("Failed to allocate memory for swap buffer.\n");
//        return -2;
//    }
//    copy_buffer(from_buffer, buffer_size, to_buffer, initial_size);
//    for(size_t i = 0; i < table_size; ++i) {
//        replacement_t cur_rep = table[i];
//        int copy_res = copy_with_replacement(*from_buffer, buffer_size, 
//                                             to_buffer, initial_size,
//                                             cur_rep);
//        if(copy_res) {
//            printf("Failed to decode. Source is corrupted.\n");
//            return -3;
//        }
//
//    }
//    return 0;
//}
