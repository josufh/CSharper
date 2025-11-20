#ifndef ENCODING_H
#define ENCODING_H

#include "strings.h"
#include "bytes.h"

typedef struct EncodingStruct EncodingStruct;
typedef Bytes (*GetBytesFn)(String string);
typedef String (*GetStringFn)(Bytes bytes);

struct EncodingStruct {
    GetBytesFn GetBytes;
    GetStringFn GetString;
};

extern const EncodingStruct Encoding;

#endif // ENCODING_H