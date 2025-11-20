#ifndef CONVERT_H
#define CONVERT_H

#include "strings.h"
#include "bytes.h"

typedef struct ConvertStruct ConvertStruct;
typedef String (*ToBase64Fn)(Bytes bytes);
typedef Bytes (*FromBase64Fn)(String string);

struct ConvertStruct {
    ToBase64Fn ToBase64;
    FromBase64Fn FromBase64;
};

extern const ConvertStruct Convert;

#endif // CONVERT_H