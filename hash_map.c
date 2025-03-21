#include <stddef.h>
#include <stdlib.h>

typedef char pair_t[2];
typedef pair_t* bucket_t; 
typedef bucket_t* buckets_t;
typedef struct {
    size_t size;
    size_t  capacity;
    buckets_t buckets;
} hash_map_t;

hash_map_t* hash_map_new() {
    return (hash_map_t*) malloc(sizeof(hash_map_t));
}
void hash_map_ctor(hash_map_t* hm) {
    hm->size = 0;
    hm->capacity = 16;
    hm->buckets = (bucket_t*)malloc(sizeof(bucket_t) * hm->capacity);
    for(size_t i = 0; i < hm->capacity; ++i) {
        hm->buckets[i] = (pair_t*) malloc(sizeof(char[2]) * 5);
    }
}
void hash_map_dtor(hash_map_t* hm) {
    for(int i = 0; i < hm->capacity; ++i) {
        free(hm->buckets[i]);
    }
    free(hm->buckets);
}

void hash_map_insert(hash_map_t* hm, int k, int v);
void hash_map_remove(hash_map_t* hm, int k);
int* hash_map_get(hash_map_t* hm, int k);
