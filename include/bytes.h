#ifndef BYTES_H
#define BYTES_H

#include <stddef.h>

typedef struct Bytes Bytes;

struct Bytes {
    const unsigned char* Data;
    const size_t Length;
};

#endif // BYTES_H