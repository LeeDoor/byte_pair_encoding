#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "hash_map.h"
#define KYEL  "\x1B[33m"
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
void test_resize_capacity() {
    printf("%sTesting rezising and changing capacity..", KYEL);
    hash_map_t* hash = hash_map_new();
    hash_map_ctor(hash);
    assert(hash_map_get_size(hash) == 0);
    assert(hash_map_get_cap(hash) == 16);

    hash_map_reserve(hash, 16);
    assert(hash_map_get_cap(hash) == 16);
    assert(hash_map_get_size(hash) == 0);

    hash_map_reserve(hash, 15);
    assert(hash_map_get_cap(hash) == 15);
    assert(hash_map_get_size(hash) == 0);

    hash_map_reserve(hash, 20);
    assert(hash_map_get_cap(hash) == 20);
    assert(hash_map_get_size(hash) == 0);

    hash_map_resize(hash, 5);
    assert(hash_map_get_cap(hash) == 20);
    assert(hash_map_get_size(hash) == 5);

    hash_map_resize(hash, 21);
    assert(hash_map_get_cap(hash)  >= 21);
    assert(hash_map_get_size(hash) == 21);

    hash_map_resize(hash, 5);
    assert(hash_map_get_cap(hash) == 42);
    assert(hash_map_get_size(hash) == 5);

    hash_map_reserve(hash, 4);
    assert(hash_map_get_cap(hash) == 4);
    assert(hash_map_get_size(hash) == 4);
    hash_map_dtor(hash);
    free(hash);
    printf(".passed!\n");
}
void test_insert() {
    printf("%sTesting inserting..", KYEL);
    hash_map_t* hash = hash_map_new();
    hash_map_ctor(hash);

    hash_map_insert(hash, L"ab", 1);    
    assert(hash_map_get_cap(hash) >= 1);
    assert(hash_map_get_size(hash) == 1);
    assert(*hash_map_get(hash, L"ab") == 1);

    hash_map_dtor(hash);
    free(hash);
    printf(".passed!\n");
}
int main() {
    printf("%sStarting tests.\n", KYEL);
    test_resize_capacity();
    test_insert();
    printf("%sAll tests passed!\n", KGRN);
    return 0;
}
