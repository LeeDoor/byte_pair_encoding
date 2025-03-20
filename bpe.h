#ifndef BPE_H
#define BPE_H
#include <stddef.h>
#define SWAP_BUFFERS(a, b) \
wchar_t* swap = a; \
a = b; \
b = swap

int bpe_encode(wchar_t**, size_t);
int bpe_decode(wchar_t**, size_t);

#endif
