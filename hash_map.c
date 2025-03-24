#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "hash_map.h"

#define DEFAULT_CAPACITY 16
#define MAX_BUCKET_OVERFLOW 5
size_t hash_wchar_pair(wchar_t pair[2]) {
    return 2;
}

hash_map_t* hash_map_new() {
    return (hash_map_t*) malloc(sizeof(hash_map_t));
}
void buckets_ctor(bucket_t** buckets, size_t cap) {
    *buckets = (bucket_t*)malloc(sizeof(bucket_t) * cap);
    for(size_t buckid = 0; buckid < cap; ++buckid) {
        (*buckets)[buckid] = 
            (hkey_value_t*) malloc(sizeof(hkey_value_t) * MAX_BUCKET_OVERFLOW);
        for(size_t subid = 0; subid < MAX_BUCKET_OVERFLOW; ++subid) {
            (*buckets)[buckid][subid].is_used = false;
        }
    }
}
void hash_map_ctor(hash_map_t* hm) {
    hm->size = 0;
    hm->capacity = DEFAULT_CAPACITY;
    buckets_ctor(&hm->buckets, DEFAULT_CAPACITY);
}
void buckets_dtor(bucket_t* buckets, size_t cap) {
    for(size_t i = 0; i < cap; ++i) {
        free(buckets[i]);
    }
    free(buckets);
}
void hash_map_dtor(hash_map_t* hm) {
    buckets_dtor(hm->buckets, hm->capacity);
}
void hkey_value_copy(const hkey_value_t* from, hkey_value_t* to) {
    to->is_used = from->is_used;
    wcsncpy(to->key, from->key, 2);
    to->val = from->val;
}
void buckets_copy(const bucket_t* from, bucket_t* to, size_t copy_size) {
    for(size_t i = 0; i < copy_size; ++i) {
        hkey_value_copy(from[i], to[i]);
    }
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
    buckets_copy(hm->buckets, bucketscp, new_size);
    buckets_dtor(hm->buckets, hm->capacity);
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
    size_t new_capacity = new_size * 2;
    buckets_ctor(&buckcp, new_capacity);
    if(buckcp == NULL) {
        return -1;
    }
    buckets_copy(hm->buckets, buckcp, hm->size);
    buckets_dtor(hm->buckets, hm->capacity);
    hm->buckets = buckcp;
    hm->size = new_size;
    hm->capacity = new_capacity;
    return 0;
}
 
void hash_map_insert(hash_map_t* hm, hkey_t k, hvalue_t v) {
    if(hm->capacity == hm->size) {
        hash_map_reserve(hm, hm->capacity + 1);
    }
    size_t hash_idx = hash_wchar_pair(k) % hm->capacity;
    hkey_value_t hkv;
    wcsncpy(hkv.key, k, 2);
    hkv.val = v;
    hkv.is_used = true;
    size_t subid = 0;
    while (subid < MAX_BUCKET_OVERFLOW && hm->buckets[hash_idx][subid++].is_used);
    if(subid == MAX_BUCKET_OVERFLOW) {
        // rebuild buckets
        printf("rebuild hash map.\n");
        exit(1);
    }
    hm->buckets[hash_idx][subid] = hkv;
    ++hm->size;
}
void hash_map_remove(hash_map_t* hm, hkey_t  k);
int* hash_map_get(hash_map_t* hm, hkey_t k);
