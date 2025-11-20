#include "encoding.h"

#include <stdlib.h>
#include <string.h>

static Bytes Encoding_GetBytes(String string) {
    
    unsigned char* buf = (unsigned char*)malloc(string.Length);
    if (buf) {
        memcpy(buf, string.Value, string.Length);
    } else {
        exit(69);
    }
    
    Bytes bytes = {buf, string.Length};
    return bytes;
}

const EncodingStruct Encoding = {
    .GetBytes = Encoding_GetBytes
};