#include <stddef.h>
#include <stdlib.h>
#include <wchar.h>

typedef int hvalue_t;
typedef wchar_t hkey_t[2];
typedef struct {
    hkey_t key;
    hvalue_t val;
} hkey_value_t;
typedef hkey_value_t* bucket_t; 
typedef struct {
    size_t size;
    size_t  capacity;
    bucket_t* buckets;
} hash_map_t;
#define DEFAULT_CAPACITY 16
#define MAX_BUCKET_OVERFLOW 5
size_t hash_wchar_pair(wchar_t pair[2]) {
    return 2;
}

hash_map_t* hash_map_new() {
    return (hash_map_t*) malloc(sizeof(hash_map_t));
}
void buckets_ctor(bucket_t** buckets, size_t size) {
    *buckets = (bucket_t*)malloc(sizeof(bucket_t) * size);
    for(size_t i = 0; i < size; ++i) {
        (*buckets)[i] = (hkey_value_t*) malloc(sizeof(hkey_value_t) * MAX_BUCKET_OVERFLOW);
    }
}
void hash_map_ctor(hash_map_t* hm) {
    hm->size = 0;
    hm->capacity = DEFAULT_CAPACITY;
    buckets_ctor(&hm->buckets, DEFAULT_CAPACITY);
}
void hash_map_dtor(hash_map_t* hm) {
    for(size_t i = 0; i < hm->capacity; ++i) {
        free(hm->buckets[i]);
    }
    free(hm->buckets);
}

size_t hash_map_get_size(hash_map_t* hm) {
    return hm->size;
}
size_t hash_map_get_cap(hash_map_t* hm) {
    return hm->capacity;
}

int hash_map_reserve(hash_map_t* hm, size_t new_capacity){
    if(new_capacity == hm->capacity) return 0;
    size_t new_size = hm->size < new_capacity ? hm->size : new_capacity;
    bucket_t* bucketscp;
    buckets_ctor(&bucketscp, new_capacity); 
    if(bucketscp == NULL) {
        return -1;
    }
    for(size_t i = 0; i < new_size; ++i) {
        bucketscp[i] = hm->buckets[i];
    }
    free(hm->buckets);
    hm->buckets = bucketscp;
    hm->capacity = new_capacity;
    hm->size = new_size;
    return 0;
}
int hash_map_resize(hash_map_t* hm, size_t new_size) {
    if(new_size == hm->size) return 0;
    if(new_size <= hm->capacity) {
        hm->size = new_size;
        return 0;
    }
    bucket_t* buckcp;
    buckets_ctor(&buckcp, new_size * 2);
    if(buckcp == NULL) {
        return -1;
    }
    for(size_t i = 0; i < hm->size; ++i){
        buckcp[i] = hm->buckets[i];
    }
    free(hm->buckets);
    hm->buckets = buckcp;
    hm->size = new_size;
    return 0;
}
// 
// void hash_map_insert(hash_map_t* hm, hkey_t k, hvalue_t v) {
//     size_t hash_idx = hash_wchar_pair(k);
//     hkey_value_t hkv;
//     wcsncpy(hkv.key, k, 2);
//     hkv.val = v;
// 
//     if(hm->capacity <= hm->size) {
//         
//     }
// }
// void hash_map_remove(hash_map_t* hm, hkey_t  k);
// int* hash_map_get(hash_map_t* hm, hkey_t k);
