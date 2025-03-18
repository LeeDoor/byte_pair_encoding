#ifndef BPE_H
#define BPE_H
#include <stddef.h>

size_t bpe_encode(wchar_t**, size_t);
size_t bpe_decode(wchar_t**, size_t);

#endif
