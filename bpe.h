#ifndef BPE_H
#define BPE_H

#include <stdio.h>

// encodes file source and saves result to dest
// returns 0 if success
// source - original text to encode. opened C stream FILE
// dest - destination file to store result in. opened C stream FILE
int bpe_encode_file(FILE* source, FILE* dest);
// decodes file encoded by this algo
// returns 0 if success
// source - encoded text. opened C stream FILE
// dest - directory to store original decoded text. opened C stream FILE
int bpe_decode_file(FILE* source, FILE* dest);

#endif

