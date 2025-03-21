#ifndef HASH_H
#define HASH_H

struct hash_map_t;

hash_map_t* hash_map_new();
void hash_map_ctor();

void hash_map_insert(int k, int v);
void hash_map_remove(int k);
int* hash_map_get(int k);

#endif
