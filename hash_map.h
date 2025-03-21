#ifndef HASH_H
#define HASH_H

#include <stddef.h>
struct hash_map_t;

struct hash_map_t* hash_map_new();
void hash_map_ctor(struct hash_map_t* hm);

size_t hash_map_get_size(struct hash_map_t*);
size_t hash_map_get_cap(struct hash_map_t*);
int hash_map_reserve(struct hash_map_t* hm, size_t new_capacity);
int hash_map_resize(struct hash_map_t* hm, size_t new_size);
void hash_map_insert(int k, int v);
void hash_map_remove(int k);
int* hash_map_get(int k);

#endif
