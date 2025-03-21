#include <assert.h>
#include <stdio.h>
#include "hash_map.h"

int main() {
    struct hash_map_t* hash = hash_map_new();
    hash_map_ctor(hash);
    printf("%zd\n", hash_map_get_cap(hash));
    return 0;
}
