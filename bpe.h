#ifndef BPE_H
#define BPE_H

#include <stdio.h>

// encodes file source and saves result to dest
// returns 0 if success
// source - original text to encode. opened C stream FILE
// dest - destination file to store result in. opened C stream FILE
int bpe(FILE* source, FILE* dest);

#endif

