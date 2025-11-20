#include "encoding.h"

#include <stdlib.h>
#include <string.h>
#include "gc.h"

static Bytes Encoding_GetBytes(String string) {
    unsigned char* buf = (unsigned char*)GC_Alloc(string.Length);
    memcpy(buf, string.Value, string.Length);
    
    Bytes bytes = {buf, string.Length};
    return bytes;
}

const EncodingStruct Encoding = {
    .GetBytes = Encoding_GetBytes
};