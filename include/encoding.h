#ifndef ENCODING_H
#define ENCODING_H

#include "strings.h"
#include "bytes.h"

typedef struct EncodingStruct EncodingStruct;
typedef Bytes (*GetBytesFn)(String);
typedef String (*GetStringFn)(Bytes);

struct EncodingStruct {
    GetBytesFn GetBytes;
    GetStringFn GetString;
};

extern const EncodingStruct Encoding;

#endif // ENCODING_H