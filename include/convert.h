#ifndef CONVERT_H
#define CONVERT_H

#include "strings.h"
#include "bytes.h"

typedef struct ConvertStruct ConvertStruct;
typedef String (*ToBase64Fn)(Bytes);

struct ConvertStruct {
    ToBase64Fn ToBase64;
};

extern const ConvertStruct Convert;

#endif // CONVERT_H