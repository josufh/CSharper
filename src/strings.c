#include "../include/strings.h" // for some reason vscode is not getting strings.h

#include <string.h>
#include "../include/gc.h"

static String Strings_NewFn(const char* text) {
    size_t byte_len  = strlen(text);
    
    char* buf = (char*)GC_Alloc(byte_len  + 1);
    memcpy(buf, text, byte_len  + 1);

    size_t char_len = 0;
    for (size_t i = 0; i < byte_len; ++i) {
        unsigned char c = (unsigned char)buf[i];
        if ((c & 0xC0) != 0x80) {
            ++char_len;
        }
    }
    
    String string = {buf, char_len};
    return string;
}

const StringsStruct Strings = {
    .New = Strings_NewFn
};