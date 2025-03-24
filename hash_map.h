#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdbool.h>

typedef int hvalue_t;
typedef wchar_t hkey_t[2];
typedef struct {
    bool is_used;
    hkey_t key;
    hvalue_t val;
} hkey_value_t;
typedef hkey_value_t* bucket_t; 
typedef struct {
    size_t size;
    size_t  capacity;
    bucket_t* buckets;
} hash_map_t;

hash_map_t* hash_map_new();
void hash_map_ctor(hash_map_t* hm);
void hash_map_dtor(hash_map_t* hm);

size_t hash_map_get_size(hash_map_t*);
size_t hash_map_get_cap(hash_map_t*);
int hash_map_reserve(hash_map_t* hm, size_t new_capacity);
int hash_map_resize(hash_map_t* hm, size_t new_size);
void hash_map_insert(hash_map_t*, hkey_t k, hvalue_t v);
void hash_map_remove(hash_map_t*, hkey_t k);
int* hash_map_get(hash_map_t*, hkey_t k);

#endif
