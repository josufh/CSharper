#include "encoding.h"

#include <stdlib.h>
#include <string.h>
#include "gc.h"

static Bytes Encoding_GetBytes(String string) {
    size_t len = strlen(string.Value);
    unsigned char* buf = (unsigned char*)GC_Alloc(len);
    memcpy(buf, string.Value, len);
    
    Bytes bytes = {buf, len};
    return bytes;
}

static String Encoding_GetString(Bytes bytes) {
    size_t byte_len = bytes.Length;

    char* buf = (char*)GC_Alloc(byte_len + 1);
    memcpy(buf, bytes.Data, byte_len);
    buf[byte_len] = '\0';

    size_t char_len = 0;
    for (size_t i = 0; i < byte_len; ++i) {
        unsigned char c = (unsigned char)buf[i];
        if ((c & 0xC0) != 0x80) {
            ++char_len;
        }
    }

    String string = { buf, char_len };
    return string;
}

const EncodingStruct Encoding = {
    .GetBytes = Encoding_GetBytes,
    .GetString = Encoding_GetString
};