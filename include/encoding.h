#ifndef ENCODING_H
#define ENCODING_H

#include "strings.h"
#include "bytes.h"

typedef struct EncodingStruct EncodingStruct;
typedef Bytes (*GetBytesFn)(String);

struct EncodingStruct {
    GetBytesFn GetBytes;
};

extern const EncodingStruct Encoding;

#endif // ENCODING_H