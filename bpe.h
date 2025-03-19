#ifndef BPE_H
#define BPE_H
#include <stddef.h>

int bpe_encode(wchar_t**, size_t);
int bpe_decode(wchar_t**, size_t);

#endif
