#include "verbose.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

bool is_verbose = false;
void verbose(const char* format, ...) {
    if(is_verbose) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}
void set_verbose() {
    is_verbose = true;
}
