#include "strings.h"
#include <stdlib.h>
#include <string.h>

static String Strings_NewFn(const char* text) {
    size_t len = strlen(text);
    
    char* buf = (char*)malloc(len + 1);
    if (buf) {
        memcpy(buf, text, len + 1);
    } else {
        exit(69);
    }
    
    String string = {buf, len};
    return string;
}

const StringsStruct Strings = {
    .New = Strings_NewFn
};